/*
 * Superblock (or level 0 metadata) functions
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

#if !defined( _LIBFSREFS_SUPERBLOCK_H )
#define _LIBFSREFS_SUPERBLOCK_H

#include <common.h>
#include <types.h>

#include "libfsrefs_io_handle.h"
#include "libfsrefs_libbfio.h"
#include "libfsrefs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsrefs_superblock libfsrefs_superblock_t;

struct libfsrefs_superblock
{
	/* The volume identifier
	 */
	uint8_t volume_identifier[ 16 ];

	/* The primary checkpoint block number
	 */
	uint64_t primary_checkpoint_block_number;

	/* The secondary checkpoint block number
	 */
	uint64_t secondary_checkpoint_block_number;
};

int libfsrefs_superblock_initialize(
     libfsrefs_superblock_t **superblock,
     libcerror_error_t **error );

int libfsrefs_superblock_free(
     libfsrefs_superblock_t **superblock,
     libcerror_error_t **error );

int libfsrefs_superblock_read_data(
     libfsrefs_superblock_t *superblock,
     libfsrefs_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsrefs_superblock_read_file_io_handle(
     libfsrefs_superblock_t *superblock,
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSREFS_SUPERBLOCK_H ) */

