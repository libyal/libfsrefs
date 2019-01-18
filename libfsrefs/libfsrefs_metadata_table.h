/*
 * Metadata table functions
 *
 * Copyright (C) 2012-2019, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _LIBFSREFS_METADATA_TABLE_H )
#define _LIBFSREFS_METADATA_TABLE_H

#include <common.h>
#include <types.h>

#include "libfsrefs_libcdata.h"
#include "libfsrefs_libcerror.h"
#include "libfsrefs_metadata_value.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsrefs_metadata_table libfsrefs_metadata_table_t;

struct libfsrefs_metadata_table
{
	/* The (table) size
	 */
	size_t size;

	/* The values array
	 */
	libcdata_array_t *values_array;
};

int libfsrefs_metadata_table_initialize(
     libfsrefs_metadata_table_t **metadata_table,
     libcerror_error_t **error );

int libfsrefs_metadata_table_free(
     libfsrefs_metadata_table_t **metadata_table,
     libcerror_error_t **error );

int libfsrefs_metadata_table_read(
     libfsrefs_metadata_table_t *metadata_table,
     const uint8_t *metadata_block_data,
     size_t metadata_block_data_size,
     size_t metadata_block_data_offset,
     int level,
     libcerror_error_t **error );

int libfsrefs_metadata_table_get_number_of_values(
     libfsrefs_metadata_table_t *metadata_table,
     int *number_of_values,
     libcerror_error_t **error );

int libfsrefs_metadata_table_get_value_by_index(
     libfsrefs_metadata_table_t *metadata_table,
     int value_index,
     libfsrefs_metadata_value_t **metadata_value,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSREFS_METADATA_TABLE_H ) */

