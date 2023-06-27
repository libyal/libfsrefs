/*
 * Superblock (or level 0 metadata) functions
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
#include "libfsrefs_debug.h"
#include "libfsrefs_io_handle.h"
#include "libfsrefs_libbfio.h"
#include "libfsrefs_libcerror.h"
#include "libfsrefs_libcnotify.h"
#include "libfsrefs_libfguid.h"
#include "libfsrefs_metadata_block_header.h"
#include "libfsrefs_superblock.h"

#include "fsrefs_metadata_block.h"
#include "fsrefs_superblock.h"

/* Creates a superblock
 * Make sure the value superblock is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_superblock_initialize(
     libfsrefs_superblock_t **superblock,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_superblock_initialize";

	if( superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid superblock.",
		 function );

		return( -1 );
	}
	if( *superblock != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid superblock value already set.",
		 function );

		return( -1 );
	}
	*superblock = memory_allocate_structure(
	               libfsrefs_superblock_t );

	if( *superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create superblock.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *superblock,
	     0,
	     sizeof( libfsrefs_superblock_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear superblock.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *superblock != NULL )
	{
		memory_free(
		 *superblock );

		*superblock = NULL;
	}
	return( -1 );
}

/* Frees a uperblock
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_superblock_free(
     libfsrefs_superblock_t **superblock,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_superblock_free";

	if( superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid superblock.",
		 function );

		return( -1 );
	}
	if( *superblock != NULL )
	{
		memory_free(
		 *superblock );

		*superblock = NULL;
	}
	return( 1 );
}

/* Reads a superblock
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_superblock_read_data(
     libfsrefs_superblock_t *superblock,
     libfsrefs_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function                          = "libfsrefs_superblock_read_data";
	size_t data_offset                             = 0;
	size_t header_size                             = 0;
	uint32_t checkpoints_data_offset               = 0;
	uint32_t number_of_checkpoints                 = 0;
	uint32_t self_reference_data_offset            = 0;
	uint32_t self_reference_data_size              = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	libfsrefs_block_descriptor_t *block_descriptor = NULL;
	uint64_t value_64bit                           = 0;
#endif

	if( superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid superblock.",
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
	if( ( data_size < sizeof( fsrefs_superblock_t ) )
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
		 "%s: superblock data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsrefs_superblock_t ),
		 0 );
	}
#endif
	if( memory_copy(
	     superblock->volume_identifier,
	     ( (fsrefs_superblock_t *) data )->volume_identifier,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy volume identifier.",
		 function );

		goto on_error;
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsrefs_superblock_t *) data )->checkpoints_data_offset,
	 checkpoints_data_offset );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsrefs_superblock_t *) data )->number_of_checkpoints,
	 number_of_checkpoints );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsrefs_superblock_t *) data )->self_reference_data_offset,
	 self_reference_data_offset );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsrefs_superblock_t *) data )->self_reference_data_size,
	 self_reference_data_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libfsrefs_debug_print_guid_value(
		     function,
		     "volume identifier\t\t\t",
		     ( (fsrefs_superblock_t *) data )->volume_identifier,
		     16,
		     LIBFGUID_ENDIAN_LITTLE,
		     LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print GUID value.",
			 function );

			goto on_error;
		}
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_superblock_t *) data )->unknown1,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_superblock_t *) data )->unknown2,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown2\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "%s: checkpoints data offset\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 checkpoints_data_offset );

		libcnotify_printf(
		 "%s: number of checkpoints\t\t\t: %" PRIu32 "\n",
		 function,
		 number_of_checkpoints );

		libcnotify_printf(
		 "%s: self reference data offset\t\t: 0x%08" PRIx32 "\n",
		 function,
		 self_reference_data_offset );

		libcnotify_printf(
		 "%s: self reference data size\t\t: %" PRIu32 "\n",
		 function,
		 self_reference_data_size );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	data_offset = sizeof( fsrefs_superblock_t );

	if( ( checkpoints_data_offset < ( data_offset + header_size ) )
	 || ( checkpoints_data_offset >= ( data_size + header_size ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid checkpoints data offset value out of bounds.",
		 function );

		goto on_error;
	}
	if( number_of_checkpoints != 2 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported number of checkpoints.",
		 function );

		goto on_error;
	}
	checkpoints_data_offset -= header_size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( checkpoints_data_offset > data_offset )
		{
			libcnotify_printf(
			 "%s: unknown3\n",
			 function );
			libcnotify_print_data(
			 &( data[ data_offset ] ),
			 (size_t) checkpoints_data_offset - data_offset,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	data_offset = checkpoints_data_offset;

	if( ( data_size - data_offset ) < 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid checkpoints data size value out of bounds.",
		 function );

		goto on_error;
	}
	byte_stream_copy_to_uint64_little_endian(
	 &( data[ data_offset ] ),
	 superblock->primary_checkpoint_block_number );

	data_offset += 8;

	byte_stream_copy_to_uint64_little_endian(
	 &( data[ data_offset ] ),
	 superblock->secondary_checkpoint_block_number );

	data_offset += 8;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: primary checkpoint block number\t\t: %" PRIu64 "\n",
		 function,
		 superblock->primary_checkpoint_block_number );

		libcnotify_printf(
		 "%s: secondary checkpoint block number\t: %" PRIu64 "\n",
		 function,
		 superblock->secondary_checkpoint_block_number );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( ( self_reference_data_offset < ( data_offset + header_size ) )
	 || ( self_reference_data_offset >= ( data_size + header_size ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid self reference data offset value out of bounds.",
		 function );

		goto on_error;
	}
	self_reference_data_offset -= header_size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( self_reference_data_offset > data_offset )
		{
			libcnotify_printf(
			 "%s: unknown4\n",
			 function );
			libcnotify_print_data(
			 &( data[ data_offset ] ),
			 (size_t) self_reference_data_offset - data_offset,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	data_offset = self_reference_data_offset;

	if( ( data_size - data_offset ) < self_reference_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid self reference data size value out of bounds.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: self reference data\n",
		 function );
		libcnotify_print_data(
		 &( data[ data_offset ] ),
		 self_reference_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

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
		if( libfsrefs_block_descriptor_read_data(
		     block_descriptor,
		     io_handle,
		     &( data[ data_offset ] ),
		     self_reference_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read block descriptor.",
			 function );

			goto on_error;
		}
		if( libfsrefs_block_descriptor_free(
		     &block_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free block descriptor.",
			 function );

			goto on_error;
		}
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );

on_error:
#if defined( HAVE_DEBUG_OUTPUT )
	if( block_descriptor != NULL )
	{
		libfsrefs_block_descriptor_free(
		 &block_descriptor,
		 NULL );
	}
#endif
	return( -1 );
}

/* Reads a superblock
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_superblock_read_file_io_handle(
     libfsrefs_superblock_t *superblock,
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	libfsrefs_metadata_block_header_t *metadata_block_header = NULL;
	uint8_t *metadata_block_data                             = NULL;
	static char *function                                    = "libfsrefs_superblock_read_file_io_handle";
	size_t header_size                                       = 0;
	ssize_t read_count                                       = 0;

	if( superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid superblock.",
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
	if( ( io_handle->metadata_block_size < header_size )
	 || ( io_handle->metadata_block_size > MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid IO handle - metadata block size value out of bounds.",
		 function );

		return( -1 );
	}
	metadata_block_data = (uint8_t *) memory_allocate(
	                                   io_handle->metadata_block_size );

	if( metadata_block_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create metadata block data.",
		 function );

		goto on_error;
	}
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              metadata_block_data,
	              (size_t) io_handle->metadata_block_size,
	              file_offset,
	              error );

	if( read_count != (ssize_t) io_handle->metadata_block_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read superblock metadata at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		goto on_error;
	}
	if( libfsrefs_metadata_block_header_initialize(
	     &metadata_block_header,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create metadata block header.",
		 function );

		goto on_error;
	}
	if( libfsrefs_metadata_block_header_read_data(
	     metadata_block_header,
	     io_handle,
	     metadata_block_data,
	     header_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read metadata block header at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		goto on_error;
	}
	file_offset += header_size;

	if( io_handle->major_format_version == 3 )
	{
		if( memory_compare(
		     metadata_block_header->signature,
		     "SUPB",
		     4 ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid metadata block signature.",
			 function );

			goto on_error;
		}
		if( metadata_block_header->block_number2 != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid metadata block header - block number 2 value out of bounds.",
			 function );

			goto on_error;
		}
		if( metadata_block_header->block_number3 != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid metadata block header - block number 3 value out of bounds.",
			 function );

			goto on_error;
		}
		if( metadata_block_header->block_number4 != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid metadata block header - block number 4 value out of bounds.",
			 function );

			goto on_error;
		}
	}
	if( libfsrefs_metadata_block_header_free(
	     &metadata_block_header,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free metadata block header.",
		 function );

		goto on_error;
	}
	if( libfsrefs_superblock_read_data(
	     superblock,
	     io_handle,
	     &( metadata_block_data[ header_size ] ),
	     io_handle->metadata_block_size - header_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read superblock metadata.",
		 function );

		goto on_error;
	}
	memory_free(
	 metadata_block_data );

	return( 1 );

on_error:
	if( metadata_block_header != NULL )
	{
		libfsrefs_metadata_block_header_free(
		 &metadata_block_header,
		 NULL );
	}
	if( metadata_block_data != NULL )
	{
		memory_free(
		 metadata_block_data );
	}
	return( -1 );
}

