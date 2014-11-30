/*
 * Volume functions
 *
 * Copyright (C) 2012-2014, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBFSREFS_INTERNAL_VOLUME_H )
#define _LIBFSREFS_INTERNAL_VOLUME_H

#include <common.h>
#include <types.h>

#include "libfsrefs_extern.h"
#include "libfsrefs_io_handle.h"
#include "libfsrefs_libbfio.h"
#include "libfsrefs_libcerror.h"
#include "libfsrefs_libfcache.h"
#include "libfsrefs_libfdata.h"
#include "libfsrefs_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsrefs_internal_volume libfsrefs_internal_volume_t;

struct libfsrefs_internal_volume
{
	/* The IO handle
	 */
	libfsrefs_io_handle_t *io_handle;

	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* Value to indicate if the file IO handle was created inside the library
	 */
	uint8_t file_io_handle_created_in_library;

	/* Value to indicate if the file IO handle was opened inside the library
	 */
	uint8_t file_io_handle_opened_in_library;
};

LIBFSREFS_EXTERN \
int libfsrefs_volume_initialize(
     libfsrefs_volume_t **volume,
     libcerror_error_t **error );

LIBFSREFS_EXTERN \
int libfsrefs_volume_free(
     libfsrefs_volume_t **volume,
     libcerror_error_t **error );

LIBFSREFS_EXTERN \
int libfsrefs_volume_signal_abort(
     libfsrefs_volume_t *volume,
     libcerror_error_t **error );

LIBFSREFS_EXTERN \
int libfsrefs_volume_open(
     libfsrefs_volume_t *volume,
     const char *filename,
     int access_flags,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

LIBFSREFS_EXTERN \
int libfsrefs_volume_open_wide(
     libfsrefs_volume_t *volume,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

LIBFSREFS_EXTERN \
int libfsrefs_volume_open_file_io_handle(
     libfsrefs_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error );

LIBFSREFS_EXTERN \
int libfsrefs_volume_close(
     libfsrefs_volume_t *volume,
     libcerror_error_t **error );

int libfsrefs_volume_open_read(
     libfsrefs_internal_volume_t *internal_volume,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

LIBFSREFS_EXTERN \
int libfsrefs_volume_get_utf8_name_size(
     libfsrefs_volume_t *volume,
     size_t *utf8_name_size,
     libcerror_error_t **error );

LIBFSREFS_EXTERN \
int libfsrefs_volume_get_utf8_name(
     libfsrefs_volume_t *volume,
     uint8_t *utf8_name,
     size_t utf8_name_size,
     libcerror_error_t **error );

LIBFSREFS_EXTERN \
int libfsrefs_volume_get_utf16_name_size(
     libfsrefs_volume_t *volume,
     size_t *utf16_name_size,
     libcerror_error_t **error );

LIBFSREFS_EXTERN \
int libfsrefs_volume_get_utf16_name(
     libfsrefs_volume_t *volume,
     uint16_t *utf16_name,
     size_t utf16_name_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

