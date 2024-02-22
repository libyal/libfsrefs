/*
 * Ministore node (or level 2+ metadata) functions
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
#include "libfsrefs_libbfio.h"
#include "libfsrefs_libcerror.h"
#include "libfsrefs_libcnotify.h"
#include "libfsrefs_metadata_block_header.h"
#include "libfsrefs_ministore_node.h"
#include "libfsrefs_node_header.h"
#include "libfsrefs_node_record.h"
#include "libfsrefs_tree_header.h"

#include "fsrefs_metadata_block.h"
#include "fsrefs_ministore_tree.h"

/* Creates a Ministore node
 * Make sure the value ministore_node is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_ministore_node_initialize(
     libfsrefs_ministore_node_t **ministore_node,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_ministore_node_initialize";

	if( ministore_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid ministore node.",
		 function );

		return( -1 );
	}
	if( *ministore_node != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid ministore node value already set.",
		 function );

		return( -1 );
	}
	*ministore_node = memory_allocate_structure(
	                   libfsrefs_ministore_node_t );

	if( *ministore_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create ministore node.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *ministore_node,
	     0,
	     sizeof( libfsrefs_ministore_node_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear ministore node.",
		 function );

		memory_free(
		 *ministore_node );

		*ministore_node = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *ministore_node )->records_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create records array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *ministore_node != NULL )
	{
		memory_free(
		 *ministore_node );

		*ministore_node = NULL;
	}
	return( -1 );
}

/* Frees a Ministore node
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_ministore_node_free(
     libfsrefs_ministore_node_t **ministore_node,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_ministore_node_free";
	int result            = 1;

	if( ministore_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid ministore node.",
		 function );

		return( -1 );
	}
	if( *ministore_node != NULL )
	{
		/* The data reference is freed elsewhere
		 */
		if( libcdata_array_free(
		     &( ( *ministore_node )->records_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfsrefs_node_record_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free records array.",
			 function );

			result = -1;
		}
		if( ( *ministore_node )->internal_data != NULL )
		{
			memory_free(
			 ( *ministore_node )->internal_data );
		}
		memory_free(
		 *ministore_node );

		*ministore_node = NULL;
	}
	return( result );
}

/* Reads a Ministore node
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_ministore_node_read_data(
     libfsrefs_ministore_node_t *ministore_node,
     libfsrefs_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfsrefs_node_header_t *node_header = NULL;
	libfsrefs_node_record_t *node_record = NULL;
	libfsrefs_tree_header_t *tree_header = NULL;
	static char *function                = "libfsrefs_ministore_node_read_data";
	size_t data_offset                   = 0;
	size_t record_offsets_data_offset    = 0;
	uint32_t node_header_offset          = 0;
	uint32_t record_data_offset          = 0;
	uint32_t record_data_size            = 0;
	uint32_t record_offsets_index        = 0;
	int entry_index                      = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	size_t data_area_start_offset        = 0;
#endif

	if( ministore_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid ministore node.",
		 function );

		return( -1 );
	}
	if( ministore_node->data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid ministore_node - data value already set.",
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
	if( ( io_handle->major_format_version != 1 )
	 && ( io_handle->major_format_version != 3 ) )
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
	if( ( data_size < 4 )
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
	byte_stream_copy_to_uint32_little_endian(
	 data,
	 node_header_offset );

	if( ( node_header_offset < 4 )
	 || ( node_header_offset >= ( data_size - 4 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid node header offset value out of bounds.",
		 function );

		goto on_error;
	}
	data_offset = 4;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: node header offset\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 node_header_offset );
	}
#endif
	if( node_header_offset >= ( data_offset + sizeof( fsrefs_ministore_tree_header_t ) ) )
	{
		if( libfsrefs_tree_header_initialize(
		     &tree_header,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create tree header.",
			 function );

			goto on_error;
		}
		if( libfsrefs_tree_header_read_data(
		     tree_header,
		     &( data[ data_offset ] ),
		     data_size - data_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read tree header.",
			 function );

			goto on_error;
		}
		if( libfsrefs_tree_header_free(
		     &tree_header,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free tree header.",
			 function );

			goto on_error;
		}
		data_offset += sizeof( fsrefs_ministore_tree_header_t );

		ministore_node->header_data      = &( data[ data_offset ] );
		ministore_node->header_data_size = node_header_offset - data_offset;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: header data:\n",
			 function );
			libcnotify_print_data(
			 ministore_node->header_data,
			 ministore_node->header_data_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif
	}
	data_offset = node_header_offset;

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
	 || ( node_header->data_area_start_offset > ( data_size - node_header_offset ) ) )
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
		data_area_start_offset = node_header_offset + node_header->data_area_start_offset;

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
	 || ( node_header->data_area_end_offset > ( data_size - node_header_offset ) ) )
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
	 || ( node_header->record_offsets_start_offset > ( data_size - node_header_offset ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid node header - record offsets start offset value out of bounds.",
		 function );

		goto on_error;
	}
	if( node_header->number_of_record_offsets > ( ( data_size - node_header_offset - node_header->record_offsets_start_offset ) / 4 ) )
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
	  ||  ( node_header->record_offsets_end_offset > ( data_size - node_header_offset ) ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid node header - record offsets end offset value out of bounds.",
		 function );

		goto on_error;
	}
	ministore_node->node_type_flags = node_header->node_type_flags;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: ministore node data:\n",
		 function );
		libcnotify_print_data(
		 &( data[ node_header_offset ] ),
		 data_size - node_header_offset,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	record_offsets_data_offset = node_header_offset + node_header->record_offsets_start_offset;

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
			if( io_handle->major_format_version == 1 )
			{
				libcnotify_printf(
				 "%s: record: %02" PRIu32 " data offset\t\t: 0x%08" PRIx32 "\n",
				 function,
				 record_offsets_index,
				 record_data_offset );
			}
			else if( io_handle->major_format_version == 3 )
			{
				libcnotify_printf(
				 "%s: record: %02" PRIu32 " data offset\t\t: 0x%08" PRIx32 " (0x%08" PRIx32 ")\n",
				 function,
				 record_offsets_index,
				 record_data_offset & 0x0000ffffUL,
				 record_data_offset );
			}
		}
#endif
		if( io_handle->major_format_version == 3 )
		{
			record_data_offset &= 0x0000ffffUL;
		}
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
		record_data_offset += node_header_offset;

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
			 "%s: unable to create record: %" PRIu32 ".",
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
			 "%s: unable to read record: %" PRIu32 ".",
			 function,
			 record_offsets_index );

			goto on_error;
		}
		if( libcdata_array_append_entry(
		     ministore_node->records_array,
		     &entry_index,
		     (intptr_t *) node_record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append record: %" PRIu32 " to array.",
			 function,
			 record_offsets_index );

			goto on_error;
		}
		node_record = NULL;
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
	ministore_node->data      = data;
	ministore_node->data_size = data_size;

	return( 1 );

on_error:
	if( node_record != NULL )
	{
		libfsrefs_node_record_free(
		 &node_record,
		 NULL );
	}
	if( node_header != NULL )
	{
		libfsrefs_node_header_free(
		 &node_header,
		 NULL );
	}
	libcdata_array_empty(
	 ministore_node->records_array,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfsrefs_node_record_free,
	 NULL );

	return( -1 );
}

/* Reads a Ministore node
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_ministore_node_read_file_io_handle(
     libfsrefs_ministore_node_t *ministore_node,
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsrefs_block_reference_t *block_reference,
     libcerror_error_t **error )
{
	libfsrefs_metadata_block_header_t *metadata_block_header = NULL;
	static char *function                                    = "libfsrefs_ministore_node_read_file_io_handle";
	size_t header_size                                       = 0;
	size_t read_size                                         = 0;
	ssize_t read_count                                       = 0;
	uint8_t block_number_index                               = 0;

	if( ministore_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid ministore node.",
		 function );

		return( -1 );
	}
	if( ministore_node->internal_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid ministore_node - data value already set.",
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
	ministore_node->internal_data = (uint8_t *) memory_allocate(
	                                             read_size );

	if( ministore_node->internal_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create metadata block data.",
		 function );

		goto on_error;
	}
	for( block_number_index = 0;
	     block_number_index < 4;
	     block_number_index++ )
	{
		if( block_reference->block_numbers[ block_number_index ] == 0 )
		{
			break;
		}
		read_count = libbfio_handle_read_buffer_at_offset(
		              file_io_handle,
		              ministore_node->internal_data,
		              io_handle->metadata_block_size,
		              block_reference->block_offsets[ block_number_index ],
		              error );

		if( read_count != (ssize_t) io_handle->metadata_block_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read ministore node block: %" PRIu8 " at offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 block_number_index,
			 block_reference->block_offsets[ block_number_index ],
			 block_reference->block_offsets[ block_number_index ] );

			goto on_error;
		}
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
	     ministore_node->internal_data,
	     header_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read metadata block header at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 block_reference->block_offsets[ 0 ],
		 block_reference->block_offsets[ 0 ] );

		goto on_error;
	}
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
	if( libfsrefs_ministore_node_read_data(
	     ministore_node,
	     io_handle,
	     &( ministore_node->internal_data[ header_size ] ),
	     read_size - header_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read ministore node.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( metadata_block_header != NULL )
	{
		libfsrefs_metadata_block_header_free(
		 &metadata_block_header,
		 NULL );
	}
	if( ministore_node->internal_data != NULL )
	{
		memory_free(
		 ministore_node->internal_data );

		ministore_node->internal_data = NULL;
	}
	ministore_node->data_size = 0;

	return( -1 );
}

/* Retrieves the number of records
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_ministore_node_get_number_of_records(
     libfsrefs_ministore_node_t *ministore_node,
     int *number_of_records,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_ministore_node_get_number_of_records";

	if( ministore_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid ministore node.",
		 function );

		return( -1 );
	}
	if( ministore_node->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid ministore node - missing data.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     ministore_node->records_array,
	     number_of_records,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from records array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific records
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_ministore_node_get_record_by_index(
     libfsrefs_ministore_node_t *ministore_node,
     int record_index,
     libfsrefs_node_record_t **node_record,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_ministore_node_get_record_by_index";

	if( ministore_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid ministore node.",
		 function );

		return( -1 );
	}
	if( ministore_node->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid ministore node - missing data.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     ministore_node->records_array,
	     record_index,
	     (intptr_t **) node_record,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry: %d from records array.",
		 function,
		 record_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the record for a specific key
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsrefs_ministore_node_get_record_by_key(
     libfsrefs_ministore_node_t *ministore_node,
     const uint8_t *key_data,
     size_t key_data_size,
     libfsrefs_node_record_t **node_record,
     libcerror_error_t **error )
{
	libfsrefs_node_record_t *safe_node_record = NULL;
	size_t key_data_offset                    = 0;
	static char *function                     = "libfsrefs_ministore_node_get_record_by_key";
	int number_of_records                     = 0;
	int record_index                          = 0;
	int result                                = 0;

	if( ministore_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid ministore node.",
		 function );

		return( -1 );
	}
	if( ministore_node->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid ministore node - missing data.",
		 function );

		return( -1 );
	}
	if( key_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key data.",
		 function );

		return( -1 );
	}
	if( ( key_data_size == 0 )
	 || ( key_data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid key data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     ministore_node->records_array,
	     &number_of_records,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from records array.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: requested key data:\n",
		 function );
		libcnotify_print_data(
		 key_data,
		 key_data_size,
		 0 );
	}
#endif
/* TODO add support for branch nodes */

	for( record_index = 0;
	     record_index < number_of_records;
	     record_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     ministore_node->records_array,
		     record_index,
		     (intptr_t **) &safe_node_record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry: %d from records array.",
			 function,
			 record_index );

			return( -1 );
		}
		if( safe_node_record == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing record: %d.",
			 function,
			 record_index );

			return( -1 );
		}
		if( safe_node_record->key_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid record: %d - missing key data.",
			 function,
			 record_index );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: record: %d key data:\n",
			 function,
			 record_index );
			libcnotify_print_data(
			 safe_node_record->key_data,
			 safe_node_record->key_data_size,
			 0 );
		}
#endif
		if( key_data_size != safe_node_record->key_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid record: %d - key data size mismatch.",
			 function,
			 record_index );

			return( -1 );
		}
		key_data_offset = key_data_size;

		do
		{
			key_data_offset--;

			result = (int) key_data[ key_data_offset ] - (int) safe_node_record->key_data[ key_data_offset ];

			if( result != 0 )
			{
				break;
			}
		}
		while( key_data_offset > 0 );

		if( result == 0 )
		{
			*node_record = safe_node_record;

			return( 1 );
		}
		else if( result < 0 )
		{
			break;
		}
	}
	return( 0 );
}

