/*
 * Directory entry functions
 *
 * Copyright (C) 2010-2023, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBFSREFS_DIRECTORY_ENTRY_H )
#define _LIBFSREFS_DIRECTORY_ENTRY_H

#include <common.h>
#include <types.h>

#include "libfsrefs_io_handle.h"
#include "libfsrefs_libcerror.h"
#include "libfsrefs_node_record.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsrefs_directory_entry libfsrefs_directory_entry_t;

struct libfsrefs_directory_entry
{
	/* Object identifier
	 */
	uint64_t object_identifier;

	/* The (file) entry type
	 */
	uint16_t entry_type;

	/* The name data
	 */
	uint8_t *name_data;

	/* The name data size
	 */
	size_t name_data_size;

	/* The creation time
	 */
	uint64_t creation_time;

	/* The modification time
	 */
	uint64_t modification_time;

	/* The access time
	 */
	uint64_t access_time;

	/* The entry modification time
	 */
	uint64_t entry_modification_time;

	/* The file attribute flags
	 */
	uint32_t file_attribute_flags;
};

int libfsrefs_directory_entry_initialize(
     libfsrefs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfsrefs_directory_entry_free(
     libfsrefs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfsrefs_directory_entry_read_node_record(
     libfsrefs_directory_entry_t *directory_entry,
     libfsrefs_io_handle_t *io_handle,
     libfsrefs_node_record_t *node_record,
     libcerror_error_t **error );

int libfsrefs_directory_entry_read_node_record_file_values(
     libfsrefs_directory_entry_t *directory_entry,
     libfsrefs_io_handle_t *io_handle,
     libfsrefs_node_record_t *node_record,
     libcerror_error_t **error );

int libfsrefs_directory_entry_get_object_identifier(
     libfsrefs_directory_entry_t *directory_entry,
     uint64_t *object_identifier,
     libcerror_error_t **error );

int libfsrefs_directory_entry_get_utf8_name_size(
     libfsrefs_directory_entry_t *directory_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libfsrefs_directory_entry_get_utf8_name(
     libfsrefs_directory_entry_t *directory_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libfsrefs_directory_entry_get_utf16_name_size(
     libfsrefs_directory_entry_t *directory_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libfsrefs_directory_entry_get_utf16_name(
     libfsrefs_directory_entry_t *directory_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libfsrefs_directory_entry_get_creation_time(
     libfsrefs_directory_entry_t *directory_entry,
     uint64_t *filetime,
     libcerror_error_t **error );

int libfsrefs_directory_entry_get_modification_time(
     libfsrefs_directory_entry_t *directory_entry,
     uint64_t *filetime,
     libcerror_error_t **error );

int libfsrefs_directory_entry_get_access_time(
     libfsrefs_directory_entry_t *directory_entry,
     uint64_t *filetime,
     libcerror_error_t **error );

int libfsrefs_directory_entry_get_entry_modification_time(
     libfsrefs_directory_entry_t *directory_entry,
     uint64_t *filetime,
     libcerror_error_t **error );

int libfsrefs_directory_entry_get_file_attribute_flags(
     libfsrefs_directory_entry_t *directory_entry,
     uint32_t *file_attribute_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSREFS_DIRECTORY_ENTRY_H ) */

