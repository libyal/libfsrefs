/*
 * Ministore node header functions
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
#include "libfsrefs_node_header.h"

#include "fsrefs_ministore_tree.h"

/* Creates a node header
 * Make sure the value node_header is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_node_header_initialize(
     libfsrefs_node_header_t **node_header,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_node_header_initialize";

	if( node_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node header.",
		 function );

		return( -1 );
	}
	if( *node_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid node header value already set.",
		 function );

		return( -1 );
	}
	*node_header = memory_allocate_structure(
	                libfsrefs_node_header_t );

	if( *node_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create node header.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *node_header,
	     0,
	     sizeof( libfsrefs_node_header_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear node header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *node_header != NULL )
	{
		memory_free(
		 *node_header );

		*node_header = NULL;
	}
	return( -1 );
}

/* Frees a node header
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_node_header_free(
     libfsrefs_node_header_t **node_header,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_node_header_free";

	if( node_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node header.",
		 function );

		return( -1 );
	}
	if( *node_header != NULL )
	{
		memory_free(
		 *node_header );

		*node_header = NULL;
	}
	return( 1 );
}

/* Reads a node header
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_node_header_read_data(
     libfsrefs_node_header_t *node_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_node_header_read_data";

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit  = 0;
	uint16_t value_16bit  = 0;
#endif

	if( node_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node header.",
		 function );

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
	if( ( data_size < sizeof( fsrefs_ministore_tree_node_header_t ) )
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
		 "%s: node header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsrefs_ministore_tree_node_header_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsrefs_ministore_tree_node_header_t *) data )->data_area_start_offset,
	 node_header->data_area_start_offset );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsrefs_ministore_tree_node_header_t *) data )->data_area_end_offset,
	 node_header->data_area_end_offset );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsrefs_ministore_tree_node_header_t *) data )->record_offsets_start_offset,
	 node_header->record_offsets_start_offset );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsrefs_ministore_tree_node_header_t *) data )->number_of_record_offsets,
	 node_header->number_of_record_offsets );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsrefs_ministore_tree_node_header_t *) data )->record_offsets_end_offset,
	 node_header->record_offsets_end_offset );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: data area start offset\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 node_header->data_area_start_offset );

		libcnotify_printf(
		 "%s: data area end offset\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 node_header->data_area_end_offset );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsrefs_ministore_tree_node_header_t *) data )->unused_data_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: unused data size\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: node level\t\t\t\t: %" PRIu8 "\n",
		 function,
		 ( (fsrefs_ministore_tree_node_header_t *) data )->node_level );

		libcnotify_printf(
		 "%s: node type\t\t\t\t: %" PRIu8 "\n",
		 function,
		 ( (fsrefs_ministore_tree_node_header_t *) data )->node_type );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsrefs_ministore_tree_node_header_t *) data )->unknown1,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: record offsets start offset\t\t: 0x%08" PRIx32 "\n",
		 function,
		 node_header->record_offsets_start_offset );

		libcnotify_printf(
		 "%s: number of record offsets\t\t: %" PRIu32 "\n",
		 function,
		 node_header->number_of_record_offsets );

		libcnotify_printf(
		 "%s: record offsets end offset\t\t: 0x%08" PRIx32 "\n",
		 function,
		 node_header->record_offsets_end_offset );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsrefs_ministore_tree_node_header_t *) data )->unknown2,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown2\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

