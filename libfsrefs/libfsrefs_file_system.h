/*
 * File system functions
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

#if !defined( _LIBFSREFS_FILE_SYSTEM_H )
#define _LIBFSREFS_FILE_SYSTEM_H

#include <common.h>
#include <types.h>

#include "libfsrefs_block_reference.h"
#include "libfsrefs_checkpoint.h"
#include "libfsrefs_io_handle.h"
#include "libfsrefs_libcerror.h"
#include "libfsrefs_ministore_node.h"
#include "libfsrefs_superblock.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsrefs_file_system libfsrefs_file_system_t;

struct libfsrefs_file_system
{
	/* The superblock
	 */
	libfsrefs_superblock_t *superblock;

	/* The (latest) checkpoint
	 */
	libfsrefs_checkpoint_t *checkpoint;

	/* The containers (ministore) root node
	 */
	libfsrefs_ministore_node_t *containers_root_node;
};

int libfsrefs_file_system_initialize(
     libfsrefs_file_system_t **file_system,
     libcerror_error_t **error );

int libfsrefs_file_system_free(
     libfsrefs_file_system_t **file_system,
     libcerror_error_t **error );

int libfsrefs_file_system_read_superblock(
     libfsrefs_file_system_t *file_system,
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

int libfsrefs_file_system_read_checkpoints(
     libfsrefs_file_system_t *file_system,
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libfsrefs_file_system_read_container_trees(
     libfsrefs_file_system_t *file_system,
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libfsrefs_file_system_get_block_offsets(
     libfsrefs_file_system_t *file_system,
     libfsrefs_io_handle_t *io_handle,
     libfsrefs_block_reference_t *block_reference,
     libcerror_error_t **error );

int libfsrefs_file_system_get_number_of_ministore_trees(
     libfsrefs_file_system_t *file_system,
     int *number_of_ministore_trees,
     libcerror_error_t **error );

int libfsrefs_file_system_get_ministore_tree(
     libfsrefs_file_system_t *file_system,
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     int ministore_tree_index,
     libfsrefs_ministore_node_t **root_node,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSREFS_FILE_SYSTEM_H ) */

