/*
 * Ministore node header functions
 *
 * Copyright (C) 2012-2024, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBFSREFS_NODE_HEADER_H )
#define _LIBFSREFS_NODE_HEADER_H

#include <common.h>
#include <types.h>

#include "libfsrefs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsrefs_node_header libfsrefs_node_header_t;

struct libfsrefs_node_header
{
	/* The data area start offset
	 */
	uint32_t data_area_start_offset;

	/* The data area end offset
	 */
	uint32_t data_area_end_offset;

	/* The record offsets start offset
	 */
	uint32_t record_offsets_start_offset;

	/* The record offsets end offset
	 */
	uint32_t record_offsets_end_offset;

	/* The number of record offsets
	 */
	uint32_t number_of_record_offsets;

	/* The node type flags
	 */
	uint8_t node_type_flags;
};

int libfsrefs_node_header_initialize(
     libfsrefs_node_header_t **node_header,
     libcerror_error_t **error );

int libfsrefs_node_header_free(
     libfsrefs_node_header_t **node_header,
     libcerror_error_t **error );

int libfsrefs_node_header_read_data(
     libfsrefs_node_header_t *node_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSREFS_NODE_HEADER_H ) */

