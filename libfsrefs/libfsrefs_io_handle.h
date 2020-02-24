/*
 * Input/Output (IO) handle functions
 *
 * Copyright (C) 2012-2020, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBFSREFS_IO_HANDLE_H )
#define _LIBFSREFS_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include "libfsrefs_libbfio.h"
#include "libfsrefs_libcerror.h"
#include "libfsrefs_libfcache.h"
#include "libfsrefs_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern const char fsrefs_volume_file_system_signature[ 4 ];

typedef struct libfsrefs_io_handle libfsrefs_io_handle_t;

struct libfsrefs_io_handle
{
	/* The block size
	 */
	size_t block_size;

	/* The metadata block size
	 */
	size_t metadata_block_size;

	/* The number of bytes per sector
	 */
	uint32_t bytes_per_sector;

	/* The number of sectors
	 */
	uint64_t number_of_sectors;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int libfsrefs_io_handle_initialize(
     libfsrefs_io_handle_t **io_handle,
     libcerror_error_t **error );

int libfsrefs_io_handle_free(
     libfsrefs_io_handle_t **io_handle,
     libcerror_error_t **error );

int libfsrefs_io_handle_clear(
     libfsrefs_io_handle_t *io_handle,
     libcerror_error_t **error );

int libfsrefs_io_handle_read_volume_header(
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSREFS_IO_HANDLE_H ) */

