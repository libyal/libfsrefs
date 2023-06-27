/*
 * Ministore node record functions
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
#include "libfsrefs_node_record.h"

#include "fsrefs_ministore_tree.h"

/* Creates a node record
 * Make sure the value node_record is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_node_record_initialize(
     libfsrefs_node_record_t **node_record,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_node_record_initialize";

	if( node_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node record.",
		 function );

		return( -1 );
	}
	if( *node_record != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid node record value already set.",
		 function );

		return( -1 );
	}
	*node_record = memory_allocate_structure(
	                libfsrefs_node_record_t );

	if( *node_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create node record.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *node_record,
	     0,
	     sizeof( libfsrefs_node_record_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear node record.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *node_record != NULL )
	{
		memory_free(
		 *node_record );

		*node_record = NULL;
	}
	return( -1 );
}

/* Frees a node record
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_node_record_free(
     libfsrefs_node_record_t **node_record,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_node_record_free";

	if( node_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node record.",
		 function );

		return( -1 );
	}
	if( *node_record != NULL )
	{
		/* The key_data and value_data references are freed elsewhere
		 */
		memory_free(
		 *node_record );

		*node_record = NULL;
	}
	return( 1 );
}

/* Reads a node record
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_node_record_read_data(
     libfsrefs_node_record_t *node_record,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function      = "libfsrefs_node_record_read_data";
	size_t data_offset         = 0;
	uint16_t key_data_offset   = 0;
	uint16_t value_data_offset = 0;

	if( node_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node record.",
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
	if( ( data_size < sizeof( fsrefs_ministore_tree_node_record_t ) )
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
		 "%s: node record data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsrefs_ministore_tree_node_record_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsrefs_ministore_tree_node_record_t *) data )->size,
	 node_record->size );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fsrefs_ministore_tree_node_record_t *) data )->key_data_offset,
	 key_data_offset );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fsrefs_ministore_tree_node_record_t *) data )->key_data_size,
	 node_record->key_data_size );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fsrefs_ministore_tree_node_record_t *) data )->flags,
	 node_record->flags );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fsrefs_ministore_tree_node_record_t *) data )->value_data_offset,
	 value_data_offset );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fsrefs_ministore_tree_node_record_t *) data )->value_data_size,
	 node_record->value_data_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: size\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 node_record->size );

		libcnotify_printf(
		 "%s: key data offset\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 key_data_offset );

		libcnotify_printf(
		 "%s: key data size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 node_record->key_data_size );

		libcnotify_printf(
		 "%s: flags\t\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 node_record->flags );

		libcnotify_printf(
		 "%s: value data offset\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_data_offset );

		libcnotify_printf(
		 "%s: value data size\t\t\t: %" PRIu32 "\n",
		 function,
		 node_record->value_data_size );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	data_offset = sizeof( fsrefs_ministore_tree_node_record_t );

	if( ( key_data_offset < data_offset )
	 || ( key_data_offset >= data_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid key data offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( node_record->key_data_size > ( data_size - key_data_offset ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid key data size value out of bounds.",
		 function );

		return( -1 );
	}
	node_record->key_data = &( data[ key_data_offset ] );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: key data:\n",
		 function );
		libcnotify_print_data(
		 node_record->key_data,
		 node_record->key_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( ( value_data_offset < data_offset )
	 || ( value_data_offset >= data_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid value data offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( node_record->value_data_size > ( data_size - value_data_offset ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid value data size value out of bounds.",
		 function );

		return( -1 );
	}
	node_record->value_data = &( data[ value_data_offset ] );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: value data:\n",
		 function );
		libcnotify_print_data(
		 node_record->value_data,
		 node_record->value_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	return( 1 );
}

