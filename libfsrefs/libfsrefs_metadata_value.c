/*
 * Metadata value functions
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

#include "libfsrefs_libcerror.h"
#include "libfsrefs_libcnotify.h"
#include "libfsrefs_metadata_table.h"
#include "libfsrefs_metadata_value.h"

/* Creates a metadata value
 * Make sure the value metadata_value is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_metadata_value_initialize(
     libfsrefs_metadata_value_t **metadata_value,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_metadata_value_initialize";

	if( metadata_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata value.",
		 function );

		return( -1 );
	}
	if( *metadata_value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid metadata value value already set.",
		 function );

		return( -1 );
	}
	*metadata_value = memory_allocate_structure(
	                   libfsrefs_metadata_value_t );

	if( *metadata_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create metadata value.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *metadata_value,
	     0,
	     sizeof( libfsrefs_metadata_value_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear metadata value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *metadata_value != NULL )
	{
		memory_free(
		 *metadata_value );

		*metadata_value = NULL;
	}
	return( -1 );
}

/* Frees a metadata value
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_metadata_value_free(
     libfsrefs_metadata_value_t **metadata_value,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_metadata_value_free";

	if( metadata_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata value.",
		 function );

		return( -1 );
	}
	if( *metadata_value != NULL )
	{
		memory_free(
		 *metadata_value );

		*metadata_value = NULL;
	}
	return( 1 );
}

/* Reads the metadata value
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_metadata_value_read(
     libfsrefs_metadata_value_t *metadata_value,
     const uint8_t *metadata_block_data,
     size_t metadata_block_data_size,
     size_t metadata_block_data_offset,
     int level,
     libcerror_error_t **error )
{
	static char *function           = "libfsrefs_metadata_value_read";
	uint16_t data_offset            = 0;
	uint16_t identifier_data_offset = 0;

	if( metadata_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata value.",
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
	byte_stream_copy_to_uint32_little_endian(
	 &( metadata_block_data[ metadata_block_data_offset ] ),
	 metadata_value->size );

	if( metadata_value->size > 0 )
	{
		if( ( metadata_block_data_offset + metadata_value->size ) > metadata_block_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: value size value out of bounds.",
			 function );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: value data:\n",
			 function );
			libcnotify_print_data(
			 &( metadata_block_data[ metadata_block_data_offset ] ),
			 (size_t) metadata_value->size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: level: %d value size\t\t\t: %" PRIu32 "\n",
			 function,
			 level,
			 metadata_value->size );
		}
#endif
		if( metadata_value->size < 14 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: metadata block data value too small.",
			 function );

			return( -1 );
		}
		byte_stream_copy_to_uint16_little_endian(
		 &( metadata_block_data[ metadata_block_data_offset + 4 ] ),
		 identifier_data_offset );

		byte_stream_copy_to_uint16_little_endian(
		 &( metadata_block_data[ metadata_block_data_offset + 6 ] ),
		 metadata_value->identifier_data_size );

		byte_stream_copy_to_uint16_little_endian(
		 &( metadata_block_data[ metadata_block_data_offset + 8 ] ),
		 metadata_value->flags );

		byte_stream_copy_to_uint16_little_endian(
		 &( metadata_block_data[ metadata_block_data_offset + 10 ] ),
		 data_offset );

		byte_stream_copy_to_uint32_little_endian(
		 &( metadata_block_data[ metadata_block_data_offset + 12 ] ),
		 metadata_value->data_size );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: level: %d identifier data offset\t\t: 0x%04" PRIx16 "\n",
			 function,
			 level,
			 identifier_data_offset );

			libcnotify_printf(
			 "%s: level: %d identifier data size\t\t: %" PRIu16 "\n",
			 function,
			 level,
			 metadata_value->identifier_data_size );

			libcnotify_printf(
			 "%s: level: %d flags\t\t\t\t: 0x%04" PRIx16 "\n",
			 function,
			 level,
			 metadata_value->flags );

			libcnotify_printf(
			 "%s: level: %d data offset\t\t\t: 0x%04" PRIx16 "\n",
			 function,
			 level,
			 data_offset );

			libcnotify_printf(
			 "%s: level: %d data size\t\t\t: %" PRIu32 "\n",
			 function,
			 level,
			 metadata_value->data_size );
		}
#endif
		if( ( identifier_data_offset < 16 )
		 || ( ( metadata_block_data_offset + identifier_data_offset ) > metadata_block_data_size ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: identifier data offset value out of bounds.",
			 function );

			return( -1 );
		}
		if( ( metadata_block_data_offset + identifier_data_offset + metadata_value->identifier_data_size ) > metadata_block_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: identifier data size value out of bounds.",
			 function );

			return( -1 );
		}
		metadata_value->identifier_data = &( metadata_block_data[ metadata_block_data_offset + identifier_data_offset ] );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: level: %d identifier data:\n",
			 function,
			 level );
			libcnotify_print_data(
			 metadata_value->identifier_data,
			 (size_t) metadata_value->identifier_data_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif
		if( ( data_offset < 16 )
		 || ( ( metadata_block_data_offset + data_offset ) > metadata_block_data_size ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: data offset value out of bounds.",
			 function );

			return( -1 );
		}
		if( ( metadata_block_data_offset + data_offset + metadata_value->data_size ) > metadata_block_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: data size value out of bounds.",
			 function );

			return( -1 );
		}
		metadata_value->data = &( metadata_block_data[ metadata_block_data_offset + data_offset ] );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: level: %d data:\n",
			 function,
			 level );
			libcnotify_print_data(
			 metadata_value->data,
			 (size_t) metadata_value->data_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif
	}
	return( 1 );
}

