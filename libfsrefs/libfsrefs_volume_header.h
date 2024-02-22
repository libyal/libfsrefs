/*
 * The REFS volume header functions
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

#if !defined( _LIBFSREFS_VOLUME_HEADER_H )
#define _LIBFSREFS_VOLUME_HEADER_H

#include <common.h>
#include <types.h>

#include "libfsrefs_libbfio.h"
#include "libfsrefs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsrefs_volume_header libfsrefs_volume_header_t;

struct libfsrefs_volume_header
{
	/* The number of bytes per sector
	 */
	uint32_t bytes_per_sector;

	/* The volume size
	 */
	size64_t volume_size;

	/* The major format version
	 */
	uint8_t major_format_version;

	/* The minor format version
	 */
	uint8_t minor_format_version;

	/* The volume serial number
	 */
	uint64_t volume_serial_number;

	/* The cluster block size
	 */
	size32_t cluster_block_size;

	/* The metadata block size
	 */
	size32_t metadata_block_size;

	/* The container size
	 */
	size64_t container_size;
};

int libfsrefs_volume_header_initialize(
     libfsrefs_volume_header_t **volume_header,
     libcerror_error_t **error );

int libfsrefs_volume_header_free(
     libfsrefs_volume_header_t **volume_header,
     libcerror_error_t **error );

int libfsrefs_volume_header_read_data(
     libfsrefs_volume_header_t *volume_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsrefs_volume_header_read_file_io_handle(
     libfsrefs_volume_header_t *volume_header,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

int libfsrefs_volume_header_get_bytes_per_sector(
     libfsrefs_volume_header_t *volume_header,
     uint16_t *bytes_per_sector,
     libcerror_error_t **error );

int libfsrefs_volume_header_get_cluster_block_size(
     libfsrefs_volume_header_t *volume_header,
     size32_t *cluster_block_size,
     libcerror_error_t **error );

int libfsrefs_volume_header_get_volume_size(
     libfsrefs_volume_header_t *volume_header,
     size64_t *volume_size,
     libcerror_error_t **error );

int libfsrefs_volume_header_get_volume_serial_number(
     libfsrefs_volume_header_t *volume_header,
     uint64_t *volume_serial_number,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSREFS_VOLUME_HEADER_H ) */

