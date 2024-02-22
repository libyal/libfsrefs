/*
 * Attribute values functions
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

#if !defined( _LIBFSREFS_ATTRIBUTE_VALUES_H )
#define _LIBFSREFS_ATTRIBUTE_VALUES_H

#include <common.h>
#include <types.h>

#include "libfsrefs_io_handle.h"
#include "libfsrefs_libcdata.h"
#include "libfsrefs_libcerror.h"
#include "libfsrefs_node_record.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsrefs_attribute_values libfsrefs_attribute_values_t;

struct libfsrefs_attribute_values
{
	/* The attribute type
	 */
	uint32_t attribute_type;

	/* The name data
	 */
	uint8_t *name_data;

	/* The name data size
	 */
	size_t name_data_size;

	/* The data runs array
	 */
	libcdata_array_t *data_runs_array;
};

int libfsrefs_attribute_values_initialize(
     libfsrefs_attribute_values_t **attribute_values,
     libcerror_error_t **error );

int libfsrefs_attribute_values_free(
     libfsrefs_attribute_values_t **attribute_values,
     libcerror_error_t **error );

int libfsrefs_attribute_values_read_non_resident(
     libfsrefs_attribute_values_t *attribute_values,
     libfsrefs_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsrefs_attribute_values_read_resident(
     libfsrefs_attribute_values_t *attribute_values,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsrefs_attribute_values_read_node_record(
     libfsrefs_attribute_values_t *attribute_values,
     libfsrefs_io_handle_t *io_handle,
     libfsrefs_node_record_t *node_record,
     libcerror_error_t **error );

int libfsrefs_attribute_values_get_utf8_name_size(
     libfsrefs_attribute_values_t *attribute_values,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libfsrefs_attribute_values_get_utf8_name(
     libfsrefs_attribute_values_t *attribute_values,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libfsrefs_attribute_values_get_utf16_name_size(
     libfsrefs_attribute_values_t *attribute_values,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libfsrefs_attribute_values_get_utf16_name(
     libfsrefs_attribute_values_t *attribute_values,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSREFS_ATTRIBUTE_VALUES_H ) */

