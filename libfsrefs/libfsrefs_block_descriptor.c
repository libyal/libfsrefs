/*
 * Block descriptor functions
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

#include "libfsrefs_block_descriptor.h"
#include "libfsrefs_io_handle.h"
#include "libfsrefs_libcerror.h"
#include "libfsrefs_libcnotify.h"

#include "fsrefs_metadata_block.h"

/* Creates a block descriptor
 * Make sure the value block_descriptor is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_block_descriptor_initialize(
     libfsrefs_block_descriptor_t **block_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_block_descriptor_initialize";

	if( block_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block descriptor.",
		 function );

		return( -1 );
	}
	if( *block_descriptor != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid block descriptor value already set.",
		 function );

		return( -1 );
	}
	*block_descriptor = memory_allocate_structure(
	                     libfsrefs_block_descriptor_t );

	if( *block_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create block descriptor.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *block_descriptor,
	     0,
	     sizeof( libfsrefs_block_descriptor_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear block descriptor.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *block_descriptor != NULL )
	{
		memory_free(
		 *block_descriptor );

		*block_descriptor = NULL;
	}
	return( -1 );
}

/* Frees a block descriptor
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_block_descriptor_free(
     libfsrefs_block_descriptor_t **block_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_block_descriptor_free";

	if( block_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block descriptor.",
		 function );

		return( -1 );
	}
	if( *block_descriptor != NULL )
	{
		memory_free(
		 *block_descriptor );

		*block_descriptor = NULL;
	}
	return( 1 );
}

/* Reads a block descriptor
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_block_descriptor_read_data(
     libfsrefs_block_descriptor_t *block_descriptor,
     libfsrefs_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function        = "libfsrefs_block_descriptor_read_data";
	size_t block_descriptor_size = 0;
	size_t data_offset           = 0;
	uint16_t checksum_data_size  = 0;
	uint8_t checksum_data_offset = 0;
	uint8_t checksum_type        = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint16_t value_16bit         = 0;
#endif

	if( block_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block descriptor.",
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
		block_descriptor_size = sizeof( fsrefs_metadata_block_descriptor_v1_t );
	}
	else if( io_handle->major_format_version == 3 )
	{
		block_descriptor_size = sizeof( fsrefs_metadata_block_descriptor_v3_t );
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
	if( ( data_size < block_descriptor_size )
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
		 "%s: block descriptor data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 block_descriptor_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( io_handle->major_format_version == 1 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_metadata_block_descriptor_v1_t *) data )->block_number,
		 block_descriptor->block_number1 );

		checksum_type = ( (fsrefs_metadata_block_descriptor_v1_t *) data )->checksum_type;

		checksum_data_offset = ( (fsrefs_metadata_block_descriptor_v1_t *) data )->checksum_data_offset;

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsrefs_metadata_block_descriptor_v1_t *) data )->checksum_data_size,
		 checksum_data_size );
	}
	else if( io_handle->major_format_version == 3 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_metadata_block_descriptor_v3_t *) data )->block_number1,
		 block_descriptor->block_number1 );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_metadata_block_descriptor_v3_t *) data )->block_number2,
		 block_descriptor->block_number2 );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_metadata_block_descriptor_v3_t *) data )->block_number3,
		 block_descriptor->block_number3 );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_metadata_block_descriptor_v3_t *) data )->block_number4,
		 block_descriptor->block_number4 );

		checksum_type = ( (fsrefs_metadata_block_descriptor_v3_t *) data )->checksum_type;

		checksum_data_offset = ( (fsrefs_metadata_block_descriptor_v3_t *) data )->checksum_data_offset;

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsrefs_metadata_block_descriptor_v3_t *) data )->checksum_data_size,
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
			 block_descriptor->block_number1 );
		}
		else if( io_handle->major_format_version == 3 )
		{
			libcnotify_printf(
			 "%s: block number1\t\t\t: %" PRIu64 "\n",
			 function,
			 block_descriptor->block_number1 );

			libcnotify_printf(
			 "%s: block number2\t\t\t: %" PRIu64 "\n",
			 function,
			 block_descriptor->block_number2 );

			libcnotify_printf(
			 "%s: block number3\t\t\t: %" PRIu64 "\n",
			 function,
			 block_descriptor->block_number3 );

			libcnotify_printf(
			 "%s: block number4\t\t\t: %" PRIu64 "\n",
			 function,
			 block_descriptor->block_number4 );
		}
		if( io_handle->major_format_version == 1 )
		{
			byte_stream_copy_to_uint16_little_endian(
			 ( (fsrefs_metadata_block_descriptor_v1_t *) data )->unknown1,
			 value_16bit );
		}
		else if( io_handle->major_format_version == 3 )
		{
			byte_stream_copy_to_uint16_little_endian(
			 ( (fsrefs_metadata_block_descriptor_v3_t *) data )->unknown1,
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
		 "%s: checksum data size\t\t: %" PRIu16 "\n",
		 function,
		 checksum_data_size );

		if( io_handle->major_format_version == 1 )
		{
			byte_stream_copy_to_uint16_little_endian(
			 ( (fsrefs_metadata_block_descriptor_v1_t *) data )->unknown2,
			 value_16bit );
		}
		else if( io_handle->major_format_version == 3 )
		{
			byte_stream_copy_to_uint16_little_endian(
			 ( (fsrefs_metadata_block_descriptor_v3_t *) data )->unknown2,
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

	if( io_handle->major_format_version == 1 )
	{
		data_offset = 8 + checksum_data_offset;
	}
	else if( io_handle->major_format_version == 3 )
	{
		data_offset = 32 + checksum_data_offset;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: checksum data:\n",
		 function );
		libcnotify_print_data(
		 &( data[ data_offset ] ),
		 checksum_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
/* TODO
	data_offset += checksum_data_size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
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
#endif
*/
	return( 1 );
}

