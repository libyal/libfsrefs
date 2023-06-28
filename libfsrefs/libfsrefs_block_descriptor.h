/*
 * Block descriptor functions
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

#if !defined( _LIBFSREFS_BLOCK_DESCRIPTOR_H )
#define _LIBFSREFS_BLOCK_DESCRIPTOR_H

#include <common.h>
#include <types.h>

#include "libfsrefs_io_handle.h"
#include "libfsrefs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsrefs_block_descriptor libfsrefs_block_descriptor_t;

struct libfsrefs_block_descriptor
{
	/* The first block number
	 */
	uint64_t block_number1;

	/* The second block number
	 */
	uint64_t block_number2;

	/* The third block number
	 */
	uint64_t block_number3;

	/* The fourth block number
	 */
	uint64_t block_number4;

	/* The checksum
	 */
	uint64_t checksum;
};

int libfsrefs_block_descriptor_initialize(
     libfsrefs_block_descriptor_t **block_descriptor,
     libcerror_error_t **error );

int libfsrefs_block_descriptor_free(
     libfsrefs_block_descriptor_t **block_descriptor,
     libcerror_error_t **error );

int libfsrefs_block_descriptor_read_data(
     libfsrefs_block_descriptor_t *block_descriptor,
     libfsrefs_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSREFS_BLOCK_DESCRIPTOR_H ) */

