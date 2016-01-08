/*
 * Block descriptors functions
 *
 * Copyright (C) 2012-2016, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBFSREFS_BLOCK_DESCRIPTORS_H )
#define _LIBFSREFS_BLOCK_DESCRIPTORS_H

#include <common.h>
#include <types.h>

#include "libfsrefs_block_descriptor.h"
#include "libfsrefs_io_handle.h"
#include "libfsrefs_libbfio.h"
#include "libfsrefs_libcdata.h"
#include "libfsrefs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsrefs_block_descriptors libfsrefs_block_descriptors_t;

struct libfsrefs_block_descriptors
{
	/* The block descriptors array
	 */
	libcdata_array_t *block_descriptors_array;
};

int libfsrefs_block_descriptors_initialize(
     libfsrefs_block_descriptors_t **block_descriptors,
     libcerror_error_t **error );

int libfsrefs_block_descriptors_free(
     libfsrefs_block_descriptors_t **block_descriptors,
     libcerror_error_t **error );

int libfsrefs_block_descriptors_read(
     libfsrefs_block_descriptors_t *block_descriptors,
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     int level,
     libcerror_error_t **error );

int libfsrefs_block_descriptors_get_number_of_block_descriptors(
     libfsrefs_block_descriptors_t *block_descriptors,
     int *number_of_block_descriptors,
     libcerror_error_t **error );

int libfsrefs_block_descriptors_get_block_descriptor_by_index(
     libfsrefs_block_descriptors_t *block_descriptors,
     int block_descriptor_index,
     libfsrefs_block_descriptor_t **block_descriptor,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

