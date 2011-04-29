/*
 *
 *  neard - Near Field Communication manager
 *
 *  Copyright (C) 2011  Intel Corporation. All rights reserved.
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

#include <glib.h>

#include <near/types.h>

struct near_adapter;
struct near_target;

#include <near/log.h>

int __near_log_init(const char *debug, gboolean detach);
void __near_log_cleanup(void);

#include <near/dbus.h>

int __near_dbus_init(DBusConnection *conn);
void __near_dbus_cleanup(void);

DBusMessage *__near_error_failed(DBusMessage *msg, int errnum);
DBusMessage *__near_error_invalid_arguments(DBusMessage *msg);
DBusMessage *__near_error_permission_denied(DBusMessage *msg);
DBusMessage *__near_error_passphrase_required(DBusMessage *msg);
DBusMessage *__near_error_not_registered(DBusMessage *msg);
DBusMessage *__near_error_not_unique(DBusMessage *msg);
DBusMessage *__near_error_not_supported(DBusMessage *msg);
DBusMessage *__near_error_not_implemented(DBusMessage *msg);
DBusMessage *__near_error_not_found(DBusMessage *msg);
DBusMessage *__near_error_no_carrier(DBusMessage *msg);
DBusMessage *__near_error_in_progress(DBusMessage *msg);
DBusMessage *__near_error_already_exists(DBusMessage *msg);
DBusMessage *__near_error_already_enabled(DBusMessage *msg);
DBusMessage *__near_error_already_disabled(DBusMessage *msg);
DBusMessage *__near_error_already_connected(DBusMessage *msg);
DBusMessage *__near_error_not_connected(DBusMessage *msg);
DBusMessage *__near_error_operation_aborted(DBusMessage *msg);
DBusMessage *__near_error_operation_timeout(DBusMessage *msg);
DBusMessage *__near_error_invalid_service(DBusMessage *msg);
DBusMessage *__near_error_invalid_property(DBusMessage *msg);

int __near_manager_adapter_add(guint32 idx, const char *name, guint32 protocols);
void __near_manager_adapter_remove(guint32 idx);
int __near_manager_init(DBusConnection *conn);
void __near_manager_cleanup(void);

struct near_adapter * __near_adapter_create(guint32 idx,
				const char *name, guint32 protocols);
void __near_adapter_destroy(struct near_adapter *adapter);
const char *__near_adapter_get_path(struct near_adapter *adapter);
struct near_adapter *__near_adapter_get(guint32 idx);
int __near_adapter_add(struct near_adapter *adapter);
void __near_adapter_remove(struct near_adapter *adapter);
int __near_adapter_add_target(guint32 idx, struct near_target *target);
int __near_adapter_remove_target(guint32 idx);
void __near_adapter_list(DBusMessageIter *iter, void *user_data);
int __near_adapter_init(void);
void __near_adapter_cleanup(void);

enum near_target_type {
	NEAR_TARGET_TYPE_TAG = 0,
	NEAR_TARGET_TYPE_DEVICE = 1,
};

const char *__near_target_get_path(struct near_target *target);
near_uint16_t __near_target_get_tag_type(struct near_target *target);
guint32 __near_target_get_idx(struct near_target *target);
guint32 __near_target_get_adapter_idx(struct near_target *target);
int __near_target_add(guint32 adapter_idx, guint32 target_idx,
		guint32 protocols, enum near_target_type type,
		near_uint16_t sens_res, near_uint8_t sel_res);
void __near_target_remove(guint32 target_idx);
int __near_target_init(void);
void __near_target_cleanup(void);

#include <near/tag.h>

int __near_tag_read(struct near_target *target, void *buf, size_t length);

int __near_netlink_get_adapters(void);
int __near_netlink_start_poll(int idx, guint32 protocols);
int __near_netlink_stop_poll(int idx);
int __near_netlink_init(void);
void __near_netlink_cleanup(void);

#include <near/plugin.h>

int __near_plugin_init(const char *pattern, const char *exclude);
void __near_plugin_cleanup(void);
