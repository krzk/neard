/*
 *
 *  neard - Near Field Communication manager
 *
 *  Copyright (C) 2012  Intel Corporation. All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <glib.h>

#include <gdbus.h>

#include "near.h"

static DBusConnection *connection = NULL;
static GHashTable *ndef_app_hash;

struct near_ndef_agent {
	char *sender;
	char *path;
	char *record_type;
	guint watch;
};

static void ndef_agent_free(gpointer data)
{
	struct near_ndef_agent *agent = data;

	if (agent == NULL)
		return;

	g_free(agent->sender);
	g_free(agent->path);
}

static void ndef_agent_disconnect(DBusConnection *conn, void *user_data)
{
	struct near_ndef_agent *agent = user_data;

	DBG("agent %s disconnected", agent->path);

	ndef_agent_free(agent);
}

int __near_agent_ndef_register(const char *sender, const char *path,
						const char *record_type)
{
	struct near_ndef_agent *agent;
	char *rec_type;

	DBG("%s registers path %s for %s", sender, path, record_type);

	if (g_hash_table_lookup(ndef_app_hash, record_type) != NULL)
		return -EEXIST;

	agent = g_try_malloc0(sizeof(struct near_ndef_agent));
	if (agent == NULL)
		return -ENOMEM;

	agent->sender = g_strdup(sender);
	agent->path = g_strdup(path);
	rec_type = g_strdup(record_type);
	
	if (agent->sender == NULL || agent->path == NULL ||
	    rec_type == NULL) {
		g_free(agent);
		return -ENOMEM;
	}

	agent->watch = g_dbus_add_disconnect_watch(connection, sender,
						ndef_agent_disconnect, agent, NULL);


	g_hash_table_insert(ndef_app_hash, rec_type, agent);

	return 0;
}

int __near_agent_ndef_unregister(const char *sender, const char *path,
						const char *record_type)
{
	struct near_ndef_agent *agent;

	DBG("sender %s path %s type %s", sender, path, record_type);

	agent = g_hash_table_lookup(ndef_app_hash, record_type);
	if (agent == NULL)
		return -EINVAL;

	if (strcmp(agent->path, path) != 0 || strcmp(agent->sender, sender) != 0)
		return -EINVAL;

	g_dbus_remove_watch(connection, agent->watch);

	g_hash_table_remove(ndef_app_hash, record_type);

	return 0;
}

static guint handover_agent_watch = 0;
static gchar *handover_agent_path = NULL;
static gchar *handover_agent_sender = NULL;

static void handover_agent_free(void)
{
	handover_agent_watch = 0;

	g_free(handover_agent_sender);
	handover_agent_sender = NULL;

	g_free(handover_agent_path);
	handover_agent_path = NULL;
}

static void handover_agent_disconnect(DBusConnection *conn, void *data)
{
	DBG("data %p", data);

	handover_agent_free();
}

int __near_agent_handover_register(const char *sender, const char *path)
{
	DBG("sender %s path %s", sender, path);

	if (handover_agent_path != NULL)
		return -EEXIST;

	handover_agent_sender = g_strdup(sender);
	handover_agent_path = g_strdup(path);

	handover_agent_watch = g_dbus_add_disconnect_watch(connection, sender,
					handover_agent_disconnect, NULL, NULL);

	return 0;
}

int __near_agent_handover_unregister(const char *sender, const char *path)
{
	DBG("sender %s path %s", sender, path);

	if (handover_agent_path == NULL)
		return -ESRCH;

	if (handover_agent_watch > 0)
		g_dbus_remove_watch(connection, handover_agent_watch);

	handover_agent_free();

	return 0;
}

int __near_agent_init(void)
{
	DBG("");

	connection = near_dbus_get_connection();
	if (connection == NULL)
		return -1;

	ndef_app_hash = g_hash_table_new_full(g_str_hash, g_str_equal,
						g_free, ndef_agent_free);

	return 0;
}

void __near_agent_cleanup(void)
{
	DBG("");

	g_hash_table_destroy(ndef_app_hash);
	ndef_app_hash = NULL;

	handover_agent_free();
}
