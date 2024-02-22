/*
 * Directory entry functions
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

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libfsrefs_attribute_values.h"
#include "libfsrefs_debug.h"
#include "libfsrefs_directory_entry.h"
#include "libfsrefs_libcdata.h"
#include "libfsrefs_libcerror.h"
#include "libfsrefs_libcnotify.h"
#include "libfsrefs_libfdatetime.h"
#include "libfsrefs_libuna.h"
#include "libfsrefs_ministore_node.h"
#include "libfsrefs_node_record.h"

#include "fsrefs_directory_object.h"

/* Creates a directory entry
 * Make sure the value directory_entry is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_directory_entry_initialize(
     libfsrefs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_directory_entry_initialize";

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( *directory_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid directory entry value already set.",
		 function );

		return( -1 );
	}
	*directory_entry = memory_allocate_structure(
	                    libfsrefs_directory_entry_t );

	if( *directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create directory entry.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *directory_entry,
	     0,
	     sizeof( libfsrefs_directory_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear directory entry.",
		 function );

		memory_free(
		 *directory_entry );

		*directory_entry = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *directory_entry )->attributes_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create attributes array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *directory_entry != NULL )
	{
		memory_free(
		 *directory_entry );

		*directory_entry = NULL;
	}
	return( -1 );
}

/* Frees a directory entry
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_directory_entry_free(
     libfsrefs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_directory_entry_free";
	int result            = 1;

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( *directory_entry != NULL )
	{
		if( libcdata_array_free(
		     &( ( *directory_entry )->attributes_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfsrefs_attribute_values_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free attributes array.",
			 function );

			result = -1;
		}
		if( ( *directory_entry )->name_data != NULL )
		{
			memory_free(
			 ( *directory_entry )->name_data );
		}
		memory_free(
		 *directory_entry );

		*directory_entry = NULL;
	}
	return( result );
}

/* Reads the directory entry directory values
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_directory_entry_read_directory_values(
     libfsrefs_directory_entry_t *directory_entry,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_directory_entry_read_directory_values";

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit  = 0;
	uint32_t value_32bit  = 0;
#endif

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size != sizeof( fsrefs_directory_values_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: directory values data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint64_little_endian(
	 ( (fsrefs_directory_values_t *) data )->object_identifier,
	 directory_entry->object_identifier );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsrefs_directory_values_t *) data )->creation_time,
	 directory_entry->creation_time );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsrefs_directory_values_t *) data )->modification_time,
	 directory_entry->modification_time );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsrefs_directory_values_t *) data )->entry_modification_time,
	 directory_entry->entry_modification_time );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsrefs_directory_values_t *) data )->access_time,
	 directory_entry->access_time );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsrefs_directory_values_t *) data )->file_attribute_flags,
	 directory_entry->file_attribute_flags );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: object identifier\t\t: 0x%08" PRIx64 "\n",
		 function,
		 directory_entry->object_identifier );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_directory_values_t *) data )->unknown1,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		if( libfsrefs_debug_print_filetime_value(
		     function,
		     "creation time\t\t\t",
		     ( (fsrefs_directory_values_t *) data )->creation_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print FILETIME value.",
			 function );

			return( -1 );
		}
		if( libfsrefs_debug_print_filetime_value(
		     function,
		     "modification time\t\t",
		     ( (fsrefs_directory_values_t *) data )->modification_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print FILETIME value.",
			 function );

			return( -1 );
		}
		if( libfsrefs_debug_print_filetime_value(
		     function,
		     "entry modification time\t",
		     ( (fsrefs_directory_values_t *) data )->entry_modification_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print FILETIME value.",
			 function );

			return( -1 );
		}
		if( libfsrefs_debug_print_filetime_value(
		     function,
		     "access time\t\t\t",
		     ( (fsrefs_directory_values_t *) data )->access_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print FILETIME value.",
			 function );

			return( -1 );
		}
		libcnotify_printf(
		 "%s: unknown2:\n",
		 function );
		libcnotify_print_data(
		 ( (fsrefs_directory_values_t *) data )->unknown2,
		 16,
		 0 );

		libcnotify_printf(
		 "%s: file attribute flags\t\t: 0x%08" PRIx32 "\n",
		 function,
		 directory_entry->file_attribute_flags );
		libfsrefs_debug_print_file_attribute_flags(
		 directory_entry->file_attribute_flags );
		libcnotify_printf(
		 "\n" );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsrefs_directory_values_t *) data )->unknown3,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown3\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

/* Reads the directory entry file values
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_directory_entry_read_file_values(
     libfsrefs_directory_entry_t *directory_entry,
     libfsrefs_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfsrefs_attribute_values_t *attribute_values = NULL;
	libfsrefs_ministore_node_t *node               = NULL;
	libfsrefs_node_record_t *node_record           = NULL;
	static char *function                          = "libfsrefs_directory_entry_read_file_values";
	int entry_index                                = 0;
	int number_of_records                          = 0;
	int record_index                               = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit                           = 0;
	uint32_t value_32bit                           = 0;
#endif

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( libfsrefs_ministore_node_initialize(
	     &node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file values ministore node.",
		 function );

		goto on_error;
	}
	if( libfsrefs_ministore_node_read_data(
	     node,
	     io_handle,
	     data,
	     data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file values ministore node.",
		 function );

		goto on_error;
	}
	if( ( node->node_type_flags & 0x03 ) != 0x02 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid file values ministore node - unsupported node type flags.",
		 function );

		goto on_error;
	}
	if( node->header_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file values ministore node - missing header data.",
		 function );

		return( -1 );
	}
	if( node->header_data_size != sizeof( fsrefs_file_values_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid file values ministore node - header data size value out of bounds.",
		 function );

		goto on_error;
	}
	byte_stream_copy_to_uint64_little_endian(
	 ( (fsrefs_file_values_t *) node->header_data )->creation_time,
	 directory_entry->creation_time );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsrefs_file_values_t *) node->header_data )->modification_time,
	 directory_entry->modification_time );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsrefs_file_values_t *) node->header_data )->entry_modification_time,
	 directory_entry->entry_modification_time );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsrefs_file_values_t *) node->header_data )->access_time,
	 directory_entry->access_time );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsrefs_file_values_t *) node->header_data )->file_attribute_flags,
	 directory_entry->file_attribute_flags );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libfsrefs_debug_print_filetime_value(
		     function,
		     "creation time\t\t",
		     ( (fsrefs_file_values_t *) node->header_data )->creation_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print FILETIME value.",
			 function );

			goto on_error;
		}
		if( libfsrefs_debug_print_filetime_value(
		     function,
		     "modification time\t\t",
		     ( (fsrefs_file_values_t *) node->header_data )->modification_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print FILETIME value.",
			 function );

			goto on_error;
		}
		if( libfsrefs_debug_print_filetime_value(
		     function,
		     "entry modification time\t",
		     ( (fsrefs_file_values_t *) node->header_data )->entry_modification_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print FILETIME value.",
			 function );

			goto on_error;
		}
		if( libfsrefs_debug_print_filetime_value(
		     function,
		     "access time\t\t\t",
		     ( (fsrefs_file_values_t *) node->header_data )->access_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print FILETIME value.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: file attribute flags\t: 0x%08" PRIx32 "\n",
		 function,
		 directory_entry->file_attribute_flags );
		libfsrefs_debug_print_file_attribute_flags(
		 directory_entry->file_attribute_flags );
		libcnotify_printf(
		 "\n" );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsrefs_file_values_t *) node->header_data )->unknown1,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_file_values_t *) node->header_data )->identifier_lower,
		 value_64bit );
		libcnotify_printf(
		 "%s: identifier (lower 64-bits)\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_file_values_t *) node->header_data )->identifier_upper,
		 value_64bit );
		libcnotify_printf(
		 "%s: identifier (upper 64-bits)\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsrefs_file_values_t *) node->header_data )->unknown4,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown4\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsrefs_file_values_t *) node->header_data )->unknown5,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown5\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_file_values_t *) node->header_data )->data_size,
		 value_64bit );
		libcnotify_printf(
		 "%s: data size\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_file_values_t *) node->header_data )->allocated_data_size,
		 value_64bit );
		libcnotify_printf(
		 "%s: allocated data size\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "%s: unknown6:\n",
		 function );
		libcnotify_print_data(
		 ( (fsrefs_file_values_t *) node->header_data )->unknown6,
		 32,
		 0 );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_file_values_t *) node->header_data )->unknown7,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown7\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_file_values_t *) node->header_data )->unknown8,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown8\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( libfsrefs_ministore_node_get_number_of_records(
	     node,
	     &number_of_records,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of records.",
		 function );

		goto on_error;
	}
	if( number_of_records == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported file values ministore node - missing attribute records.",
		 function );

		goto on_error;
	}
	for( record_index = 0;
	     record_index < number_of_records;
	     record_index++ )
	{
		if( libfsrefs_ministore_node_get_record_by_index(
		     node,
		     record_index,
		     &node_record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute: %d record.",
			 function,
			 record_index );

			goto on_error;
		}
		if( libfsrefs_attribute_values_initialize(
		     &attribute_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create attribute values: %d.",
			 function,
			 record_index );

			goto on_error;
		}
		if( libfsrefs_attribute_values_read_node_record(
		     attribute_values,
		     io_handle,
		     node_record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read attribute values from record: %d.",
			 function,
			 record_index );

			goto on_error;
		}
		if( libcdata_array_append_entry(
		     directory_entry->attributes_array,
		     &entry_index,
		     (intptr_t *) attribute_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append attribute values: %d to array.",
			 function,
			 record_index );

			goto on_error;
		}
		attribute_values = NULL;
	}
	if( libfsrefs_ministore_node_free(
	     &node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file values ministore node.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( attribute_values != NULL )
	{
		libfsrefs_attribute_values_free(
		 &attribute_values,
		 NULL );
	}
	if( node != NULL )
	{
		libfsrefs_ministore_node_free(
		 &node,
		 NULL );
	}
	libcdata_array_empty(
	 directory_entry->attributes_array,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfsrefs_attribute_values_free,
	 NULL );

	return( -1 );
}

/* Reads the directory entry from a ministore node record
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_directory_entry_read_node_record(
     libfsrefs_directory_entry_t *directory_entry,
     libfsrefs_io_handle_t *io_handle,
     libfsrefs_node_record_t *node_record,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_directory_entry_read_node_record";
	size_t name_data_size = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint16_t value_16bit  = 0;
#endif

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( directory_entry->name_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid directory entry - namd data value already set.",
		 function );

		return( -1 );
	}
	if( node_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node record.",
		 function );

		return( -1 );
	}
	if( node_record->key_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid node record - missing key data.",
		 function );

		return( -1 );
	}
	if( node_record->key_data_size < 6 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid node record - key data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: key data:\n",
		 function );
		libcnotify_print_data(
		 node_record->key_data,
		 node_record->key_data_size,
		 0 );
	}
#endif
	byte_stream_copy_to_uint16_little_endian(
	 &( node_record->key_data[ 2 ] ),
	 directory_entry->entry_type );

	name_data_size = node_record->key_data_size - 4;

	directory_entry->name_data = (uint8_t *) memory_allocate(
	                                          sizeof( uint8_t ) * name_data_size );

	if( directory_entry->name_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create name data.",
		 function );

		goto on_error;
	}
	directory_entry->name_data_size = name_data_size;

	if( memory_copy(
	     directory_entry->name_data,
	     &( node_record->key_data[ 4 ] ),
	     directory_entry->name_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy name data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint16_little_endian(
		 &( node_record->key_data[ 0 ] ),
		 value_16bit );
		libcnotify_printf(
		 "%s: record type\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: entry type\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 directory_entry->entry_type );

		if( libfsrefs_debug_print_utf16_string_value(
		     function,
		     "name\t\t\t",
		     directory_entry->name_data,
		     directory_entry->name_data_size,
		     LIBUNA_ENDIAN_LITTLE | LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print UTF-16 string value.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( directory_entry->entry_type == 1 )
	{
		if( libfsrefs_directory_entry_read_file_values(
		     directory_entry,
		     io_handle,
		     node_record->value_data,
		     node_record->value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read file values.",
			 function );

			goto on_error;
		}
	}
	else if( directory_entry->entry_type == 2 )
	{
		if( libfsrefs_directory_entry_read_directory_values(
		     directory_entry,
		     node_record->value_data,
		     node_record->value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read directory values.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( directory_entry->name_data != NULL )
	{
		memory_free(
		 directory_entry->name_data );

		directory_entry->name_data = NULL;
	}
	directory_entry->name_data_size = 0;

	libcdata_array_empty(
	 directory_entry->attributes_array,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfsrefs_attribute_values_free,
	 NULL );

	return( -1 );
}

/* Retrieves the object identifier
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_directory_entry_get_object_identifier(
     libfsrefs_directory_entry_t *directory_entry,
     uint64_t *object_identifier,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_directory_entry_get_object_identifier";

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( object_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid object identifier.",
		 function );

		return( -1 );
	}
	*object_identifier = directory_entry->object_identifier;

	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded name
 * This function uses UTF-8 RFC 2279 (or 6-byte UTF-8) to support characters outside Unicode
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_directory_entry_get_utf8_name_size(
     libfsrefs_directory_entry_t *directory_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_directory_entry_get_utf8_name_size";

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_size_from_utf16_stream(
	     directory_entry->name_data,
	     directory_entry->name_data_size,
	     LIBUNA_ENDIAN_LITTLE | LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-8 name.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded name
 * This function uses UTF-8 RFC 2279 (or 6-byte UTF-8) to support characters outside Unicode
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_directory_entry_get_utf8_name(
     libfsrefs_directory_entry_t *directory_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_directory_entry_get_utf8_name";

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_copy_from_utf16_stream(
	     utf8_string,
	     utf8_string_size,
	     directory_entry->name_data,
	     directory_entry->name_data_size,
	     LIBUNA_ENDIAN_LITTLE | LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 name.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded name
 * This function uses UCS-2 (with surrogates) to support characters outside Unicode
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_directory_entry_get_utf16_name_size(
     libfsrefs_directory_entry_t *directory_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_directory_entry_get_utf16_name_size";

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_string_size_from_utf16_stream(
	     directory_entry->name_data,
	     directory_entry->name_data_size,
	     LIBUNA_ENDIAN_LITTLE | LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-16 name.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded name
 * This function uses UCS-2 (with surrogates) to support characters outside Unicode
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_directory_entry_get_utf16_name(
     libfsrefs_directory_entry_t *directory_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_directory_entry_get_utf16_name";

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_string_copy_from_utf16_stream(
	     utf16_string,
	     utf16_string_size,
	     directory_entry->name_data,
	     directory_entry->name_data_size,
	     LIBUNA_ENDIAN_LITTLE | LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 name.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the creation date and time
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_directory_entry_get_creation_time(
     libfsrefs_directory_entry_t *directory_entry,
     uint64_t *filetime,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_directory_entry_get_creation_time";

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( filetime == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid FILETIME.",
		 function );

		return( -1 );
	}
	*filetime = directory_entry->creation_time;

	return( 1 );
}

/* Retrieves the (file) modification (last written) date and time
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_directory_entry_get_modification_time(
     libfsrefs_directory_entry_t *directory_entry,
     uint64_t *filetime,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_directory_entry_get_modification_time";

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( filetime == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid FILETIME.",
		 function );

		return( -1 );
	}
	*filetime = directory_entry->modification_time;

	return( 1 );
}

/* Retrieves the access date and time
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_directory_entry_get_access_time(
     libfsrefs_directory_entry_t *directory_entry,
     uint64_t *filetime,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_directory_entry_get_access_time";

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( filetime == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid FILETIME.",
		 function );

		return( -1 );
	}
	*filetime = directory_entry->access_time;

	return( 1 );
}

/* Retrieves the (file system entry) modification date and time
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_directory_entry_get_entry_modification_time(
     libfsrefs_directory_entry_t *directory_entry,
     uint64_t *filetime,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_directory_entry_get_entry_modification_time";

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( filetime == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid FILETIME.",
		 function );

		return( -1 );
	}
	*filetime = directory_entry->entry_modification_time;

	return( 1 );
}

/* Retrieves the file attribute flags
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_directory_entry_get_file_attribute_flags(
     libfsrefs_directory_entry_t *directory_entry,
     uint32_t *file_attribute_flags,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_directory_entry_get_file_attribute_flags";

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( file_attribute_flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file attribute flags.",
		 function );

		return( -1 );
	}
	*file_attribute_flags = directory_entry->file_attribute_flags;

	return( 1 );
}

