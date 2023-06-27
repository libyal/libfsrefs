/*
 * Ministore tree (or level 2 metadata) functions
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
#include "libfsrefs_libbfio.h"
#include "libfsrefs_libcdata.h"
#include "libfsrefs_libcerror.h"
#include "libfsrefs_libcnotify.h"
#include "libfsrefs_metadata_block.h"
#include "libfsrefs_metadata_block_header.h"
#include "libfsrefs_metadata_value.h"
#include "libfsrefs_ministore_tree.h"
#include "libfsrefs_node_header.h"
#include "libfsrefs_node_record.h"

#include "fsrefs_metadata_block.h"
#include "fsrefs_ministore_tree.h"

/* Creates a ministore tree
 * Make sure the value ministore_tree is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_ministore_tree_initialize(
     libfsrefs_ministore_tree_t **ministore_tree,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_ministore_tree_initialize";

	if( ministore_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid ministore tree.",
		 function );

		return( -1 );
	}
	if( *ministore_tree != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid ministore tree value already set.",
		 function );

		return( -1 );
	}
	*ministore_tree = memory_allocate_structure(
	                   libfsrefs_ministore_tree_t );

	if( *ministore_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create ministore tree.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *ministore_tree,
	     0,
	     sizeof( libfsrefs_ministore_tree_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear ministore tree.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( ( *ministore_tree )->block_descriptors_array ),
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
	if( *ministore_tree != NULL )
	{
		memory_free(
		 *ministore_tree );

		*ministore_tree = NULL;
	}
	return( -1 );
}

/* Frees a ministore tree
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_ministore_tree_free(
     libfsrefs_ministore_tree_t **ministore_tree,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_ministore_tree_free";
	int result            = 1;

	if( ministore_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid ministore tree.",
		 function );

		return( -1 );
	}
	if( *ministore_tree != NULL )
	{
		if( libcdata_array_free(
		     &( ( *ministore_tree )->block_descriptors_array ),
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
		 *ministore_tree );

		*ministore_tree = NULL;
	}
	return( result );
}

/* Reads a ministore tree
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_ministore_tree_read_data(
     libfsrefs_ministore_tree_t *ministore_tree,
     libfsrefs_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfsrefs_node_header_t *node_header = NULL;
	libfsrefs_node_record_t *node_record = NULL;
	static char *function                = "libfsrefs_ministore_tree_read_data";
	size_t data_offset                   = 0;
	size_t header_size                   = 0;
	size_t node_header_data_offset       = 0;
	size_t record_offsets_data_offset    = 0;
	uint32_t information_entry_size      = 0;
	uint32_t record_data_offset          = 0;
	uint32_t record_data_size            = 0;
	uint32_t record_offsets_index        = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	size_t data_area_start_offset        = 0;
#endif

	if( ministore_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid ministore tree.",
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
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( data_size - data_offset ) < 4 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid tree information entry size data size value out of bounds.",
		 function );

		goto on_error;
	}
	byte_stream_copy_to_uint32_little_endian(
	 &( data[ data_offset ] ),
	 information_entry_size );

	data_offset += 4;

	if( ( information_entry_size < 4 )
	 || ( ( data_size - data_offset ) < information_entry_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid tree information entry size value out of bounds.",
		 function );

		goto on_error;
	}
	information_entry_size -= 4;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: tree information entry size\t: %" PRIu32 "\n",
		 function,
		 information_entry_size + 4 );

		libcnotify_printf(
		 "%s: tree information entry data:\n",
		 function );
		libcnotify_print_data(
		 &( data[ data_offset ] ),
		 information_entry_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	data_offset += information_entry_size;

	node_header_data_offset = data_offset;

	if( libfsrefs_node_header_initialize(
	     &node_header,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create node header.",
		 function );

		goto on_error;
	}
	if( libfsrefs_node_header_read_data(
	     node_header,
	     &( data[ data_offset ] ),
	     data_size - data_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read node header.",
		 function );

		goto on_error;
	}
	data_offset += sizeof( fsrefs_ministore_tree_node_header_t );

	if( ( node_header->data_area_start_offset < sizeof( fsrefs_ministore_tree_node_header_t ) )
	 || ( node_header->data_area_start_offset > ( data_size - node_header_data_offset ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid node header - data area start offset value out of bounds.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		data_area_start_offset = node_header_data_offset + node_header->data_area_start_offset;

		if( data_offset < data_area_start_offset )
		{
			libcnotify_printf(
			 "%s: node header trailing data:\n",
			 function );
			libcnotify_print_data(
			 &( data[ data_offset ] ),
			 data_area_start_offset - data_offset,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
	}
#endif
	if( ( node_header->data_area_end_offset < sizeof( fsrefs_ministore_tree_node_header_t ) )
	 || ( node_header->data_area_end_offset > ( data_size - node_header_data_offset ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid node header - data area end offset value out of bounds.",
		 function );

		goto on_error;
	}
	if( ( node_header->record_offsets_start_offset < sizeof( fsrefs_ministore_tree_node_header_t ) )
	 || ( node_header->record_offsets_start_offset > ( data_size - node_header_data_offset ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid node header - record offsets start offset value out of bounds.",
		 function );

		goto on_error;
	}
	if( node_header->number_of_record_offsets > ( ( data_size - node_header_data_offset - node_header->record_offsets_start_offset ) / 4 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid node header - number of record offsets entries value out of bounds.",
		 function );

		goto on_error;
	}
	if( ( node_header->record_offsets_end_offset != 0 )
	 && ( ( node_header->record_offsets_end_offset < sizeof( fsrefs_ministore_tree_node_header_t ) )
	  ||  ( node_header->record_offsets_end_offset > ( data_size - node_header_data_offset ) ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid node header - record offsets end offset value out of bounds.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: ministore tree data:\n",
		 function );
		libcnotify_print_data(
		 &( data[ node_header_data_offset ] ),
		 data_size - node_header_data_offset,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	record_offsets_data_offset = node_header_data_offset + node_header->record_offsets_start_offset;

	for( record_offsets_index = 0;
	     record_offsets_index < node_header->number_of_record_offsets;
	     record_offsets_index++ )
	{
		byte_stream_copy_to_uint32_little_endian(
		 &( data[ record_offsets_data_offset ] ),
		 record_data_offset );

		record_offsets_data_offset += 4;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: record: %02" PRIu32 " data offset\t\t: 0x%08" PRIx32 " (0x%08" PRIx32 ")\n",
			 function,
			 record_offsets_index,
			 record_data_offset & 0x0000ffffUL,
			 record_data_offset );
		}
#endif
		record_data_offset &= 0x0000ffffUL;

		if( ( record_data_offset < node_header->data_area_start_offset )
		 || ( record_data_offset >= node_header->data_area_end_offset ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid record: %" PRIu32 " data offset value out of bounds.",
			 function,
			 record_offsets_index );

			goto on_error;
		}
		record_data_offset += node_header_data_offset;

		byte_stream_copy_to_uint32_little_endian(
		 &( data[ record_data_offset ] ),
		 record_data_size );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: record: %02" PRIu32 " data size\t\t: %" PRIu32 "\n",
			 function,
			 record_offsets_index,
			 record_data_size );

			libcnotify_printf(
			 "%s: record: %02" PRIu32 " data:\n",
			 function,
			 record_offsets_index );
			libcnotify_print_data(
			 &( data[ record_data_offset ] ),
			 record_data_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		if( libfsrefs_node_record_initialize(
		     &node_record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create node record: %" PRIu32 ".",
			 function,
			 record_offsets_index );

			goto on_error;
		}
		if( libfsrefs_node_record_read_data(
		     node_record,
		     &( data[ record_data_offset ] ),
		     record_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read node record: %" PRIu32 ".",
			 function,
			 record_offsets_index );

			goto on_error;
		}
		if( libfsrefs_node_record_free(
		     &node_record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free node record: %" PRIu32 ".",
			 function,
			 record_offsets_index );

			goto on_error;
		}
	}
	if( libfsrefs_node_header_free(
	     &node_header,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free node header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( node_header != NULL )
	{
		libfsrefs_node_header_free(
		 &node_header,
		 NULL );
	}
	return( -1 );
}

/* Reads a ministore tree
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_ministore_tree_read_file_io_handle(
     libfsrefs_ministore_tree_t *ministore_tree,
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	libfsrefs_metadata_block_header_t *metadata_block_header = NULL;
	uint8_t *metadata_block_data                             = NULL;
	static char *function                                    = "libfsrefs_ministore_tree_read_file_io_handle";
	size_t header_size                                       = 0;
	size_t read_size                                         = 0;
	ssize_t read_count                                       = 0;

	if( ministore_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid ministore tree.",
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
	read_size = io_handle->metadata_block_size;

	if( read_size == 4096 )
	{
		read_size *= 4;
	}
	if( ( read_size < header_size )
	 || ( read_size > MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
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
	                                   read_size );

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
	              read_size,
	              file_offset,
	              error );

	if( read_count != (ssize_t) read_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read ministore tree metadata at offset: %" PRIi64 " (0x%08" PRIx64 ").",
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
		     "MSB+",
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
		if( ( metadata_block_header->block_number2 != 0 )
		 && ( metadata_block_header->block_number2 != metadata_block_header->block_number1 + 1 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid metadata block header - block number 2 value out of bounds.",
			 function );

			goto on_error;
		}
		if( ( metadata_block_header->block_number3 != 0 )
		 && ( metadata_block_header->block_number3 != metadata_block_header->block_number2 + 1 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid metadata block header - block number 3 value out of bounds.",
			 function );

			goto on_error;
		}
		if( ( metadata_block_header->block_number4 != 0 )
		 && ( metadata_block_header->block_number4 != metadata_block_header->block_number3 + 1 ) )
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
	if( libfsrefs_ministore_tree_read_data(
	     ministore_tree,
	     io_handle,
	     &( metadata_block_data[ header_size ] ),
	     read_size - header_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read ministore tree metadata.",
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

/* Retrieves the number of block descriptors
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_ministore_tree_get_number_of_block_descriptors(
     libfsrefs_ministore_tree_t *ministore_tree,
     int *number_of_block_descriptors,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_ministore_tree_get_number_of_block_descriptors";

	if( ministore_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid ministore tree.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     ministore_tree->block_descriptors_array,
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
int libfsrefs_ministore_tree_get_block_descriptor_by_index(
     libfsrefs_ministore_tree_t *ministore_tree,
     int block_descriptor_index,
     libfsrefs_block_descriptor_t **block_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_ministore_tree_get_block_descriptor_by_index";

	if( ministore_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid ministore tree.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     ministore_tree->block_descriptors_array,
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
