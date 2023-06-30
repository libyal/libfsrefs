/*
 * Checkpoint (or level 1 metadata) functions
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

#if !defined( _LIBFSREFS_CHECKPOINT_H )
#define _LIBFSREFS_CHECKPOINT_H

#include <common.h>
#include <types.h>

#include "libfsrefs_block_reference.h"
#include "libfsrefs_io_handle.h"
#include "libfsrefs_libbfio.h"
#include "libfsrefs_libcdata.h"
#include "libfsrefs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsrefs_checkpoint libfsrefs_checkpoint_t;

struct libfsrefs_checkpoint
{
	/* The sequence number
	 */
	uint64_t sequence_number;

	/* The (ministore tree) block references array
	 */
	libcdata_array_t *block_references_array;
};

int libfsrefs_checkpoint_initialize(
     libfsrefs_checkpoint_t **checkpoint,
     libcerror_error_t **error );

int libfsrefs_checkpoint_free(
     libfsrefs_checkpoint_t **checkpoint,
     libcerror_error_t **error );

int libfsrefs_checkpoint_read_data(
     libfsrefs_checkpoint_t *checkpoint,
     libfsrefs_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsrefs_checkpoint_read_file_io_handle(
     libfsrefs_checkpoint_t *checkpoint,
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

int libfsrefs_checkpoint_get_number_of_ministore_tree_block_references(
     libfsrefs_checkpoint_t *checkpoint,
     int *number_of_block_references,
     libcerror_error_t **error );

int libfsrefs_checkpoint_get_ministore_tree_block_reference_by_index(
     libfsrefs_checkpoint_t *checkpoint,
     int block_reference_index,
     libfsrefs_block_reference_t **block_reference,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSREFS_CHECKPOINT_H ) */

