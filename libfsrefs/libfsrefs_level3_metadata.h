/*
 * Level 3 metadata functions
 *
 * Copyright (C) 2012-2015, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBFSREFS_LEVEL3_METADATA_H )
#define _LIBFSREFS_LEVEL3_METADATA_H

#include <common.h>
#include <types.h>

#include "libfsrefs_io_handle.h"
#include "libfsrefs_libbfio.h"
#include "libfsrefs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsrefs_level3_metadata libfsrefs_level3_metadata_t;

struct libfsrefs_level3_metadata
{
	/* Dummy
	 */
	int dummy;
};

int libfsrefs_level3_metadata_initialize(
     libfsrefs_level3_metadata_t **level3_metadata,
     libcerror_error_t **error );

int libfsrefs_level3_metadata_free(
     libfsrefs_level3_metadata_t **level3_metadata,
     libcerror_error_t **error );

int libfsrefs_level3_metadata_read(
     libfsrefs_level3_metadata_t *level3_metadata,
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

