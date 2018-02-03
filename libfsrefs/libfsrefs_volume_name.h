/*
 * Volume name attribute ($VOLUME_NAME) functions
 *
 * Copyright (C) 2012-2018, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBFSREFS_VOLUME_NAME_H )
#define _LIBFSREFS_VOLUME_NAME_H

#include <common.h>
#include <types.h>

#include "libfsrefs_libcerror.h"
#include "libfsrefs_metadata_value.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsrefs_volume_name libfsrefs_volume_name_t;

struct libfsrefs_volume_name
{
	/* The name
	 */
	uint8_t *name;

	/* The name size
	 */
	size_t name_size;
};

int libfsrefs_volume_name_initialize(
     libfsrefs_volume_name_t **volume_name,
     libcerror_error_t **error );

int libfsrefs_volume_name_free(
     libfsrefs_volume_name_t **volume_name,
     libcerror_error_t **error );

int libfsrefs_volume_name_read(
     libfsrefs_volume_name_t *volume_name,
     libfsrefs_metadata_value_t *metadata_value,
     libcerror_error_t **error );

int libfsrefs_volume_name_get_utf8_name_size(
     libfsrefs_volume_name_t *volume_name,
     size_t *utf8_name_size,
     libcerror_error_t **error );

int libfsrefs_volume_name_get_utf8_name(
     libfsrefs_volume_name_t *volume_name,
     uint8_t *utf8_name,
     size_t utf8_name_size,
     libcerror_error_t **error );

int libfsrefs_volume_name_get_utf16_name_size(
     libfsrefs_volume_name_t *volume_name,
     size_t *utf16_name_size,
     libcerror_error_t **error );

int libfsrefs_volume_name_get_utf16_name(
     libfsrefs_volume_name_t *volume_name,
     uint16_t *utf16_name,
     size_t utf16_name_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSREFS_VOLUME_NAME_H ) */

