/*
 * Block reference functions
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

#include "libfsrefs_block_reference.h"
#include "libfsrefs_io_handle.h"
#include "libfsrefs_libcerror.h"
#include "libfsrefs_libcnotify.h"

#include "fsrefs_metadata_block.h"

/* Creates a block reference
 * Make sure the value block_reference is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_block_reference_initialize(
     libfsrefs_block_reference_t **block_reference,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_block_reference_initialize";

	if( block_reference == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block reference.",
		 function );

		return( -1 );
	}
	if( *block_reference != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid block reference value already set.",
		 function );

		return( -1 );
	}
	*block_reference = memory_allocate_structure(
	                     libfsrefs_block_reference_t );

	if( *block_reference == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create block reference.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *block_reference,
	     0,
	     sizeof( libfsrefs_block_reference_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear block reference.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *block_reference != NULL )
	{
		memory_free(
		 *block_reference );

		*block_reference = NULL;
	}
	return( -1 );
}

/* Frees a block reference
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_block_reference_free(
     libfsrefs_block_reference_t **block_reference,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_block_reference_free";

	if( block_reference == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block reference.",
		 function );

		return( -1 );
	}
	if( *block_reference != NULL )
	{
		memory_free(
		 *block_reference );

		*block_reference = NULL;
	}
	return( 1 );
}

/* Reads a block reference
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_block_reference_read_data(
     libfsrefs_block_reference_t *block_reference,
     libfsrefs_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function        = "libfsrefs_block_reference_read_data";
	size_t block_reference_size  = 0;
	uint16_t checksum_data_size  = 0;
	uint8_t checksum_data_offset = 0;
	uint8_t checksum_type        = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	size_t data_offset           = 0;
	uint16_t value_16bit         = 0;
#endif

	if( block_reference == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block reference.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( io_handle->major_format_version == 1 )
	{
		block_reference_size = sizeof( fsrefs_metadata_block_reference_v1_t );
	}
	else if( io_handle->major_format_version == 3 )
	{
		block_reference_size = sizeof( fsrefs_metadata_block_reference_v3_t );
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format version: %" PRIu8 ".%" PRIu8 ".",
		 function,
		 io_handle->major_format_version,
		 io_handle->minor_format_version );

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
	if( ( data_size < block_reference_size )
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
		 "%s: block reference data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 block_reference_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( io_handle->major_format_version == 1 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_metadata_block_reference_v1_t *) data )->block_number,
		 block_reference->block_numbers[ 0 ] );

		checksum_type = ( (fsrefs_metadata_block_reference_v1_t *) data )->checksum_type;

		checksum_data_offset = ( (fsrefs_metadata_block_reference_v1_t *) data )->checksum_data_offset;

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsrefs_metadata_block_reference_v1_t *) data )->checksum_data_size,
		 checksum_data_size );
	}
	else if( io_handle->major_format_version == 3 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_metadata_block_reference_v3_t *) data )->block_number1,
		 block_reference->block_numbers[ 0 ] );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_metadata_block_reference_v3_t *) data )->block_number2,
		 block_reference->block_numbers[ 1 ] );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_metadata_block_reference_v3_t *) data )->block_number3,
		 block_reference->block_numbers[ 2 ] );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_metadata_block_reference_v3_t *) data )->block_number4,
		 block_reference->block_numbers[ 3 ] );

		checksum_type = ( (fsrefs_metadata_block_reference_v3_t *) data )->checksum_type;

		checksum_data_offset = ( (fsrefs_metadata_block_reference_v3_t *) data )->checksum_data_offset;

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsrefs_metadata_block_reference_v3_t *) data )->checksum_data_size,
		 checksum_data_size );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( io_handle->major_format_version == 1 )
		{
			libcnotify_printf(
			 "%s: block number\t\t\t: %" PRIu64 "\n",
			 function,
			 block_reference->block_numbers[ 0 ] );
		}
		else if( io_handle->major_format_version == 3 )
		{
			libcnotify_printf(
			 "%s: block number1\t\t\t: %" PRIu64 "\n",
			 function,
			 block_reference->block_numbers[ 0 ] );

			libcnotify_printf(
			 "%s: block number2\t\t\t: %" PRIu64 "\n",
			 function,
			 block_reference->block_numbers[ 1 ] );

			libcnotify_printf(
			 "%s: block number3\t\t\t: %" PRIu64 "\n",
			 function,
			 block_reference->block_numbers[ 2 ] );

			libcnotify_printf(
			 "%s: block number4\t\t\t: %" PRIu64 "\n",
			 function,
			 block_reference->block_numbers[ 3 ] );
		}
		if( io_handle->major_format_version == 1 )
		{
			byte_stream_copy_to_uint16_little_endian(
			 ( (fsrefs_metadata_block_reference_v1_t *) data )->unknown1,
			 value_16bit );
		}
		else if( io_handle->major_format_version == 3 )
		{
			byte_stream_copy_to_uint16_little_endian(
			 ( (fsrefs_metadata_block_reference_v3_t *) data )->unknown1,
			 value_16bit );
		}
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: checksum type\t\t\t: %" PRIu8 "\n",
		 function,
		 checksum_type );

		libcnotify_printf(
		 "%s: checksum data offset\t\t: %" PRIu8 "\n",
		 function,
		 checksum_data_offset );

		libcnotify_printf(
		 "%s: checksum data size\t\t\t: %" PRIu16 "\n",
		 function,
		 checksum_data_size );

		if( io_handle->major_format_version == 1 )
		{
			byte_stream_copy_to_uint16_little_endian(
			 ( (fsrefs_metadata_block_reference_v1_t *) data )->unknown2,
			 value_16bit );
		}
		else if( io_handle->major_format_version == 3 )
		{
			byte_stream_copy_to_uint16_little_endian(
			 ( (fsrefs_metadata_block_reference_v3_t *) data )->unknown2,
			 value_16bit );
		}
		libcnotify_printf(
		 "%s: unknown2\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( ( checksum_type != 1 )
	 && ( checksum_type != 2 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported checksum type: %" PRIu8 ".",
		 function,
		 checksum_type );

		return( -1 );
	}
/* TODO validate checksum data offset */
/* TODO validate checksum data size */

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( io_handle->major_format_version == 1 )
		{
			data_offset = 8 + checksum_data_offset;
		}
		else if( io_handle->major_format_version == 3 )
		{
			data_offset = 32 + checksum_data_offset;
		}
		libcnotify_printf(
		 "%s: checksum data:\n",
		 function );
		libcnotify_print_data(
		 &( data[ data_offset ] ),
		 checksum_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

/* TODO
		data_offset += checksum_data_size;

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
*/
	}
#endif
	return( 1 );
}

