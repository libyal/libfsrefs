/*
 * Ministore node record functions
 *
 * Copyright (C) 2012-2023, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#if !defined( _LIBFSREFS_NODE_RECORD_H )
#define _LIBFSREFS_NODE_RECORD_H

#include <common.h>
#include <types.h>

#include "libfsrefs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsrefs_node_record libfsrefs_node_record_t;

struct libfsrefs_node_record
{
	/* The size
	 */
	uint32_t size;

	/* The flags
	 */
	uint16_t flags;

	/* The key data
	 */
	const uint8_t *key_data;

	/* The key data size
	 */
	uint16_t key_data_size;

	/* The value data
	 */
	const uint8_t *value_data;

	/* The value data size
	 */
	uint16_t value_data_size;
};

int libfsrefs_node_record_initialize(
     libfsrefs_node_record_t **node_record,
     libcerror_error_t **error );

int libfsrefs_node_record_free(
     libfsrefs_node_record_t **node_record,
     libcerror_error_t **error );

int libfsrefs_node_record_read_data(
     libfsrefs_node_record_t *node_record,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSREFS_NODE_RECORD_H ) */

