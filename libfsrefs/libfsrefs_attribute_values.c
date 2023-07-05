/*
 * Attribute values functions
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

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libfsrefs_attribute_values.h"
#include "libfsrefs_data_run.h"
#include "libfsrefs_debug.h"
#include "libfsrefs_libcdata.h"
#include "libfsrefs_libcerror.h"
#include "libfsrefs_libcnotify.h"
#include "libfsrefs_libfdatetime.h"
#include "libfsrefs_libuna.h"
#include "libfsrefs_ministore_node.h"
#include "libfsrefs_node_record.h"

#include "fsrefs_directory_object.h"

/* Creates a attribute values
 * Make sure the value attribute_values is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_attribute_values_initialize(
     libfsrefs_attribute_values_t **attribute_values,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_attribute_values_initialize";

	if( attribute_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute values.",
		 function );

		return( -1 );
	}
	if( *attribute_values != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid attribute values value already set.",
		 function );

		return( -1 );
	}
	*attribute_values = memory_allocate_structure(
	                     libfsrefs_attribute_values_t );

	if( *attribute_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create attribute values.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *attribute_values,
	     0,
	     sizeof( libfsrefs_attribute_values_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear attribute values.",
		 function );

		memory_free(
		 *attribute_values );

		*attribute_values = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *attribute_values )->data_runs_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data runs array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *attribute_values != NULL )
	{
		memory_free(
		 *attribute_values );

		*attribute_values = NULL;
	}
	return( -1 );
}

/* Frees a attribute values
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_attribute_values_free(
     libfsrefs_attribute_values_t **attribute_values,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_attribute_values_free";
	int result            = 1;

	if( attribute_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute values.",
		 function );

		return( -1 );
	}
	if( *attribute_values != NULL )
	{
		if( libcdata_array_free(
		     &( ( *attribute_values )->data_runs_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfsrefs_data_run_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free data runs array.",
			 function );

			result = -1;
		}
		if( ( *attribute_values )->name_data != NULL )
		{
			memory_free(
			 ( *attribute_values )->name_data );
		}
		memory_free(
		 *attribute_values );

		*attribute_values = NULL;
	}
	return( result );
}

/* Reads a non-resident attribute values
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_attribute_values_read_non_resident(
     libfsrefs_attribute_values_t *attribute_values,
     libfsrefs_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfsrefs_data_run_t *data_run       = NULL;
	libfsrefs_ministore_node_t *node     = NULL;
	libfsrefs_node_record_t *node_record = NULL;
	static char *function                = "libfsrefs_attribute_values_read_non_resident";
	int entry_index                      = 0;
	int number_of_records                = 0;
	int record_index                     = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit                 = 0;
	uint32_t value_32bit                 = 0;
#endif

	if( attribute_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute values.",
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
		 "%s: unable to create non-resident attribute ministore node.",
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
		 "%s: unable to read non-resident attribute ministore node.",
		 function );

		goto on_error;
	}
	if( ( node->node_type_flags & 0x03 ) != 0x02 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid non-resident attribute ministore node - unsupported node type flags.",
		 function );

		goto on_error;
	}
	if( node->header_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid non-resident attribute ministore node - missing header data.",
		 function );

		return( -1 );
	}
	if( node->header_data_size != sizeof( fsrefs_attribute_non_resident_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid non-resident attribute ministore node - header data size value out of bounds.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (fsrefs_attribute_non_resident_t *) node->header_data )->unknown1,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_attribute_non_resident_t *) node->header_data )->unknown2,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown2\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_attribute_non_resident_t *) node->header_data )->allocated_data_size,
		 value_64bit );
		libcnotify_printf(
		 "%s: allocated data size\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_attribute_non_resident_t *) node->header_data )->data_size,
		 value_64bit );
		libcnotify_printf(
		 "%s: data size\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_attribute_non_resident_t *) node->header_data )->valid_data_size,
		 value_64bit );
		libcnotify_printf(
		 "%s: valid data size\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "%s: unknown3:\n",
		 function );
		libcnotify_print_data(
		 ( (fsrefs_attribute_non_resident_t *) node->header_data )->unknown3,
		 60,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
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
			 "%s: unable to retrieve data run: %d record.",
			 function,
			 record_index );

			goto on_error;
		}
		if( libfsrefs_data_run_initialize(
		     &data_run,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create data run: %d.",
			 function,
			 record_index );

			goto on_error;
		}
		if( libfsrefs_data_run_read_data(
		     data_run,
		     io_handle,
		     node_record->value_data,
		     node_record->value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read data run: %d.",
			 function,
			 record_index );

			goto on_error;
		}
		if( libcdata_array_append_entry(
		     attribute_values->data_runs_array,
		     &entry_index,
		     (intptr_t *) data_run,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append data run: %d to array.",
			 function,
			 record_index );

			goto on_error;
		}
		data_run = NULL;
	}
	if( libfsrefs_ministore_node_free(
	     &node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free non-resident attribute ministore node.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( data_run != NULL )
	{
		libfsrefs_data_run_free(
		 &data_run,
		 NULL );
	}
	if( node != NULL )
	{
		libfsrefs_ministore_node_free(
		 &node,
		 NULL );
	}
	libcdata_array_empty(
	 attribute_values->data_runs_array,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfsrefs_data_run_free,
	 NULL );

	return( -1 );
}

/* Reads a resident attribute values
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_attribute_values_read_resident(
     libfsrefs_attribute_values_t *attribute_values,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function       = "libfsrefs_attribute_values_read_resident";
	uint32_t inline_data_offset = 0;
	uint32_t inline_data_size   = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	size_t data_offset          = 0;
	uint64_t value_64bit        = 0;
	uint32_t value_32bit        = 0;
#endif

	if( attribute_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute values.",
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
	if( ( data_size < sizeof( fsrefs_attribute_resident_t ) )
	 || ( data_size > (size_t) SSIZE_MAX ) )
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
		 "%s: resident attribute data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsrefs_attribute_resident_t *) data )->inline_data_offset,
	 inline_data_offset );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsrefs_attribute_resident_t *) data )->inline_data_size,
	 inline_data_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (fsrefs_attribute_resident_t *) data )->unknown1,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: inline data offset\t\t: 0x%08" PRIx32 "\n",
		 function,
		 inline_data_offset );

		libcnotify_printf(
		 "%s: inline data size\t\t: %" PRIu32 "\n",
		 function,
		 inline_data_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsrefs_attribute_resident_t *) data )->unknown3,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown3\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_attribute_resident_t *) data )->unknown4,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown4\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_attribute_resident_t *) data )->unknown5,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown5\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_attribute_resident_t *) data )->unknown6,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown6\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_attribute_resident_t *) data )->unknown7,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown7\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_attribute_resident_t *) data )->unknown8,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown8\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsrefs_attribute_resident_t *) data )->unknown9,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown9\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( ( inline_data_offset < sizeof( fsrefs_attribute_resident_t ) )
	 || ( inline_data_offset >= data_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid inline data offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( inline_data_size > ( data_size - inline_data_offset ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid inline data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: inline data:\n",
		 function );
		libcnotify_print_data(
		 &( data[ inline_data_offset ] ),
		 inline_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

		data_offset = inline_data_offset + inline_data_size;

		if( data_offset < data_size )
		{
			libcnotify_printf(
			 "%s: trailing data:\n",
			 function );
			libcnotify_print_data(
			 &( data[ data_offset ] ),
			 data_size - data_offset,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

/* Reads the attribute values from a ministore node record
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_attribute_values_read_node_record(
     libfsrefs_attribute_values_t *attribute_values,
     libfsrefs_io_handle_t *io_handle,
     libfsrefs_node_record_t *node_record,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_attribute_values_read_node_record";
	size_t name_data_size = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit  = 0;
#endif

	if( attribute_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute values.",
		 function );

		return( -1 );
	}
	if( attribute_values->name_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid attribute values - namd data value already set.",
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
	if( node_record->key_data_size < 14 )
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
	byte_stream_copy_to_uint32_little_endian(
	 &( node_record->key_data[ 8 ] ),
	 attribute_values->attribute_type );

	name_data_size = node_record->key_data_size - 12;

	attribute_values->name_data = (uint8_t *) memory_allocate(
	                                           sizeof( uint8_t ) * name_data_size );

	if( attribute_values->name_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create name data.",
		 function );

		goto on_error;
	}
	attribute_values->name_data_size = name_data_size;

	if( memory_copy(
	     attribute_values->name_data,
	     &( node_record->key_data[ 12 ] ),
	     attribute_values->name_data_size ) == NULL )
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
		byte_stream_copy_to_uint64_little_endian(
		 &( node_record->key_data[ 0 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "%s: attribute type\t\t: 0x%08" PRIx32 "\n",
		 function,
		 attribute_values->attribute_type );

		if( libfsrefs_debug_print_utf16_string_value(
		     function,
		     "name\t\t\t",
		     attribute_values->name_data,
		     attribute_values->name_data_size,
		     LIBUNA_ENDIAN_LITTLE,
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

	if( ( node_record->flags & 0x0008 ) != 0 )
	{
		if( libfsrefs_attribute_values_read_non_resident(
		     attribute_values,
		     io_handle,
		     node_record->value_data,
		     node_record->value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read non-resident attribute values.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( libfsrefs_attribute_values_read_resident(
		     attribute_values,
		     node_record->value_data,
		     node_record->value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read resident attribute values.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( attribute_values->name_data != NULL )
	{
		memory_free(
		 attribute_values->name_data );

		attribute_values->name_data = NULL;
	}
	attribute_values->name_data_size = 0;

	libcdata_array_empty(
	 attribute_values->data_runs_array,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfsrefs_data_run_free,
	 NULL );

	return( -1 );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_attribute_values_get_utf8_name_size(
     libfsrefs_attribute_values_t *attribute_values,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_attribute_values_get_utf8_name_size";

	if( attribute_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute values.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_size_from_utf16_stream(
	     attribute_values->name_data,
	     attribute_values->name_data_size,
	     LIBUNA_ENDIAN_LITTLE,
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
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_attribute_values_get_utf8_name(
     libfsrefs_attribute_values_t *attribute_values,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_attribute_values_get_utf8_name";

	if( attribute_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute values.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_copy_from_utf16_stream(
	     utf8_string,
	     utf8_string_size,
	     attribute_values->name_data,
	     attribute_values->name_data_size,
	     LIBUNA_ENDIAN_LITTLE,
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
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_attribute_values_get_utf16_name_size(
     libfsrefs_attribute_values_t *attribute_values,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_attribute_values_get_utf16_name_size";

	if( attribute_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute values.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_string_size_from_utf16_stream(
	     attribute_values->name_data,
	     attribute_values->name_data_size,
	     LIBUNA_ENDIAN_LITTLE,
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
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_attribute_values_get_utf16_name(
     libfsrefs_attribute_values_t *attribute_values,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_attribute_values_get_utf16_name";

	if( attribute_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute values.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_string_copy_from_utf16_stream(
	     utf16_string,
	     utf16_string_size,
	     attribute_values->name_data,
	     attribute_values->name_data_size,
	     LIBUNA_ENDIAN_LITTLE,
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

