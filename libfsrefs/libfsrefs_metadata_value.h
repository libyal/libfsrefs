/*
 * Metadata value functions
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

#if !defined( _LIBFSREFS_METADATA_VALUE_H )
#define _LIBFSREFS_METADATA_VALUE_H

#include <common.h>
#include <types.h>

#include "libfsrefs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsrefs_metadata_value libfsrefs_metadata_value_t;

struct libfsrefs_metadata_value
{
	/* The (value) size
	 */
	uint32_t size;

	/* The identifier data
	 */
	const uint8_t *identifier_data;

	/* The identifier data size
	 */
	uint16_t identifier_data_size;

	/* The flags
	 */
	uint16_t flags;

	/* The data
	 */
	const uint8_t *data;

	/* The data size
	 */
	uint16_t data_size;

	/* The data offset
	 */
	uint16_t data_offset;
};

int libfsrefs_metadata_value_initialize(
     libfsrefs_metadata_value_t **metadata_value,
     libcerror_error_t **error );

int libfsrefs_metadata_value_free(
     libfsrefs_metadata_value_t **metadata_value,
     libcerror_error_t **error );

int libfsrefs_metadata_value_read(
     libfsrefs_metadata_value_t *metadata_value,
     const uint8_t *metadata_block_data,
     size_t metadata_block_data_size,
     size_t metadata_block_data_offset,
     int level,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSREFS_METADATA_VALUE_H ) */

