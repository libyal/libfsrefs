/*
 * Objects tree functions
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

#if !defined( _LIBFSREFS_OBJECTS_TREE_H )
#define _LIBFSREFS_OBJECTS_TREE_H

#include <common.h>
#include <types.h>

#include "libfsrefs_file_system.h"
#include "libfsrefs_io_handle.h"
#include "libfsrefs_libbfio.h"
#include "libfsrefs_libcerror.h"
#include "libfsrefs_ministore_node.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsrefs_objects_tree libfsrefs_objects_tree_t;

struct libfsrefs_objects_tree
{
	/* File system
	 */
	libfsrefs_file_system_t *file_system;

	/* Ministore root node
	 */
	libfsrefs_ministore_node_t *root_node;
};

int libfsrefs_objects_tree_initialize(
     libfsrefs_objects_tree_t **objects_tree,
     libfsrefs_file_system_t *file_system,
     libcerror_error_t **error );

int libfsrefs_objects_tree_free(
     libfsrefs_objects_tree_t **objects_tree,
     libcerror_error_t **error );

int libfsrefs_objects_tree_read(
     libfsrefs_objects_tree_t *objects_tree,
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libfsrefs_objects_get_ministore_tree_by_identifier(
     libfsrefs_objects_tree_t *objects_tree,
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint64_t object_identifier,
     libfsrefs_ministore_node_t **root_node,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSREFS_OBJECTS_TREE_H ) */

