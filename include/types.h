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

#ifndef __NEAR_TYPES_H
#define __NEAR_TYPES_H

#include <byteswap.h>
#include <endian.h>

#define near_get_unaligned(ptr)			\
({						\
	struct __attribute__((packed)) {	\
		typeof(*(ptr)) __v;		\
	} *__p = (typeof(__p)) (ptr);		\
	__p->__v;				\
})

#define near_put_unaligned(val, ptr)		\
do {						\
	struct __attribute__((packed)) {	\
		typeof(*(ptr)) __v;		\
	} *__p = (typeof(__p)) (ptr);		\
	__p->__v = (val);			\
} while (0)

#if __BYTE_ORDER == __LITTLE_ENDIAN
static inline uint64_t near_get_le64(const void *ptr)
{
	return near_get_unaligned((const uint64_t *) ptr);
}

static inline uint64_t near_get_be64(const void *ptr)
{
	return bswap_64(near_get_unaligned((const uint64_t *) ptr));
}

static inline uint32_t near_get_le32(const void *ptr)
{
	return near_get_unaligned((const uint32_t *) ptr);
}

static inline uint32_t near_get_be32(const void *ptr)
{
	return bswap_32(near_get_unaligned((const uint32_t *) ptr));
}

static inline uint16_t near_get_le16(const void *ptr)
{
	return near_get_unaligned((const uint16_t *) ptr);
}

static inline uint16_t near_get_be16(const void *ptr)
{
	return bswap_16(near_get_unaligned((const uint16_t *) ptr));
}

static inline void near_put_le64(uint64_t val, const void *ptr)
{
	near_put_unaligned(val, (uint64_t *) ptr);
}

static inline void near_put_be64(uint64_t val, const void *ptr)
{
	near_put_unaligned(bswap_64(val), (uint64_t *) ptr);
}

static inline void near_put_le32(uint32_t val, const void *ptr)
{
	near_put_unaligned(val, (uint32_t *) ptr);
}

static inline void near_put_be32(uint32_t val, const void *ptr)
{
	near_put_unaligned(bswap_32(val), (uint32_t *) ptr);
}

static inline void near_put_le16(uint16_t val, const void *ptr)
{
	near_put_unaligned(val, (uint16_t *) ptr);
}

static inline void near_put_be16(uint16_t val, const void *ptr)
{
	near_put_unaligned(bswap_16(val), (uint16_t *) ptr);
}

#elif __BYTE_ORDER == __BIG_ENDIAN
static inline uint64_t near_get_le64(const void *ptr)
{
	return bswap_64(near_get_unaligned((const uint64_t *) ptr));
}

static inline uint64_t near_get_be64(const void *ptr)
{
	return near_get_unaligned((const uint64_t *) ptr);
}

static inline uint32_t near_get_le32(const void *ptr)
{
	return bswap_32(near_get_unaligned((const uint32_t *) ptr));
}

static inline uint32_t near_get_be32(const void *ptr)
{
	return near_get_unaligned((const uint32_t *) ptr);
}

static inline uint16_t near_get_le16(const void *ptr)
{
	return bswap_16(near_get_unaligned((const uint16_t *) ptr));
}

static inline uint16_t near_get_be16(const void *ptr)
{
	return near_get_unaligned((const uint16_t *) ptr);
}

static inline void near_put_le64(uint64_t val, const void *ptr)
{
	near_put_unaligned(bswap_64(val), (uint64_t *) ptr);
}

static inline void near_put_be64(uint64_t val, const void *ptr)
{
	near_put_unaligned(val, (uint64_t *) ptr);
}

static inline void near_put_le32(uint32_t val, const void *ptr)
{
	near_put_unaligned(bswap_32(val), (uint32_t *) ptr);
}

static inline void near_put_be32(uint32_t val, const void *ptr)
{
	near_put_unaligned(val, (uint32_t *) ptr);
}

static inline void near_put_le16(uint16_t val, const void *ptr)
{
	near_put_unaligned(bswap_16(val), (uint16_t *) ptr);
}

static inline void near_put_be16(uint16_t val, const void *ptr)
{
	near_put_unaligned(val, (uint16_t *) ptr);
}
#else
#error "Unknown byte order"
#endif

#endif
