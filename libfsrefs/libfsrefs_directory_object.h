/*
 * Directory object functions
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

#if !defined( _LIBFSREFS_DIRECTORY_OBJECT_H )
#define _LIBFSREFS_DIRECTORY_OBJECT_H

#include <common.h>
#include <types.h>

#include "libfsrefs_block_tree.h"
#include "libfsrefs_directory_entry.h"
#include "libfsrefs_io_handle.h"
#include "libfsrefs_libbfio.h"
#include "libfsrefs_libcerror.h"
#include "libfsrefs_ministore_node.h"
#include "libfsrefs_objects_tree.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsrefs_directory_object libfsrefs_directory_object_t;

struct libfsrefs_directory_object
{
	/* Object identifier
	 */
	uint64_t object_identifier;

	/* Objects tree
	 */
	libfsrefs_objects_tree_t *objects_tree;

	/* Ministore root node
	 */
	libfsrefs_ministore_node_t *root_node;

	/* The (Ministore) node block tree
	 */
	libfsrefs_block_tree_t *node_block_tree;

	/* The directory entries array
	 */
	libcdata_array_t *directory_entries_array;
};

int libfsrefs_directory_object_initialize(
     libfsrefs_directory_object_t **directory_object,
     libfsrefs_io_handle_t *io_handle,
     libfsrefs_objects_tree_t *objects_tree,
     uint64_t object_identifier,
     libcerror_error_t **error );

int libfsrefs_directory_object_free(
     libfsrefs_directory_object_t **directory_object,
     libcerror_error_t **error );

int libfsrefs_directory_object_check_if_node_block_first_read(
     libfsrefs_directory_object_t *directory_object,
     libfsrefs_block_tree_t *node_block_tree,
     uint64_t block_number,
     off64_t block_offset,
     libcerror_error_t **error );

int libfsrefs_directory_object_read(
     libfsrefs_directory_object_t *directory_object,
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libfsrefs_directory_object_read_node(
     libfsrefs_directory_object_t *directory_object,
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsrefs_ministore_node_t *node,
     libcerror_error_t **error );

int libfsrefs_directory_object_get_number_of_directory_entries(
     libfsrefs_directory_object_t *directory_object,
     int *number_of_directory_entries,
     libcerror_error_t **error );

int libfsrefs_directory_object_get_directory_entry_by_index(
     libfsrefs_directory_object_t *directory_object,
     int directory_entry_index,
     libfsrefs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSREFS_DIRECTORY_OBJECT_H ) */

