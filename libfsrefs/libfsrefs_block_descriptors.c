/*
 * Block descriptors functions
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
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "libfsrefs_block_descriptor.h"
#include "libfsrefs_block_descriptors.h"
#include "libfsrefs_io_handle.h"
#include "libfsrefs_libbfio.h"
#include "libfsrefs_libcdata.h"
#include "libfsrefs_libcerror.h"
#include "libfsrefs_libcnotify.h"
#include "libfsrefs_libuna.h"
#include "libfsrefs_metadata_block.h"
#include "libfsrefs_metadata_table.h"
#include "libfsrefs_metadata_value.h"

#include "fsrefs_metadata_block.h"

/* Creates block descriptors
 * Make sure the value block_descriptors is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_block_descriptors_initialize(
     libfsrefs_block_descriptors_t **block_descriptors,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_block_descriptors_initialize";

	if( block_descriptors == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block descriptors.",
		 function );

		return( -1 );
	}
	if( *block_descriptors != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid block descriptors value already set.",
		 function );

		return( -1 );
	}
	*block_descriptors = memory_allocate_structure(
	                      libfsrefs_block_descriptors_t );

	if( *block_descriptors == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create block descriptors.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *block_descriptors,
	     0,
	     sizeof( libfsrefs_block_descriptors_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear block descriptors.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( ( *block_descriptors )->block_descriptors_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create block descriptors array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *block_descriptors != NULL )
	{
		memory_free(
		 *block_descriptors );

		*block_descriptors = NULL;
	}
	return( -1 );
}

/* Frees block descriptors
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_block_descriptors_free(
     libfsrefs_block_descriptors_t **block_descriptors,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_block_descriptors_free";
	int result            = 1;

	if( block_descriptors == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block descriptors.",
		 function );

		return( -1 );
	}
	if( *block_descriptors != NULL )
	{
		if( libcdata_array_free(
		     &( ( *block_descriptors )->block_descriptors_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfsrefs_block_descriptor_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free block descriptors array.",
			 function );

			result = -1;
		}
		memory_free(
		 *block_descriptors );

		*block_descriptors = NULL;
	}
	return( result );
}

/* Reads the block descriptors
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_block_descriptors_read(
     libfsrefs_block_descriptors_t *block_descriptors,
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     int level,
     libcerror_error_t **error )
{
	libfsrefs_block_descriptor_t *block_descriptor = NULL;
	libfsrefs_metadata_block_t *metadata_block     = NULL;
	libfsrefs_metadata_table_t *metadata_table     = NULL;
	libfsrefs_metadata_value_t *metadata_value     = NULL;
	static char *function                          = "libfsrefs_block_descriptors_read";
	int array_entry_index                          = 0;
	int metadata_table_value_index                 = 0;
	int number_of_metadata_table_values            = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	system_character_t *value_string               = NULL;
	size_t value_string_size                       = 0;
	uint16_t value_16bit                           = 0;
	int result                                     = 0;
#endif

	if( block_descriptors == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block descriptors.",
		 function );

		return( -1 );
	}
	if( libfsrefs_metadata_block_initialize(
	     &metadata_block,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create metadata block.",
		 function );

		goto on_error;
	}
	if( libfsrefs_metadata_block_read(
	     metadata_block,
	     io_handle,
	     file_io_handle,
	     file_offset,
	     level,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read metadata block.",
		 function );

		goto on_error;
	}
	if( libfsrefs_metadata_table_initialize(
	     &metadata_table,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create metadata table.",
		 function );

		goto on_error;
	}
	if( libfsrefs_metadata_table_read(
	     metadata_table,
	     metadata_block->data,
	     (size_t) metadata_block->data_size - sizeof( fsrefs_metadata_block_header_t ),
	     sizeof( fsrefs_metadata_block_header_t ),
	     level,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read metadata table.",
		 function );

		goto on_error;
	}
	if( libfsrefs_metadata_table_get_number_of_values(
	     metadata_table,
	     &number_of_metadata_table_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values from metdata table.",
		 function );

		goto on_error;
	}
	for( metadata_table_value_index = 0;
	     metadata_table_value_index < number_of_metadata_table_values;
	     metadata_table_value_index++ )
	{
		if( libfsrefs_metadata_table_get_value_by_index(
		     metadata_table,
		     metadata_table_value_index,
		     &metadata_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %d from metadata table.",
			 function,
			 metadata_table_value_index );

			goto on_error;
		}
		if( metadata_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing metadata value: %d.",
			 function,
			 metadata_table_value_index );

			goto on_error;
		}
		if( ( metadata_value->identifier_data[ 0 ] == 0x30 )
		 && ( metadata_value->identifier_data[ 1 ] == 0x00 )
		 && ( metadata_value->identifier_data_size > 4 ) )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				byte_stream_copy_to_uint16_little_endian(
				 metadata_value->identifier_data,
				 value_16bit );
				libcnotify_printf(
				 "%s: level: %d value: %02d identifier signature\t: 0x%04" PRIx16 "\n",
				 function,
				 level,
				 metadata_table_value_index,
				 value_16bit );

				byte_stream_copy_to_uint16_little_endian(
				 &( metadata_value->identifier_data[ 2 ] ),
				 value_16bit );
				libcnotify_printf(
				 "%s: level: %d value: %02d identifier file entry type\t: 0x%04" PRIx16 "\n",
				 function,
				 level,
				 metadata_table_value_index,
				 value_16bit );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
				result = libuna_utf16_string_size_from_utf16_stream(
				          &( metadata_value->identifier_data[ 4 ] ),
				          metadata_value->identifier_data_size - 4,
					  LIBUNA_ENDIAN_LITTLE,
					  &value_string_size,
					  error );
#else
				result = libuna_utf8_string_size_from_utf16_stream(
				          &( metadata_value->identifier_data[ 4 ] ),
				          metadata_value->identifier_data_size - 4,
					  LIBUNA_ENDIAN_LITTLE,
					  &value_string_size,
					  error );
#endif
				if( result != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to determine size of value string.",
					 function );

					goto on_error;
				}
				value_string = system_string_allocate(
						value_string_size );

				if( value_string == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to create value string.",
					 function );

					goto on_error;
				}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
				result = libuna_utf16_string_copy_from_utf16_stream(
					  (libuna_utf16_character_t *) value_string,
					  value_string_size,
				          &( metadata_value->identifier_data[ 4 ] ),
				          metadata_value->identifier_data_size - 4,
					  LIBUNA_ENDIAN_LITTLE,
					  error );
#else
				result = libuna_utf8_string_copy_from_utf16_stream(
					  (libuna_utf8_character_t *) value_string,
					  value_string_size,
				          &( metadata_value->identifier_data[ 4 ] ),
				          metadata_value->identifier_data_size - 4,
					  LIBUNA_ENDIAN_LITTLE,
					  error );
#endif
				if( result != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set value string.",
					 function );

					goto on_error;
				}
				libcnotify_printf(
				 "%s: level: %d value: %02d identifier string\t\t: %" PRIs_SYSTEM "\n",
				 function,
				 level,
				 metadata_table_value_index,
				 value_string );

				memory_free(
				 value_string );

				value_string = NULL;

				libcnotify_printf(
				 "\n" );
			}
#endif
		}
#if defined( HAVE_DEBUG_OUTPUT )
		else if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: level: %d value: %02d identifier data:\n",
			 function,
			 level,
			 metadata_table_value_index );
			libcnotify_print_data(
			 metadata_value->identifier_data,
			 metadata_value->identifier_data_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif
		if( libfsrefs_block_descriptor_initialize(
		     &block_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create block descriptor.",
			 function );

			goto on_error;
		}
		if( libfsrefs_block_descriptor_set_identifier(
		     block_descriptor,
		     metadata_value->identifier_data,
		     metadata_value->identifier_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set block descriptor identifier.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: level: %d value: %02d identifier data:\n",
			 function,
			 level,
			 metadata_table_value_index );
			libcnotify_print_data(
			 metadata_value->data,
			 24,
			 0 );
		}
#endif
		byte_stream_copy_to_uint64_little_endian(
		 metadata_value->data,
		 block_descriptor->block_number );

		byte_stream_copy_to_uint64_little_endian(
		 &( metadata_value->data[ 8 ] ),
		 block_descriptor->unknown );

		byte_stream_copy_to_uint64_little_endian(
		 &( metadata_value->data[ 16 ] ),
		 block_descriptor->checksum );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: level: %d value: %02d block number\t\t: %" PRIu64 "\n",
			 function,
			 level,
			 metadata_table_value_index,
			 block_descriptor->block_number );

			libcnotify_printf(
			 "%s: level: %d value: %02d unknown1\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 level,
			 metadata_table_value_index,
			 block_descriptor->unknown );

			libcnotify_printf(
			 "%s: level: %d value: %02d checksum\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 level,
			 metadata_table_value_index,
			 block_descriptor->checksum );
		}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( metadata_value->data_size > 24 )
			{
				libcnotify_printf(
				 "%s: level: %d value: %02d remaining data:\n",
				 function,
				 level,
				 metadata_table_value_index );
				libcnotify_print_data(
				 &( metadata_value->data[ 24 ] ),
				 metadata_value->data_size - 24,
				 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
			}
			else
			{
				libcnotify_printf(
				 "\n" );
			}
		}
#endif
		if( libcdata_array_append_entry(
		     block_descriptors->block_descriptors_array,
		     &array_entry_index,
		     (intptr_t *) block_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append block descriptor to array.",
			 function );

			goto on_error;
		}
		block_descriptor = NULL;
	}
	if( libfsrefs_metadata_table_free(
	     &metadata_table,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free metadata table.",
		 function );

		goto on_error;
	}
	if( libfsrefs_metadata_block_free(
	     &metadata_block,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free metadata block.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( block_descriptor != NULL )
	{
		libfsrefs_block_descriptor_free(
		 &block_descriptor,
		 NULL );
	}
	if( metadata_table != NULL )
	{
		libfsrefs_metadata_table_free(
		 &metadata_table,
		 NULL );
	}
	if( metadata_block != NULL )
	{
		libfsrefs_metadata_block_free(
		 &metadata_block,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the number of block descriptors
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_block_descriptors_get_number_of_block_descriptors(
     libfsrefs_block_descriptors_t *block_descriptors,
     int *number_of_block_descriptors,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_block_descriptors_get_number_of_block_descriptors";

	if( block_descriptors == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block descriptors.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     block_descriptors->block_descriptors_array,
	     number_of_block_descriptors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific block descriptor
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_block_descriptors_get_block_descriptor_by_index(
     libfsrefs_block_descriptors_t *block_descriptors,
     int block_descriptor_index,
     libfsrefs_block_descriptor_t **block_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_block_descriptors_get_block_descriptor_by_index";

	if( block_descriptors == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block descriptors.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     block_descriptors->block_descriptors_array,
	     block_descriptor_index,
	     (intptr_t **) block_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry: %d from array.",
		 function,
		 block_descriptor_index );

		return( -1 );
	}
	return( 1 );
}

