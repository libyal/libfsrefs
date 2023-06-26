/*
 * Checkpoint (or level 1 metadata) functions
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
#include "libfsrefs_checkpoint.h"
#include "libfsrefs_io_handle.h"
#include "libfsrefs_libbfio.h"
#include "libfsrefs_libcdata.h"
#include "libfsrefs_libcerror.h"
#include "libfsrefs_libcnotify.h"
#include "libfsrefs_metadata_block_header.h"

#include "fsrefs_checkpoint.h"
#include "fsrefs_metadata_block.h"

/* Creates checkpoint
 * Make sure the value checkpoint is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_checkpoint_initialize(
     libfsrefs_checkpoint_t **checkpoint,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_checkpoint_initialize";

	if( checkpoint == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid checkpoint.",
		 function );

		return( -1 );
	}
	if( *checkpoint != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid checkpoint value already set.",
		 function );

		return( -1 );
	}
	*checkpoint = memory_allocate_structure(
	               libfsrefs_checkpoint_t );

	if( *checkpoint == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create checkpoint.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *checkpoint,
	     0,
	     sizeof( libfsrefs_checkpoint_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear checkpoint.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( ( *checkpoint )->level2_metadata_block_descriptors_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create level 2 metadata block descriptors array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *checkpoint != NULL )
	{
		memory_free(
		 *checkpoint );

		*checkpoint = NULL;
	}
	return( -1 );
}

/* Frees checkpoint
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_checkpoint_free(
     libfsrefs_checkpoint_t **checkpoint,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_checkpoint_free";
	int result            = 1;

	if( checkpoint == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid checkpoint.",
		 function );

		return( -1 );
	}
	if( *checkpoint != NULL )
	{
		if( libcdata_array_free(
		     &( ( *checkpoint )->level2_metadata_block_descriptors_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfsrefs_block_descriptor_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free level 2 metadata block descriptors array.",
			 function );

			result = -1;
		}
		memory_free(
		 *checkpoint );

		*checkpoint = NULL;
	}
	return( result );
}

/* Reads a checkpoint
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_checkpoint_read_data(
     libfsrefs_checkpoint_t *checkpoint,
     libfsrefs_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfsrefs_block_descriptor_t *block_descriptor = NULL;
	static char *function                          = "libfsrefs_checkpoint_read_data";
	size_t data_offset                             = 0;
	size_t header_size                             = 0;
	size_t trailer_size                            = 0;
	uint32_t descriptor_offset                     = 0;
	uint32_t number_of_offsets                     = 0;
	uint32_t offset_index                          = 0;
	uint32_t offsets_data_offset                   = 0;
	uint32_t self_reference_data_offset            = 0;
	uint32_t self_reference_data_size              = 0;
	int entry_index                                = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit                           = 0;
	uint32_t value_32bit                           = 0;
	uint16_t value_16bit                           = 0;
#endif

	if( checkpoint == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid checkpoint.",
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
		header_size  = sizeof( fsrefs_metadata_block_header_v1_t );
		trailer_size = sizeof( fsrefs_checkpoint_trailer_v1_t );
	}
	else if( io_handle->major_format_version == 3 )
	{
		header_size  = sizeof( fsrefs_metadata_block_header_v3_t );
		trailer_size = sizeof( fsrefs_checkpoint_trailer_v3_t );
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
	if( ( data_size < sizeof( fsrefs_checkpoint_header_t ) )
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
		 "%s: checkpoint header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsrefs_checkpoint_header_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsrefs_checkpoint_header_t *) data )->self_reference_data_offset,
	 self_reference_data_offset );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsrefs_checkpoint_header_t *) data )->self_reference_data_size,
	 self_reference_data_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (fsrefs_checkpoint_header_t *) data )->unknown1,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsrefs_checkpoint_header_t *) data )->major_format_version,
		 value_16bit );
		libcnotify_printf(
		 "%s: major format version\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsrefs_checkpoint_header_t *) data )->minor_format_version,
		 value_16bit );
		libcnotify_printf(
		 "%s: minor format version\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

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

	data_offset = sizeof( fsrefs_checkpoint_header_t );

	if( ( self_reference_data_offset < ( data_offset + header_size ) )
	 || ( self_reference_data_offset >= io_handle->metadata_block_size ) )
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

	if( ( data_size - data_offset ) < trailer_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: checkpoint trailer data:\n",
		 function );
		libcnotify_print_data(
		 &( data[ data_offset ] ),
		 trailer_size,
		 0 );
	}
#endif
	if( io_handle->major_format_version == 1 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (fsrefs_checkpoint_trailer_v1_t *) &( data[ data_offset ] ) )->number_of_offsets,
		 number_of_offsets );
	}
	else if( io_handle->major_format_version == 3 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (fsrefs_checkpoint_trailer_v3_t *) &( data[ data_offset ] ) )->number_of_offsets,
		 number_of_offsets );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( io_handle->major_format_version == 1 )
		{
			byte_stream_copy_to_uint64_little_endian(
			 ( (fsrefs_checkpoint_trailer_v1_t *) &( data[ data_offset ] ) )->unknown2,
			 value_64bit );
			libcnotify_printf(
			 "%s: unknown2\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (fsrefs_checkpoint_trailer_v1_t *) &( data[ data_offset ] ) )->unknown3,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown3\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (fsrefs_checkpoint_trailer_v1_t *) &( data[ data_offset ] ) )->unknown4,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown4\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint64_little_endian(
			 ( (fsrefs_checkpoint_trailer_v1_t *) &( data[ data_offset ] ) )->unknown5,
			 value_64bit );
			libcnotify_printf(
			 "%s: unknown5\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );
		}
		else if( io_handle->major_format_version == 3 )
		{
			byte_stream_copy_to_uint64_little_endian(
			 ( (fsrefs_checkpoint_trailer_v3_t *) &( data[ data_offset ] ) )->unknown2,
			 value_64bit );
			libcnotify_printf(
			 "%s: unknown2\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 ( (fsrefs_checkpoint_trailer_v3_t *) &( data[ data_offset ] ) )->unknown3,
			 value_64bit );
			libcnotify_printf(
			 "%s: unknown3\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (fsrefs_checkpoint_trailer_v3_t *) &( data[ data_offset ] ) )->unknown4,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown4\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (fsrefs_checkpoint_trailer_v3_t *) &( data[ data_offset ] ) )->unknown5,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown5\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint64_little_endian(
			 ( (fsrefs_checkpoint_trailer_v3_t *) &( data[ data_offset ] ) )->unknown6,
			 value_64bit );
			libcnotify_printf(
			 "%s: unknown6\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 ( (fsrefs_checkpoint_trailer_v3_t *) &( data[ data_offset ] ) )->unknown7,
			 value_64bit );
			libcnotify_printf(
			 "%s: unknown7\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (fsrefs_checkpoint_trailer_v3_t *) &( data[ data_offset ] ) )->unknown8,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown8\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (fsrefs_checkpoint_trailer_v3_t *) &( data[ data_offset ] ) )->unknown9,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown9\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );
		}
		libcnotify_printf(
		 "%s: number of offsets\t\t\t: %" PRIu32 "\n",
		 function,
		 number_of_offsets );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	data_offset += trailer_size;

	if( ( ( data_size - data_offset ) / 4 ) < number_of_offsets )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of offsets value out of bounds.",
		 function );

		goto on_error;
	}
	offsets_data_offset = data_offset;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: offsets data:\n",
		 function );
		libcnotify_print_data(
		 &( data[ data_offset ] ),
		 number_of_offsets * 4,
		 0 );
	}
#endif
	data_offset += number_of_offsets * 4;

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

	for( offset_index = 0;
	     offset_index < number_of_offsets;
	     offset_index++ )
	{
		byte_stream_copy_to_uint32_little_endian(
		 &( data[ offsets_data_offset ] ),
		 descriptor_offset );

		offsets_data_offset += 4;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: descriptor: %02" PRIu32 " offset\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 offset_index,
			 descriptor_offset );
		}
#endif
		if( ( descriptor_offset < ( data_offset + header_size ) )
		 || ( descriptor_offset >= io_handle->metadata_block_size ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid descriptor offset value out of bounds.",
			 function );

			goto on_error;
		}
		descriptor_offset -= header_size;

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
		     &( data[ descriptor_offset ] ),
		     data_size - descriptor_offset,
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
		if( libcdata_array_append_entry(
		     checkpoint->level2_metadata_block_descriptors_array,
		     &entry_index,
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
	return( 1 );

on_error:
	if( block_descriptor != NULL )
	{
		libfsrefs_block_descriptor_free(
		 &block_descriptor,
		 NULL );
	}
	return( -1 );
}

/* Reads a checkpoint
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_checkpoint_read_file_io_handle(
     libfsrefs_checkpoint_t *checkpoint,
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	libfsrefs_metadata_block_header_t *metadata_block_header = NULL;
	uint8_t *metadata_block_data                             = NULL;
	static char *function                                    = "libfsrefs_checkpoint_read_file_io_handle";
	size_t header_size                                       = 0;
	ssize_t read_count                                       = 0;

	if( checkpoint == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid checkpoint.",
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
		 "%s: unable to read checkpoint metadata at offset: %" PRIi64 " (0x%08" PRIx64 ").",
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
		     "CHKP",
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
	if( libfsrefs_checkpoint_read_data(
	     checkpoint,
	     io_handle,
	     &( metadata_block_data[ header_size ] ),
	     io_handle->metadata_block_size - header_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read checkpoint metadata.",
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

/* Retrieves the number of level 2 metadata block descriptors
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_checkpoint_get_number_of_level2_metadata_block_descriptors(
     libfsrefs_checkpoint_t *checkpoint,
     int *number_of_block_descriptors,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_checkpoint_get_number_of_level2_metadata_block_descriptors";

	if( checkpoint == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid checkpoint.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     checkpoint->level2_metadata_block_descriptors_array,
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

/* Retrieves a specific level 2 metadata block descriptor
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_checkpoint_get_level2_metadata_block_descriptor_by_index(
     libfsrefs_checkpoint_t *checkpoint,
     int block_descriptor_index,
     libfsrefs_block_descriptor_t **block_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_checkpoint_get_level2_metadata_block_descriptor_by_index";

	if( checkpoint == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid checkpoint.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     checkpoint->level2_metadata_block_descriptors_array,
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
