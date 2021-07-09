#!/bin/sh
#
# Copyright (c) 2021 Canonical Ltd.
# Author: Krzysztof Kozlowski <krzysztof.kozlowski@canonical.com>
#                             <krzk@kernel.org>
#
# SPDX-License-Identifier: GPL-2.0
#

set -e -E

apt update

apt install -y --no-install-recommends \
	autoconf \
	automake \
	build-essential \
	libdbus-1-dev \
	libglib2.0-dev \
	libnl-3-dev \
	libnl-genl-3-dev \
	libtool \

