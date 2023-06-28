/*
 * Volume functions
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

#if !defined( _LIBFSREFS_INTERNAL_VOLUME_H )
#define _LIBFSREFS_INTERNAL_VOLUME_H

#include <common.h>
#include <types.h>

#include "libfsrefs_extern.h"
#include "libfsrefs_file_system.h"
#include "libfsrefs_io_handle.h"
#include "libfsrefs_libbfio.h"
#include "libfsrefs_libcerror.h"
#include "libfsrefs_libcthreads.h"
#include "libfsrefs_libfcache.h"
#include "libfsrefs_libfdata.h"
#include "libfsrefs_ministore_node.h"
#include "libfsrefs_node_record.h"
#include "libfsrefs_objects_tree.h"
#include "libfsrefs_types.h"
#include "libfsrefs_volume_header.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsrefs_internal_volume libfsrefs_internal_volume_t;

struct libfsrefs_internal_volume
{
	/* The volume header
	 */
	libfsrefs_volume_header_t *volume_header;

	/* The file system
	 */
	libfsrefs_file_system_t *file_system;

	/* The objects tree
	 */
	libfsrefs_objects_tree_t *objects_tree;

	/* The volume information object
	 */
	libfsrefs_ministore_node_t *volume_information_object;

	/* The volume name record
	 */
	libfsrefs_node_record_t *volume_name_record;

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

#if defined( HAVE_LIBFSREFS_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
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

int libfsrefs_internal_volume_open_read(
     libfsrefs_internal_volume_t *internal_volume,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libfsrefs_internal_volume_get_volume_name_record(
     libfsrefs_internal_volume_t *internal_volume,
     libcerror_error_t **error );

LIBFSREFS_EXTERN \
int libfsrefs_volume_get_bytes_per_sector(
     libfsrefs_volume_t *volume,
     uint16_t *bytes_per_sector,
     libcerror_error_t **error );

LIBFSREFS_EXTERN \
int libfsrefs_volume_get_cluster_block_size(
     libfsrefs_volume_t *volume,
     size32_t *cluster_block_size,
     libcerror_error_t **error );

LIBFSREFS_EXTERN \
int libfsrefs_volume_get_utf8_name_size(
     libfsrefs_volume_t *volume,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBFSREFS_EXTERN \
int libfsrefs_volume_get_utf8_name(
     libfsrefs_volume_t *volume,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBFSREFS_EXTERN \
int libfsrefs_volume_get_utf16_name_size(
     libfsrefs_volume_t *volume,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBFSREFS_EXTERN \
int libfsrefs_volume_get_utf16_name(
     libfsrefs_volume_t *volume,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBFSREFS_EXTERN \
int libfsrefs_volume_get_version(
     libfsrefs_volume_t *volume,
     uint8_t *major_version,
     uint8_t *minor_version,
     libcerror_error_t **error );

LIBFSREFS_EXTERN \
int libfsrefs_volume_get_serial_number(
     libfsrefs_volume_t *volume,
     uint64_t *serial_number,
     libcerror_error_t **error );

LIBFSREFS_EXTERN \
int libfsrefs_volume_get_root_directory(
     libfsrefs_volume_t *volume,
     libfsrefs_file_entry_t **file_entry,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSREFS_INTERNAL_VOLUME_H ) */

