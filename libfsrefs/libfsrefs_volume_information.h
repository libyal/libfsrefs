/*
 * Volume information attribute ($VOLUME_INFORMATION) functions
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

#if !defined( _LIBFSREFS_VOLUME_INFORMATION_H )
#define _LIBFSREFS_VOLUME_INFORMATION_H

#include <common.h>
#include <types.h>

#include "libfsrefs_libcerror.h"
#include "libfsrefs_metadata_value.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsrefs_volume_information libfsrefs_volume_information_t;

struct libfsrefs_volume_information
{
	/* The major version (number)
	 */
	uint8_t major_version;

	/* The minor version (number)
	 */
	uint8_t minor_version;
};

int libfsrefs_volume_information_initialize(
     libfsrefs_volume_information_t **volume_information,
     libcerror_error_t **error );

int libfsrefs_volume_information_free(
     libfsrefs_volume_information_t **volume_information,
     libcerror_error_t **error );

int libfsrefs_volume_information_read(
     libfsrefs_volume_information_t *volume_information,
     libfsrefs_metadata_value_t *metadata_value,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSREFS_VOLUME_INFORMATION_H ) */

