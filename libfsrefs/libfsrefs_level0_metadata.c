/*
 * Level 0 metadata functions
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

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <system_string.h>
#include <types.h>

#include "libfsrefs_io_handle.h"
#include "libfsrefs_libbfio.h"
#include "libfsrefs_libcerror.h"
#include "libfsrefs_libcnotify.h"
#include "libfsrefs_libfguid.h"
#include "libfsrefs_level0_metadata.h"
#include "libfsrefs_metadata_block.h"

/* Creates level 0 metadata
 * Make sure the value level0_metadata is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_level0_metadata_initialize(
     libfsrefs_level0_metadata_t **level0_metadata,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_level0_metadata_initialize";

	if( level0_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid level 0 metadata.",
		 function );

		return( -1 );
	}
	if( *level0_metadata != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid level 0 metadata value already set.",
		 function );

		return( -1 );
	}
	*level0_metadata = memory_allocate_structure(
	                    libfsrefs_level0_metadata_t );

	if( *level0_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create level 0 metadata.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *level0_metadata,
	     0,
	     sizeof( libfsrefs_level0_metadata_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear level 0 metadata.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *level0_metadata != NULL )
	{
		memory_free(
		 *level0_metadata );

		*level0_metadata = NULL;
	}
	return( -1 );
}

/* Frees level 0 metadata
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_level0_metadata_free(
     libfsrefs_level0_metadata_t **level0_metadata,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_level0_metadata_free";

	if( level0_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid level 0 metadata.",
		 function );

		return( -1 );
	}
	if( *level0_metadata != NULL )
	{
		memory_free(
		 *level0_metadata );

		*level0_metadata = NULL;
	}
	return( 1 );
}

/* Reads the level 0 metadata
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_level0_metadata_read(
     libfsrefs_level0_metadata_t *level0_metadata,
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	libfsrefs_metadata_block_t *metadata_block = NULL;
	static char *function                      = "libfsrefs_level0_metadata_read";

#if defined( HAVE_DEBUG_OUTPUT )
	system_character_t guid_string[ 48 ];

        libfguid_identifier_t *guid                = NULL;
	uint64_t value_64bit                       = 0;
	uint32_t value_32bit                       = 0;
	int result                                 = 0;
#endif

	if( level0_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid level 0 metadata.",
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
	     0,
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
	if( metadata_block->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid metadata block - missing data.",
		 function );

		goto on_error;
	}
	byte_stream_copy_to_uint64_little_endian(
	 &( metadata_block->data[ 160 ] ),
	 level0_metadata->primary_level1_metadata_block_number );

	byte_stream_copy_to_uint64_little_endian(
	 &(  metadata_block->data[ 168 ] ),
	 level0_metadata->secondary_level1_metadata_block_number );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libfguid_identifier_initialize(
		     &guid,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create GUID.",
			 function );

			goto on_error;
		}
		if( libfguid_identifier_copy_from_byte_stream(
		     guid,
		     &(  metadata_block->data[ 48 ] ),
		     16,
		     LIBFGUID_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy byte stream to GUID.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfguid_identifier_copy_to_utf16_string(
			  guid,
			  (uint16_t *) guid_string,
			  48,
			  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
			  error );
#else
		result = libfguid_identifier_copy_to_utf8_string(
			  guid,
			  (uint8_t *) guid_string,
			  48,
			  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy GUID to string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: identifier\t\t\t\t: %" PRIs_SYSTEM "\n",
		 function,
		 guid_string );

		if( libfguid_identifier_free(
		     &guid,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free GUID.",
			 function );

			goto on_error;
		}
		byte_stream_copy_to_uint64_little_endian(
		 &(  metadata_block->data[ 64 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 &(  metadata_block->data[ 72 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown2\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint32_little_endian(
		 &(  metadata_block->data[ 80 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown3\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 &(  metadata_block->data[ 84 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown4\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 &(  metadata_block->data[ 88 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown5\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 &(  metadata_block->data[ 92 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown6\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: unknown7\n",
		 function );
		libcnotify_print_data(
		 &(  metadata_block->data[ 96 ] ),
		 64,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: primary level 1 metadata block number\t: %" PRIu64 "\n",
		 function,
		 level0_metadata->primary_level1_metadata_block_number );

		libcnotify_printf(
		 "%s: secondary level 1 metadata block number\t: %" PRIu64 "\n",
		 function,
		 level0_metadata->secondary_level1_metadata_block_number );

		libcnotify_printf(
		 "\n" );
	}
#endif

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 &(  metadata_block->data[ 176 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: block number\t\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 &(  metadata_block->data[ 184 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown9\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 &(  metadata_block->data[ 192 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown10\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "\n" );
	}
#endif
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( guid != NULL )
	{
		libfguid_identifier_free(
		 &guid,
		 NULL );
	}
#endif
	if( metadata_block != NULL )
	{
		libfsrefs_metadata_block_free(
		 &metadata_block,
		 NULL );
	}
	return( -1 );
}

