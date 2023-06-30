/*
 * Directory object functions
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
#include "libfsrefs_block_reference.h"
#include "libfsrefs_block_tree.h"
#include "libfsrefs_block_tree_node.h"
#include "libfsrefs_directory_entry.h"
#include "libfsrefs_directory_object.h"
#include "libfsrefs_libcerror.h"
#include "libfsrefs_libcnotify.h"
#include "libfsrefs_ministore_node.h"
#include "libfsrefs_node_record.h"
#include "libfsrefs_objects_tree.h"

/* Creates a directory object
 * Make sure the value directory_object is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_directory_object_initialize(
     libfsrefs_directory_object_t **directory_object,
     libfsrefs_io_handle_t *io_handle,
     libfsrefs_objects_tree_t *objects_tree,
     uint64_t object_identifier,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_directory_object_initialize";

	if( directory_object == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory object.",
		 function );

		return( -1 );
	}
	if( *directory_object != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid directory object value already set.",
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
	if( objects_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid objects tree.",
		 function );

		return( -1 );
	}
	*directory_object = memory_allocate_structure(
	                     libfsrefs_directory_object_t );

	if( *directory_object == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create directory object.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *directory_object,
	     0,
	     sizeof( libfsrefs_directory_object_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear directory object.",
		 function );

		memory_free(
		 *directory_object );

		*directory_object = NULL;

		return( -1 );
	}
	if( libfsrefs_block_tree_initialize(
	     &( ( *directory_object )->node_block_tree ),
	     io_handle->volume_size,
	     io_handle->metadata_block_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create node block tree.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( ( *directory_object )->directory_entries_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create directory entries array.",
		 function );

		goto on_error;
	}
	( *directory_object )->object_identifier = object_identifier;
	( *directory_object )->objects_tree      = objects_tree;

	return( 1 );

on_error:
	if( *directory_object != NULL )
	{
		if( ( *directory_object )->node_block_tree != NULL )
		{
			libfsrefs_block_tree_free(
			 &( ( *directory_object )->node_block_tree ),
			 (int (*)(intptr_t **, libcerror_error_t **)) &libfsrefs_block_reference_free,
			 NULL );
		}
		memory_free(
		 *directory_object );

		*directory_object = NULL;
	}
	return( -1 );
}

/* Frees a directory object
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_directory_object_free(
     libfsrefs_directory_object_t **directory_object,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_directory_object_free";
	int result            = 1;

	if( directory_object == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory object.",
		 function );

		return( -1 );
	}
	if( *directory_object != NULL )
	{
		/* The objects_tree reference is freed elsewhere
		 */
		if( libcdata_array_free(
		     &( ( *directory_object )->directory_entries_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfsrefs_directory_entry_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free directory entries array.",
			 function );

			result = -1;
		}
		if( libfsrefs_block_tree_free(
		     &( ( *directory_object )->node_block_tree ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfsrefs_block_reference_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free node block tree.",
			 function );

			result = -1;
		}
		if( ( *directory_object )->root_node != NULL )
		{
			if( libfsrefs_ministore_node_free(
			     &( ( *directory_object )->root_node ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free root node.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *directory_object );

		*directory_object = NULL;
	}
	return( result );
}


/* Checks if this is the first time the node block is being read
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_directory_object_check_if_node_block_first_read(
     libfsrefs_directory_object_t *directory_object,
     libfsrefs_block_tree_t *node_block_tree,
     uint64_t block_number,
     off64_t block_offset,
     libcerror_error_t **error )
{
	libfsrefs_block_descriptor_t *existing_block_descriptor = NULL;
	libfsrefs_block_descriptor_t *new_block_descriptor      = NULL;
	libfsrefs_block_tree_node_t *leaf_block_tree_node       = NULL;
	static char *function                                   = "libfsrefs_directory_object_check_if_node_block_first_read";
	int leaf_value_index                                    = 0;
	int result                                              = 0;

	if( directory_object == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory object.",
		 function );

		return( -1 );
	}
	if( libfsrefs_block_descriptor_initialize(
	     &new_block_descriptor,
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
	new_block_descriptor->block_number = block_number;

	result = libfsrefs_block_tree_insert_block_descriptor_by_offset(
	          node_block_tree,
	          block_offset,
	          new_block_descriptor,
	          &leaf_value_index,
	          &leaf_block_tree_node,
	          &existing_block_descriptor,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to insert block descriptor in node block tree.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid block number: %" PRIu64 " value already exists.",
		 function,
		 block_number );

		goto on_error;
	}
	new_block_descriptor = NULL;

	return( 1 );

on_error:
	if( new_block_descriptor != NULL )
	{
		libfsrefs_block_descriptor_free(
		 &new_block_descriptor,
		 NULL );
	}
	return( -1 );
}

/* Reads an directory object
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_directory_object_read(
     libfsrefs_directory_object_t *directory_object,
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_directory_object_read";

	if( directory_object == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory object.",
		 function );

		return( -1 );
	}
	if( directory_object->root_node != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid directory object - root node value already set.",
		 function );

		return( -1 );
	}
	if( libfsrefs_objects_tree_get_ministore_tree_by_identifier(
	     directory_object->objects_tree,
	     io_handle,
	     file_io_handle,
	     directory_object->object_identifier,
	     &( directory_object->root_node ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory object: 0x%08" PRIx64 " from objects tree.",
		 function,
		 directory_object->object_identifier );

		goto on_error;
	}
	if( ( directory_object->root_node->node_type_flags & 0x02 ) == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported directory object: 0x%08" PRIx64 " root node - missing is root (0x02) flag.",
		 function,
		 directory_object->object_identifier );

		goto on_error;
	}
	if( libfsrefs_directory_object_read_node(
	     directory_object,
	     io_handle,
	     file_io_handle,
	     directory_object->root_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read directory object: 0x%08" PRIx64 " root node.",
		 function,
		 directory_object->object_identifier );

		goto on_error;
	}
	return( 1 );

on_error:
	libcdata_array_empty(
	 directory_object->directory_entries_array,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfsrefs_directory_entry_free,
	 NULL );

	return( -1 );
}

/* Reads an directory object node
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_directory_object_read_node(
     libfsrefs_directory_object_t *directory_object,
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsrefs_ministore_node_t *node,
     libcerror_error_t **error )
{
	libfsrefs_block_reference_t *block_reference = NULL;
	libfsrefs_directory_entry_t *directory_entry = NULL;
	libfsrefs_ministore_node_t *sub_node         = NULL;
	libfsrefs_node_record_t *node_record         = NULL;
	static char *function                        = "libfsrefs_directory_object_read_node";
	uint16_t record_type                         = 0;
	int entry_index                              = 0;
	int number_of_records                        = 0;
	int record_index                             = 0;

	if( directory_object == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory object.",
		 function );

		return( -1 );
	}
	if( libfsrefs_ministore_node_get_number_of_records(
	     node,
	     &number_of_records,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of records.",
		 function );

		return( -1 );
	}
	for( record_index = 0;
	     record_index < number_of_records;
	     record_index++ )
	{
		if( libfsrefs_ministore_node_get_record_by_index(
		     node,
		     record_index,
		     &node_record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve record: %d.",
			 function,
			 record_index );

			return( -1 );
		}
		if( node_record == NULL )
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
		if( node_record->key_data == NULL )
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
		if( node_record->key_data_size >= 2 )
		{
			byte_stream_copy_to_uint16_little_endian(
			 node_record->key_data,
			 record_type );

			if( record_type != 0x0030 )
			{
				continue;
			}
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: record: %d key data:\n",
			 function,
			 record_index );
			libcnotify_print_data(
			 node_record->key_data,
			 node_record->key_data_size,
			 0 );
		}
#endif
		if( ( node->node_type_flags & 0x01 ) == 0 )
		{
			if( libfsrefs_directory_entry_initialize(
			     &directory_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create directory entry.",
				 function );

				goto on_error;
			}
			if( libfsrefs_directory_entry_read_node_record(
			     directory_entry,
			     io_handle,
			     node_record,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read directory entry from record: %d.",
				 function,
				 record_index );

				goto on_error;
			}
			if( libcdata_array_append_entry(
			     directory_object->directory_entries_array,
			     &entry_index,
			     (intptr_t *) directory_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append directory entry to array.",
				 function );

				goto on_error;
			}
			directory_entry = NULL;
		}
		else
		{
			if( libfsrefs_block_reference_initialize(
			     &block_reference,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create directory object: 0x%08" PRIx64 " sub node block reference: %d.",
				 function,
				 directory_object->object_identifier,
				 record_index );

				goto on_error;
			}
			if( libfsrefs_block_reference_read_data(
			     block_reference,
			     io_handle,
			     node_record->value_data,
			     node_record->value_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read directory object: 0x%08" PRIx64 " sub node: %d block reference.",
				 function,
				 directory_object->object_identifier,
				 record_index );

				goto on_error;
			}
			if( libfsrefs_file_system_get_block_offsets(
			     directory_object->objects_tree->file_system,
			     io_handle,
			     block_reference,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve irectory object: 0x%08" PRIx64 " sub node: %d block offsets.",
				 function,
				 directory_object->object_identifier,
				 record_index );

				goto on_error;
			}
			if( libfsrefs_directory_object_check_if_node_block_first_read(
			     directory_object,
			     directory_object->node_block_tree,
			     block_reference->block_numbers[ 0 ],
			     block_reference->block_offsets[ 0 ],
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to check if first read of block number: %" PRIu64 ".",
				 function,
				 block_reference->block_numbers[ 0 ] );

				goto on_error;
			}
			if( libfsrefs_ministore_node_initialize(
			     &sub_node,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create directory object: 0x%08" PRIx64 " ministore tree sub node: %d.",
				 function,
				 directory_object->object_identifier,
				 record_index );

				goto on_error;
			}
			if( libfsrefs_ministore_node_read_file_io_handle(
			     sub_node,
			     io_handle,
			     file_io_handle,
			     block_reference,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read directory object: 0x%08" PRIx64 " ministore tree sub node: %d.",
				 function,
				 directory_object->object_identifier,
				 record_index );

				goto on_error;
			}
			if( libfsrefs_block_reference_free(
			     &block_reference,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free directory object: 0x%08" PRIx64 " sub node: %d block reference.",
				 function,
				 directory_object->object_identifier,
				 record_index );

				goto on_error;
			}
			if( libfsrefs_directory_object_read_node(
			     directory_object,
			     io_handle,
			     file_io_handle,
			     sub_node,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read directory object: 0x%08" PRIx64 " sub node: %d.",
				 function,
				 directory_object->object_identifier,
				 record_index );

				goto on_error;
			}
			if( libfsrefs_ministore_node_free(
			     &sub_node,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free directory object: 0x%08" PRIx64 " sub node: %d.",
				 function,
				 directory_object->object_identifier,
				 record_index );

				goto on_error;
			}
		}
	}
	return( 1 );

on_error:
	if( sub_node != NULL )
	{
		libfsrefs_ministore_node_free(
		 &sub_node,
		 NULL );
	}
	if( block_reference != NULL )
	{
		libfsrefs_block_reference_free(
		 &block_reference,
		 NULL );
	}
	if( directory_entry != NULL )
	{
		libfsrefs_directory_entry_free(
		 &directory_entry,
		 NULL );
	}
	libcdata_array_empty(
	 directory_object->directory_entries_array,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfsrefs_directory_entry_free,
	 NULL );

	return( -1 );
}

/* Retrieves the number of directory entries
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_directory_object_get_number_of_directory_entries(
     libfsrefs_directory_object_t *directory_object,
     int *number_of_directory_entries,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_directory_object_get_number_of_directory_entries";

	if( directory_object == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory object.",
		 function );

		return( -1 );
	}
	if( directory_object->root_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid ministore node - missing root node.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     directory_object->directory_entries_array,
	     number_of_directory_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from directory entries array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific directory entry
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_directory_object_get_directory_entry_by_index(
     libfsrefs_directory_object_t *directory_object,
     int directory_entry_index,
     libfsrefs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_directory_object_get_directory_entry_by_index";

	if( directory_object == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory object.",
		 function );

		return( -1 );
	}
	if( directory_object->root_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid ministore node - missing root node.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     directory_object->directory_entries_array,
	     directory_entry_index,
	     (intptr_t **) directory_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry: %d from directory entries array.",
		 function,
		 directory_entry_index );

		return( -1 );
	}
	return( 1 );
}

