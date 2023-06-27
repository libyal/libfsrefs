/*
 * Ministore node (or level 2 metadata) functions
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

#if !defined( _LIBFSREFS_MINISTORE_NODE_H )
#define _LIBFSREFS_MINISTORE_NODE_H

#include <common.h>
#include <types.h>

#include "libfsrefs_io_handle.h"
#include "libfsrefs_libbfio.h"
#include "libfsrefs_libcdata.h"
#include "libfsrefs_libcerror.h"
#include "libfsrefs_node_record.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsrefs_ministore_node libfsrefs_ministore_node_t;

struct libfsrefs_ministore_node
{
	/* The data
	 */
	uint8_t *data;

	/* The data size
	 */
	size_t data_size;

	/* The records array
	 */
	libcdata_array_t *records_array;
};

int libfsrefs_ministore_node_initialize(
     libfsrefs_ministore_node_t **ministore_node,
     libcerror_error_t **error );

int libfsrefs_ministore_node_free(
     libfsrefs_ministore_node_t **ministore_node,
     libcerror_error_t **error );

int libfsrefs_ministore_node_read_data(
     libfsrefs_ministore_node_t *ministore_node,
     libfsrefs_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsrefs_ministore_node_read_file_io_handle(
     libfsrefs_ministore_node_t *ministore_node,
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

int libfsrefs_ministore_node_get_record(
     libfsrefs_ministore_node_t *ministore_node,
     const uint8_t *key_data,
     size_t key_data_size,
     libfsrefs_node_record_t **node_record,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSREFS_MINISTORE_NODE_H ) */

