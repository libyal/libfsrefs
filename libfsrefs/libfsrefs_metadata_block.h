/*
 * Metadata block functions
 *
 * Copyright (C) 2012-2020, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBFSREFS_METADATA_BLOCK_H )
#define _LIBFSREFS_METADATA_BLOCK_H

#include <common.h>
#include <types.h>

#include "libfsrefs_io_handle.h"
#include "libfsrefs_libbfio.h"
#include "libfsrefs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsrefs_metadata_block libfsrefs_metadata_block_t;

struct libfsrefs_metadata_block
{
	/* The metadata block data
	 */
	uint8_t *data;

	/* The metadata block data size
	 */
	size_t data_size;

	/* The sequence number
	 */
	uint64_t sequence_number;
};

int libfsrefs_metadata_block_initialize(
     libfsrefs_metadata_block_t **metadata_block,
     libcerror_error_t **error );

int libfsrefs_metadata_block_free(
     libfsrefs_metadata_block_t **metadata_block,
     libcerror_error_t **error );

int libfsrefs_metadata_block_read(
     libfsrefs_metadata_block_t *metadata_block,
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     int level,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSREFS_METADATA_BLOCK_H ) */

