/*
 * Metadata block functions
 *
 * Copyright (C) 2012-2018, Joachim Metz <joachim.metz@gmail.com>
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

#include "libfsrefs_io_handle.h"
#include "libfsrefs_libbfio.h"
#include "libfsrefs_libcerror.h"
#include "libfsrefs_libcnotify.h"
#include "libfsrefs_metadata_block.h"

#include "fsrefs_metadata_block.h"

/* Creates a metadata block
 * Make sure the value metadata_block is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_metadata_block_initialize(
     libfsrefs_metadata_block_t **metadata_block,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_metadata_block_initialize";

	if( metadata_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata block.",
		 function );

		return( -1 );
	}
	if( *metadata_block != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid metadata block value already set.",
		 function );

		return( -1 );
	}
	*metadata_block = memory_allocate_structure(
	                   libfsrefs_metadata_block_t );

	if( *metadata_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create metadata block.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *metadata_block,
	     0,
	     sizeof( libfsrefs_metadata_block_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear metadata block.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *metadata_block != NULL )
	{
		memory_free(
		 *metadata_block );

		*metadata_block = NULL;
	}
	return( -1 );
}

/* Frees a metadata block
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_metadata_block_free(
     libfsrefs_metadata_block_t **metadata_block,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_metadata_block_free";

	if( metadata_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata block.",
		 function );

		return( -1 );
	}
	if( *metadata_block != NULL )
	{
		if( ( *metadata_block )->data != NULL )
		{
			memory_free(
			 ( *metadata_block )->data );
		}
		memory_free(
		 *metadata_block );

		*metadata_block = NULL;
	}
	return( 1 );
}

/* Reads the metadata block
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_metadata_block_read(
     libfsrefs_metadata_block_t *metadata_block,
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     int level,
     libcerror_error_t **error )
{
	uint8_t *metadata_block_data              = NULL;
	static char *function                     = "libfsrefs_metadata_block_read";
	size_t metadata_block_data_size           = 0;
	ssize_t read_count                        = 0;
	uint64_t calculated_metadata_block_number = 0;
	uint64_t metadata_block_number            = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit                      = 0;
#endif

	if( metadata_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata block.",
		 function );

		return( -1 );
	}
	if( metadata_block->data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid metadata block data already set.",
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
	calculated_metadata_block_number = (uint64_t) ( file_offset / io_handle->metadata_block_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading level: %d metadata block: %" PRIu64 " at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 level,
		 calculated_metadata_block_number,
		 file_offset,
		 file_offset );
	}
#endif
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     file_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek metadata block offset: %" PRIi64 ".",
		 function,
		 file_offset );

		goto on_error;
	}
	metadata_block->data = (uint8_t *) memory_allocate(
	                                    io_handle->metadata_block_size );

	if( metadata_block->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create metadata block data.",
		 function );

		goto on_error;
	}
	metadata_block->data_size = io_handle->metadata_block_size;

	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              metadata_block->data,
	              metadata_block->data_size,
	              error );

	if( read_count != (ssize_t) metadata_block->data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read metadata block data.",
		 function );

		goto on_error;
	}
	metadata_block_data      = metadata_block->data;
	metadata_block_data_size = metadata_block->data_size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: level: %d metadata block header data:\n",
		 function,
		 level );
		libcnotify_print_data(
		 metadata_block_data,
		 sizeof( fsrefs_metadata_block_header_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint64_little_endian(
	 ( (fsrefs_metadata_block_header_t *) metadata_block_data )->block_number,
	 metadata_block_number );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsrefs_metadata_block_header_t *) metadata_block_data )->sequence_number,
	 metadata_block->sequence_number );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: level: %d block number\t\t\t: %" PRIu64 "\n",
		 function,
		 level,
		 metadata_block_number );

		libcnotify_printf(
		 "%s: level: %d sequence number\t\t\t: %" PRIu64 "\n",
		 function,
		 level,
		 metadata_block->sequence_number );

		libcnotify_printf(
		 "%s: level: %d object identifier\n",
		 function,
		 level );
		libcnotify_print_data(
		 ( (fsrefs_metadata_block_header_t *) metadata_block_data )->object_identifier,
		 16,
		 0 );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_metadata_block_header_t *) metadata_block_data )->unknown1,
		 value_64bit );
		libcnotify_printf(
		 "%s: level: %d unknown1\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 level,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_metadata_block_header_t *) metadata_block_data )->unknown2,
		 value_64bit );
		libcnotify_printf(
		 "%s: level: %d unknown2\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 level,
		 value_64bit );

		libcnotify_printf(
		 "\n" );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: level: %d metadata block data:\n",
		 function,
		 level );
		libcnotify_print_data(
		 &( metadata_block_data[ sizeof( fsrefs_metadata_block_header_t ) ] ),
		 metadata_block_data_size - sizeof( fsrefs_metadata_block_header_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	return( 1 );

on_error:
	if( metadata_block->data != NULL )
	{
		memory_free(
		 metadata_block->data );

		metadata_block->data = NULL;
	}
	metadata_block->data_size = 0;

	return( -1 );
}

