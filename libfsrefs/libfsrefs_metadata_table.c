/*
 * Metadata table functions
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

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libfsrefs_libcdata.h"
#include "libfsrefs_libcerror.h"
#include "libfsrefs_libcnotify.h"
#include "libfsrefs_metadata_table.h"
#include "libfsrefs_metadata_value.h"

/* Creates a metadata table
 * Make sure the value metadata_table is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_metadata_table_initialize(
     libfsrefs_metadata_table_t **metadata_table,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_metadata_table_initialize";

	if( metadata_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata table.",
		 function );

		return( -1 );
	}
	if( *metadata_table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid metadata table value already set.",
		 function );

		return( -1 );
	}
	*metadata_table = memory_allocate_structure(
	                   libfsrefs_metadata_table_t );

	if( *metadata_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create metadata table.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *metadata_table,
	     0,
	     sizeof( libfsrefs_metadata_table_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear metadata table.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( ( *metadata_table )->values_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create values array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *metadata_table != NULL )
	{
		memory_free(
		 *metadata_table );

		*metadata_table = NULL;
	}
	return( -1 );
}

/* Frees a metadata table
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_metadata_table_free(
     libfsrefs_metadata_table_t **metadata_table,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_metadata_table_free";
	int result            = 1;

	if( metadata_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata table.",
		 function );

		return( -1 );
	}
	if( *metadata_table != NULL )
	{
		if( libcdata_array_free(
		     &( ( *metadata_table )->values_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfsrefs_metadata_value_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free values array.",
			 function );

			result = -1;
		}
		memory_free(
		 *metadata_table );

		*metadata_table = NULL;
	}
	return( result );
}

/* Reads the metadata table
 * Returns the number of bytes read if successful or -1 on error
 */
int libfsrefs_metadata_table_read(
     libfsrefs_metadata_table_t *metadata_table,
     const uint8_t *metadata_block_data,
     size_t metadata_block_data_size,
     size_t metadata_block_data_offset,
     int level,
     libcerror_error_t **error )
{
	libfsrefs_metadata_value_t *metadata_value = NULL;
	static char *function                      = "libfsrefs_metadata_table_read";
	size_t entry_offset                        = 0;
	size_t values_metadata_block_data_offset   = 0;
	uint32_t entry_size                        = 0;
	uint32_t entry_index                       = 0;
	uint32_t header_entry_size                 = 0;
	uint32_t number_of_values                  = 0;
	uint32_t unused_table_data_offset          = 0;
	uint32_t unused_table_data_size            = 0;
	uint32_t value_data_offset                 = 0;
	uint32_t value_index                       = 0;
	uint32_t value_offsets_array_end_offset    = 0;
	uint32_t value_offsets_array_size          = 0;
	uint32_t value_offsets_array_start_offset  = 0;
	uint32_t value_size                        = 0;
	uint32_t trailing_data_size                = 0;
	int array_entry_index                      = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit                       = 0;
	uint32_t value_32bit                       = 0;
	uint16_t value_16bit                       = 0;
#endif

	if( metadata_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata table.",
		 function );

		return( -1 );
	}
	if( metadata_block_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata block data.",
		 function );

		return( -1 );
	}
	if( metadata_block_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: metadata block data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( metadata_block_data_offset > metadata_block_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: metadata block data offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( metadata_block_data_offset + 4 ) > metadata_block_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: metadata block data value too small.",
		 function );

		return( -1 );
	}
	entry_index = 0;

	while( metadata_block_data_offset < metadata_block_data_size )
	{
		if( entry_index >= 2 )
		{
			break;
		}
		byte_stream_copy_to_uint32_little_endian(
		 &( metadata_block_data[ metadata_block_data_offset ] ),
		 entry_size );

		if( entry_size == 0 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: level: %d entry: %02" PRIu32 " size\t\t\t: %" PRIu32 "\n",
				 function,
				 level,
				 entry_index,
				 entry_size );

				libcnotify_printf(
				 "\n" );
			}
#endif
			break;
		}
		if( ( metadata_block_data_offset + entry_size ) > metadata_block_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: level: %d entry: %02" PRIu32 " size value out of bounds.",
			 function,
			 level,
			 entry_index );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( ( entry_index == 0 )
			 || ( entry_index == 1 ) )
			{
				libcnotify_printf(
				 "%s: level: %d entry: %02" PRIu32 " data:\n",
				 function,
				 level,
				 entry_index );
				libcnotify_print_data(
				 &( metadata_block_data[ metadata_block_data_offset ] ),
				 (size_t) entry_size,
				 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
			}
			else
			{
				libcnotify_printf(
				 "%s: level: %d entry: %02" PRIu32 " offset\t\t: 0x%08" PRIzx "\n",
				 function,
				 level,
				 entry_index,
				 entry_offset );
			}
		}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( entry_index == 0 )
			{
				libcnotify_printf(
				 "%s: level: %d entry: %02" PRIu32 " size\t\t\t: %" PRIu32 "\n",
				 function,
				 level,
				 entry_index,
				 entry_size );
			}
			else
			{
				libcnotify_printf(
				 "%s: level: %d entry: %02" PRIu32 " size\t\t\t\t\t: %" PRIu32 "\n",
				 function,
				 level,
				 entry_index,
				 entry_size );
			}
		}
#endif
		if( entry_index == 0 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				byte_stream_copy_to_uint16_little_endian(
				 &( metadata_block_data[ metadata_block_data_offset + 4 ] ),
				 value_16bit );
				libcnotify_printf(
				 "%s: level: %d entry: %02" PRIu32 " unknown1a\t\t: 0x%04" PRIx16 "\n",
				 function,
				 level,
				 entry_index,
				 value_16bit );

				byte_stream_copy_to_uint16_little_endian(
				 &( metadata_block_data[ metadata_block_data_offset + 6 ] ),
				 value_16bit );
				libcnotify_printf(
				 "%s: level: %d entry: %02" PRIu32 " unknown1b\t\t: 0x%04" PRIx16 "\n",
				 function,
				 level,
				 entry_index,
				 value_16bit );

				if( entry_size > 8 )
				{
					byte_stream_copy_to_uint32_little_endian(
					 &( metadata_block_data[ metadata_block_data_offset + 8 ] ),
					 value_32bit );
					libcnotify_printf(
					 "%s: level: %d entry: %02" PRIu32 " unknown2\t\t: 0x%08" PRIx32 "\n",
					 function,
					 level,
					 entry_index,
					 value_32bit );

					byte_stream_copy_to_uint32_little_endian(
					 &( metadata_block_data[ metadata_block_data_offset + 12 ] ),
					 value_32bit );
					libcnotify_printf(
					 "%s: level: %d entry: %02" PRIu32 " unknown3\t\t: 0x%08" PRIx32 "\n",
					 function,
					 level,
					 entry_index,
					 value_32bit );

					byte_stream_copy_to_uint32_little_endian(
					 &( metadata_block_data[ metadata_block_data_offset + 16 ] ),
					 value_32bit );
					libcnotify_printf(
					 "%s: level: %d entry: %02" PRIu32 " unknown4\t\t: 0x%08" PRIx32 "\n",
					 function,
					 level,
					 entry_index,
					 value_32bit );

					byte_stream_copy_to_uint32_little_endian(
					 &( metadata_block_data[ metadata_block_data_offset + 20 ] ),
					 value_32bit );
					libcnotify_printf(
					 "%s: level: %d entry: %02" PRIu32 " unknown5\t\t: 0x%08" PRIx32 "\n",
					 function,
					 level,
					 entry_index,
					 value_32bit );

					byte_stream_copy_to_uint64_little_endian(
					 &( metadata_block_data[ metadata_block_data_offset + 24 ] ),
					 value_64bit );
					libcnotify_printf(
					 "%s: level: %d entry: %02" PRIu32 " unknown6\t\t: 0x%08" PRIx32 "\n",
					 function,
					 level,
					 entry_index,
					 value_64bit );

					byte_stream_copy_to_uint64_little_endian(
					 &( metadata_block_data[ metadata_block_data_offset + 32 ] ),
					 value_64bit );
					libcnotify_printf(
					 "%s: level: %d entry: %02" PRIu32 " unknown7\t\t: 0x%08" PRIx32 "\n",
					 function,
					 level,
					 entry_index,
					 value_64bit );
				}
				if( entry_size > 40 )
				{
					libcnotify_printf(
					 "%s: level: %d entry: %02" PRIu32 " remaining data:\n",
					 function,
					 level,
					 entry_index );
					libcnotify_print_data(
					 &( metadata_block_data[ metadata_block_data_offset + 40 ] ),
					 entry_size - 40,
					 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
				}
				else
				{
					libcnotify_printf(
					 "\n" );
				}
			}
#endif
			header_entry_size = entry_size;
		}
		else if( entry_index == 1 )
		{
			byte_stream_copy_to_uint32_little_endian(
			 &( metadata_block_data[ metadata_block_data_offset + 4 ] ),
			 unused_table_data_offset );

			byte_stream_copy_to_uint32_little_endian(
			 &( metadata_block_data[ metadata_block_data_offset + 8 ] ),
			 unused_table_data_size );

			byte_stream_copy_to_uint32_little_endian(
			 &( metadata_block_data[ metadata_block_data_offset + 16 ] ),
			 value_offsets_array_start_offset );

			byte_stream_copy_to_uint32_little_endian(
			 &( metadata_block_data[ metadata_block_data_offset + 20 ] ),
			 number_of_values );

			byte_stream_copy_to_uint32_little_endian(
			 &( metadata_block_data[ metadata_block_data_offset + 24 ] ),
			 value_offsets_array_end_offset );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: level: %d entry: %02" PRIu32 " unused table data offset\t\t: 0x%08" PRIx32 " (0x%08" PRIx32 ")\n",
				 function,
				 level,
				 entry_index,
				 unused_table_data_offset,
				 unused_table_data_offset + header_entry_size );

				libcnotify_printf(
				 "%s: level: %d entry: %02" PRIu32 " unused table data size\t\t: %" PRIu32 "\n",
				 function,
				 level,
				 entry_index,
				 unused_table_data_size );

				byte_stream_copy_to_uint32_little_endian(
				 &( metadata_block_data[ metadata_block_data_offset + 12 ] ),
				 value_32bit );
				libcnotify_printf(
				 "%s: level: %d entry: %02" PRIu32 " unknown3\t\t\t\t: 0x%08" PRIx32 "\n",
				 function,
				 level,
				 entry_index,
				 value_32bit );

				libcnotify_printf(
				 "%s: level: %d entry: %02" PRIu32 " value offsets array start offset\t: 0x%08" PRIx32 " (0x%08" PRIx32 ")\n",
				 function,
				 level,
				 entry_index,
				 value_offsets_array_start_offset,
				 value_offsets_array_start_offset + header_entry_size );

				libcnotify_printf(
				 "%s: level: %d entry: %02" PRIu32 " number of values\t\t\t: %" PRIu32 "\n",
				 function,
				 level,
				 entry_index,
				 number_of_values );

				libcnotify_printf(
				 "%s: level: %d entry: %02" PRIu32 " value offsets array end offset\t: 0x%08" PRIx32 " (0x%08" PRIx32 ")\n",
				 function,
				 level,
				 entry_index,
				 value_offsets_array_end_offset,
				 value_offsets_array_end_offset + header_entry_size );

				byte_stream_copy_to_uint32_little_endian(
				 &( metadata_block_data[ metadata_block_data_offset + 28 ] ),
				 value_32bit );
				libcnotify_printf(
				 "%s: level: %d entry: %02" PRIu32 " unknown7\t\t\t\t: 0x%08" PRIx32 "\n",
				 function,
				 level,
				 entry_index,
				 value_32bit );

				libcnotify_printf(
				 "\n" );
			}
#endif
			values_metadata_block_data_offset = metadata_block_data_offset;

			if( ( unused_table_data_offset < 0x00000020UL )
			 || ( unused_table_data_offset > ( metadata_block_data_size - header_entry_size ) ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: level: %d entry: %02" PRIu32 " unused table data offset value out of bounds\n",
				 function,
				 level,
				 entry_index );

				goto on_error;
			}
			if( ( unused_table_data_offset + unused_table_data_size ) > ( metadata_block_data_size - header_entry_size ) )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: level: %d entry: %02" PRIu32 " unused table data size value out of bounds\n",
					 function,
					 level,
					 entry_index );
				}
#endif
				unused_table_data_size = ( metadata_block_data_size - header_entry_size ) - unused_table_data_offset;
			}
			if( ( value_offsets_array_start_offset < 0x00000020UL )
			 || ( value_offsets_array_start_offset > ( metadata_block_data_size - header_entry_size ) ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: level: %d entry: %02" PRIu32 " value offsets array start offset value out of bounds\n",
				 function,
				 level,
				 entry_index );

				goto on_error;
			}
			if( ( value_offsets_array_end_offset < 0x00000020UL )
			 || ( value_offsets_array_end_offset > ( metadata_block_data_size - header_entry_size ) ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: level: %d entry: %02" PRIu32 " value offsets array end offset value out of bounds\n",
				 function,
				 level,
				 entry_index );

				goto on_error;
			}
			if( value_offsets_array_start_offset > value_offsets_array_end_offset )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: level: %d entry: %02" PRIu32 " value offsets array start offset exceeds end offset\n",
				 function,
				 level,
				 entry_index );

				goto on_error;
			}
			value_offsets_array_size = number_of_values * 4;

			if( ( value_offsets_array_end_offset - value_offsets_array_start_offset ) != value_offsets_array_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_INPUT,
				 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
				 "%s: level: %d entry: %02" PRIu32 " mismatch in value offsets array size\n",
				 function,
				 level,
				 entry_index );

				goto on_error;
			}
			unused_table_data_offset         += values_metadata_block_data_offset;
			value_offsets_array_start_offset += values_metadata_block_data_offset;
		}
		metadata_block_data_offset += (size_t) entry_size;
		metadata_table->size       += (size_t) entry_size;

		if( entry_index != 0 )
		{
			entry_offset += (size_t) entry_size;
		}
		entry_index++;
	}
/* TODO remove ?
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( metadata_block_data_offset < unused_table_data_offset )
		{
			trailing_data_size = unused_table_data_offset - metadata_block_data_offset;

			libcnotify_printf(
			 "%s: level: %d entries trailing data:\n",
			 function,
			 level );
			libcnotify_print_data(
			 &( metadata_block_data[ metadata_block_data_offset ] ),
			 (size_t) trailing_data_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

			metadata_block_data_offset = unused_table_data_offset;
		}
	}
#endif
*/
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: level: %d unused table data:\n",
		 function,
		 level );
		libcnotify_print_data(
		 &( metadata_block_data[ unused_table_data_offset ] ),
		 (size_t) unused_table_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

		metadata_block_data_offset += unused_table_data_size;
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( metadata_block_data_offset < value_offsets_array_start_offset )
		{
			trailing_data_size = value_offsets_array_start_offset - metadata_block_data_offset;

			libcnotify_printf(
			 "%s: level: %d unused table data trailing data:\n",
			 function,
			 level );
			libcnotify_print_data(
			 &( metadata_block_data[ metadata_block_data_offset ] ),
			 (size_t) trailing_data_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

			metadata_block_data_offset = value_offsets_array_start_offset;
		}
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: level: %d value offsets array data:\n",
		 function,
		 level );
		libcnotify_print_data(
		 &( metadata_block_data[ value_offsets_array_start_offset ] ),
		 (size_t) value_offsets_array_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

		metadata_block_data_offset += value_offsets_array_size;
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( metadata_block_data_offset < metadata_block_data_size )
		{
			trailing_data_size = metadata_block_data_size - metadata_block_data_offset;

			libcnotify_printf(
			 "%s: level: %d trailing data:\n",
			 function,
			 level );
			libcnotify_print_data(
			 &( metadata_block_data[ metadata_block_data_offset ] ),
			 (size_t) trailing_data_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
	}
#endif
	/* Read the table values in order
	 */
	for( value_index = 0;
	     value_index < number_of_values;
	     value_index++ )
	{
		byte_stream_copy_to_uint32_little_endian(
		 &( metadata_block_data[ value_offsets_array_start_offset ] ),
		 value_data_offset );

		value_offsets_array_start_offset += 4;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: level: %d value: %02" PRIu32 " value data offset\t: 0x%08" PRIx32 " (0x%08" PRIx32 ")\n",
			 function,
			 level,
			 value_index,
			 value_data_offset,
			 value_data_offset + header_entry_size );
		}
#endif
		if( ( value_data_offset < 0x00000020UL )
		 || ( value_data_offset > ( metadata_block_data_size - header_entry_size ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: level: %d value: %02" PRIu32 " value data offset value out of bounds\n",
			 function,
			 level,
			 value_index );

			goto on_error;
		}
		value_data_offset += values_metadata_block_data_offset;

		byte_stream_copy_to_uint32_little_endian(
		 &( metadata_block_data[ value_data_offset ] ),
		 value_size );

		if( libfsrefs_metadata_value_initialize(
		     &metadata_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create metadata value.",
			 function );

			goto on_error;
		}
		if( libfsrefs_metadata_value_read(
		     metadata_value,
		     &( metadata_block_data[ value_data_offset ] ),
		     (size_t) value_size,
		     0,
		     level,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read metadata value.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( metadata_value->size < value_size )
			{
				trailing_data_size = value_size - metadata_value->size;

				libcnotify_printf(
				 "%s: level: %d value: %02" PRIu32 " trailing data:\n",
				 function,
				 level,
				 value_index );
				libcnotify_print_data(
				 &( metadata_block_data[ value_data_offset + metadata_value->size ] ),
				 (size_t) trailing_data_size,
				 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
			}
		}
#endif
		if( libcdata_array_append_entry(
		     metadata_table->values_array,
		     &array_entry_index,
		     (intptr_t *) metadata_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append value to array.",
			 function );

			goto on_error;
		}
		metadata_value = NULL;
	}
	return( 1 );

on_error:
	if( metadata_value != NULL )
	{
		libfsrefs_metadata_value_free(
		 &metadata_value,
		 NULL );
	}
	return( -1 );
}

#ifdef REFACTOR
	uint32_t value_data_size                   = 0;
/* TODO
			if( ( metadata_value->flags & 0x0008 ) != 0 )
*/
			byte_stream_copy_to_uint32_little_endian(
			 metadata_value->data,
			 value_data_size );

			if( value_data_size != 0 )
			{
				if( libfsrefs_metadata_table_initialize(
				     &sub_metadata_table,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create sub metadata table.",
					 function );

					goto on_error;
				}
				if( libfsrefs_metadata_table_read(
				     sub_metadata_table,
				     metadata_value->data,
				     (size_t) metadata_value->data_size,
				     0,
				     level + 1,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read sub metadata table.",
					 function );

					goto on_error;
				}
				if( libfsrefs_metadata_table_free(
				     &sub_metadata_table,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free sub metadata table.",
					 function );

					goto on_error;
				}
			}
#endif /* REFACTOR */

/* Retrieves the number of metadata table values
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_metadata_table_get_number_of_values(
     libfsrefs_metadata_table_t *metadata_table,
     int *number_of_values,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_metadata_table_get_number_of_values";

	if( metadata_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata table.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     metadata_table->values_array,
	     number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific metadata table value
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_metadata_table_get_value_by_index(
     libfsrefs_metadata_table_t *metadata_table,
     int value_index,
     libfsrefs_metadata_value_t **metadata_value,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_metadata_table_get_value_by_index";

	if( metadata_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata table.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     metadata_table->values_array,
	     value_index,
	     (intptr_t **) metadata_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from array.",
		 function,
		 value_index );

		return( -1 );
	}
	return( 1 );
}

