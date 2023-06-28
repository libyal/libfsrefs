/*
 * File entry functions
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

#if !defined( _LIBFSREFS_FILE_ENTRY_H )
#define _LIBFSREFS_FILE_ENTRY_H

#include <common.h>
#include <types.h>

#include "libfsrefs_directory_object.h"
#include "libfsrefs_extern.h"
#include "libfsrefs_io_handle.h"
#include "libfsrefs_libbfio.h"
#include "libfsrefs_libcerror.h"
#include "libfsrefs_libcthreads.h"
#include "libfsrefs_objects_tree.h"
#include "libfsrefs_node_record.h"
#include "libfsrefs_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsrefs_internal_file_entry libfsrefs_internal_file_entry_t;

struct libfsrefs_internal_file_entry
{
	/* The IO handle
	 */
	libfsrefs_io_handle_t *io_handle;

	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* The objects tree
	 */
	libfsrefs_objects_tree_t *objects_tree;

	/* The directory object
	 */
	libfsrefs_directory_object_t *directory_object;

	/* The (file) entry type
	 */
	uint16_t entry_type;

	/* The name data
	 */
	uint8_t *name_data;

	/* The name data size
	 */
	size_t name_data_size;

#if defined( HAVE_LIBFSREFS_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

int libfsrefs_file_entry_initialize(
     libfsrefs_file_entry_t **file_entry,
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsrefs_objects_tree_t *objects_tree,
     libfsrefs_node_record_t *node_record,
     libcerror_error_t **error );

LIBFSREFS_EXTERN \
int libfsrefs_file_entry_free(
     libfsrefs_file_entry_t **file_entry,
     libcerror_error_t **error );

int libfsrefs_internal_file_entry_get_directory_object(
     libfsrefs_internal_file_entry_t *internal_file_entry,
     libcerror_error_t **error );

LIBFSREFS_EXTERN \
int libfsrefs_file_entry_get_utf8_name_size(
     libfsrefs_file_entry_t *file_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBFSREFS_EXTERN \
int libfsrefs_file_entry_get_utf8_name(
     libfsrefs_file_entry_t *file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBFSREFS_EXTERN \
int libfsrefs_file_entry_get_utf16_name_size(
     libfsrefs_file_entry_t *file_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBFSREFS_EXTERN \
int libfsrefs_file_entry_get_utf16_name(
     libfsrefs_file_entry_t *file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBFSREFS_EXTERN \
int libfsrefs_file_entry_get_number_of_sub_file_entries(
     libfsrefs_file_entry_t *file_entry,
     int *number_of_sub_entries,
     libcerror_error_t **error );

LIBFSREFS_EXTERN \
int libfsrefs_file_entry_get_sub_file_entry_by_index(
     libfsrefs_file_entry_t *file_entry,
     int sub_file_entry_index,
     libfsrefs_file_entry_t **sub_file_entry,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSREFS_FILE_ENTRY_H ) */

