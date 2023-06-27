/*
 * The REFS metadata block header functions
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

#include "libfsrefs_io_handle.h"
#include "libfsrefs_libbfio.h"
#include "libfsrefs_libcerror.h"
#include "libfsrefs_libcnotify.h"
#include "libfsrefs_metadata_block_header.h"

#include "fsrefs_metadata_block.h"

/* Creates a metadata block header
 * Make sure the value metadata_block_header is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_metadata_block_header_initialize(
     libfsrefs_metadata_block_header_t **metadata_block_header,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_metadata_block_header_initialize";

	if( metadata_block_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata block header.",
		 function );

		return( -1 );
	}
	if( *metadata_block_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid metadata block header value already set.",
		 function );

		return( -1 );
	}
	*metadata_block_header = memory_allocate_structure(
	                          libfsrefs_metadata_block_header_t );

	if( *metadata_block_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create metadata block header.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *metadata_block_header,
	     0,
	     sizeof( libfsrefs_metadata_block_header_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear metadata block header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *metadata_block_header != NULL )
	{
		memory_free(
		 *metadata_block_header );

		*metadata_block_header = NULL;
	}
	return( -1 );
}

/* Frees a metadata block header
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_metadata_block_header_free(
     libfsrefs_metadata_block_header_t **metadata_block_header,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_metadata_block_header_free";

	if( metadata_block_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata block header.",
		 function );

		return( -1 );
	}
	if( *metadata_block_header != NULL )
	{
		memory_free(
		 *metadata_block_header );

		*metadata_block_header = NULL;
	}
	return( 1 );
}

/* Reads the metadata block header
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_metadata_block_header_read_data(
     libfsrefs_metadata_block_header_t *metadata_block_header,
     libfsrefs_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_metadata_block_header_read_data";
	size_t header_size    = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit  = 0;
	uint32_t value_32bit  = 0;
#endif

	if( metadata_block_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata block header.",
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
		header_size = sizeof( fsrefs_metadata_block_header_v1_t );
	}
	else if( io_handle->major_format_version == 3 )
	{
		header_size = sizeof( fsrefs_metadata_block_header_v3_t );
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
	if( ( data_size < header_size )
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
		 "%s: metadata block header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 header_size,
		 0 );
	}
#endif
	if( io_handle->major_format_version == 1 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_metadata_block_header_v1_t *) data )->block_number,
		 metadata_block_header->block_number1 );
	}
	else if( io_handle->major_format_version == 3 )
	{
		if( memory_copy(
		     metadata_block_header->signature,
		     ( (fsrefs_metadata_block_header_v3_t *) data )->signature,
		     4 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy signature.",
			 function );

			return( -1 );
		}
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_metadata_block_header_v3_t *) data )->block_number1,
		 metadata_block_header->block_number1 );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_metadata_block_header_v3_t *) data )->block_number2,
		 metadata_block_header->block_number2 );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_metadata_block_header_v3_t *) data )->block_number3,
		 metadata_block_header->block_number3 );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_metadata_block_header_v3_t *) data )->block_number4,
		 metadata_block_header->block_number4 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( io_handle->major_format_version == 1 )
		{
			libcnotify_printf(
			 "%s: block number\t\t\t: %" PRIu64 "\n",
			 function,
			 metadata_block_header->block_number1 );

			byte_stream_copy_to_uint64_little_endian(
			 ( (fsrefs_metadata_block_header_v1_t *) data )->unknown1,
			 value_64bit );
			libcnotify_printf(
			 "%s: sequence number\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			libcnotify_printf(
			 "%s: object identifier\n",
			 function );
			libcnotify_print_data(
			 ( (fsrefs_metadata_block_header_v1_t *) data )->object_identifier,
			 16,
			 0 );

			byte_stream_copy_to_uint64_little_endian(
			 ( (fsrefs_metadata_block_header_v1_t *) data )->unknown1,
			 value_64bit );
			libcnotify_printf(
			 "%s: unknown1\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 ( (fsrefs_metadata_block_header_v1_t *) data )->unknown2,
			 value_64bit );
			libcnotify_printf(
			 "%s: unknown2\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );

			libcnotify_printf(
			 "\n" );
		}
		else if( io_handle->major_format_version == 3 )
		{
			libcnotify_printf(
			 "%s: signature\t\t\t: %c%c%c%c\n",
			 function,
			 ( (fsrefs_metadata_block_header_v3_t *) data )->signature[ 0 ],
			 ( (fsrefs_metadata_block_header_v3_t *) data )->signature[ 1 ],
			 ( (fsrefs_metadata_block_header_v3_t *) data )->signature[ 2 ],
			 ( (fsrefs_metadata_block_header_v3_t *) data )->signature[ 3 ] );

			byte_stream_copy_to_uint32_little_endian(
			 ( (fsrefs_metadata_block_header_v3_t *) data )->unknown1,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown1\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (fsrefs_metadata_block_header_v3_t *) data )->unknown2,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown2\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (fsrefs_metadata_block_header_v3_t *) data )->unknown3,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown3\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint64_little_endian(
			 ( (fsrefs_metadata_block_header_v3_t *) data )->unknown4,
			 value_64bit );
			libcnotify_printf(
			 "%s: unknown4\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 ( (fsrefs_metadata_block_header_v3_t *) data )->unknown5,
			 value_64bit );
			libcnotify_printf(
			 "%s: unknown5\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );

			libcnotify_printf(
			 "%s: block number1\t\t: %" PRIu64 "\n",
			 function,
			 metadata_block_header->block_number1 );

			libcnotify_printf(
			 "%s: block number2\t\t: %" PRIu64 "\n",
			 function,
			 metadata_block_header->block_number2 );

			libcnotify_printf(
			 "%s: block number3\t\t: %" PRIu64 "\n",
			 function,
			 metadata_block_header->block_number3 );

			libcnotify_printf(
			 "%s: block number4\t\t: %" PRIu64 "\n",
			 function,
			 metadata_block_header->block_number4 );

			byte_stream_copy_to_uint64_little_endian(
			 ( (fsrefs_metadata_block_header_v3_t *) data )->unknown6,
			 value_64bit );
			libcnotify_printf(
			 "%s: unknown6\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 ( (fsrefs_metadata_block_header_v3_t *) data )->unknown7,
			 value_64bit );
			libcnotify_printf(
			 "%s: unknown7\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );

			libcnotify_printf(
			 "\n" );
		}
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

/* Reads the metadata block header
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_metadata_block_header_read_file_io_handle(
     libfsrefs_metadata_block_header_t *metadata_block_header,
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	uint8_t metadata_block_header_data[ sizeof( fsrefs_metadata_block_header_v3_t ) ];

	static char *function = "libfsrefs_metadata_block_header_read_file_io_handle";
	size_t read_size      = 0;
	ssize_t read_count    = 0;

	if( metadata_block_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata block header.",
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
		read_size = sizeof( fsrefs_metadata_block_header_v1_t );
	}
	else if( io_handle->major_format_version == 3 )
	{
		read_size = sizeof( fsrefs_metadata_block_header_v3_t );
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading metadata block header at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              metadata_block_header_data,
	              read_size,
	              file_offset,
	              error );

	if( read_count != (ssize_t) read_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read metadata block header data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	if( libfsrefs_metadata_block_header_read_data(
	     metadata_block_header,
	     io_handle,
	     metadata_block_header_data,
	     read_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read metadata block header data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

