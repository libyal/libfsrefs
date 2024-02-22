/*
 * Block reference functions
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

#if !defined( _LIBFSREFS_BLOCK_REFRENCE_H )
#define _LIBFSREFS_BLOCK_REFRENCE_H

#include <common.h>
#include <types.h>

#include "libfsrefs_io_handle.h"
#include "libfsrefs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsrefs_block_reference libfsrefs_block_reference_t;

struct libfsrefs_block_reference
{
	/* The block numbers
	 */
	uint64_t block_numbers[ 4 ];

	/* The checksum
	 */
	uint64_t checksum;

	/* The block offsets
	 */
	off64_t block_offsets[ 4 ];
};

int libfsrefs_block_reference_initialize(
     libfsrefs_block_reference_t **block_reference,
     libcerror_error_t **error );

int libfsrefs_block_reference_free(
     libfsrefs_block_reference_t **block_reference,
     libcerror_error_t **error );

int libfsrefs_block_reference_read_data(
     libfsrefs_block_reference_t *block_reference,
     libfsrefs_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSREFS_BLOCK_REFRENCE_H ) */

