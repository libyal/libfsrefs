/*
 * Objects tree functions
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

#include "libfsrefs_block_reference.h"
#include "libfsrefs_file_system.h"
#include "libfsrefs_libcerror.h"
#include "libfsrefs_ministore_node.h"
#include "libfsrefs_node_record.h"
#include "libfsrefs_objects_tree.h"

/* Creates a objects tree
 * Make sure the value objects_tree is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_objects_tree_initialize(
     libfsrefs_objects_tree_t **objects_tree,
     libfsrefs_file_system_t *file_system,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_objects_tree_initialize";

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
	if( *objects_tree != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid objects tree value already set.",
		 function );

		return( -1 );
	}
	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	*objects_tree = memory_allocate_structure(
	                 libfsrefs_objects_tree_t );

	if( *objects_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create objects tree.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *objects_tree,
	     0,
	     sizeof( libfsrefs_objects_tree_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear objects tree.",
		 function );

		memory_free(
		 *objects_tree );

		*objects_tree = NULL;

		return( -1 );
	}
	( *objects_tree )->file_system = file_system;

	return( 1 );

on_error:
	if( *objects_tree != NULL )
	{
		memory_free(
		 *objects_tree );

		*objects_tree = NULL;
	}
	return( -1 );
}

/* Frees a objects tree
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_objects_tree_free(
     libfsrefs_objects_tree_t **objects_tree,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_objects_tree_free";
	int result            = 1;

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
	if( *objects_tree != NULL )
	{
		/* The file_system reference is freed elsewhere
		 */
		if( ( *objects_tree )->root_node != NULL )
		{
			if( libfsrefs_ministore_node_free(
			     &( ( *objects_tree )->root_node ),
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
		 *objects_tree );

		*objects_tree = NULL;
	}
	return( result );
}

/* Reads an objects tree
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_objects_tree_read(
     libfsrefs_objects_tree_t *objects_tree,
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_objects_tree_read";

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
	if( objects_tree->root_node != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid objects tree - root node value already set.",
		 function );

		return( -1 );
	}
	if( libfsrefs_file_system_get_ministore_tree(
	     objects_tree->file_system,
	     io_handle,
	     file_io_handle,
	     0,
	     &( objects_tree->root_node ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve ministore tree: 0 (objects).",
		 function );

		goto on_error;
	}
	if( ( objects_tree->root_node->node_type_flags & 0x02 ) == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported ministore tree: 0 (objects) root node - missing is root (0x02) flag.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( objects_tree->root_node != NULL )
	{
		libfsrefs_ministore_node_free(
		 &( objects_tree->root_node ),
		 NULL );
	}
	return( -1 );
}

/* Retrieves the ministore tree of a specific object identifier
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsrefs_objects_tree_get_ministore_tree_by_identifier(
     libfsrefs_objects_tree_t *objects_tree,
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint64_t object_identifier,
     libfsrefs_ministore_node_t **root_node,
     libcerror_error_t **error )
{
	uint8_t key_data[ 16 ]                       = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	libfsrefs_block_reference_t *block_reference = NULL;
	libfsrefs_ministore_node_t *safe_root_node   = NULL;
	libfsrefs_node_record_t *node_record         = NULL;
	static char *function                        = "libfsrefs_objects_tree_get_ministore_tree_by_identifier";
	int result                                   = 0;

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
	if( root_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid root node.",
		 function );

		return( -1 );
	}
	byte_stream_copy_from_uint64_little_endian(
	 &( key_data[ 8 ] ),
	 object_identifier );

	result = libfsrefs_ministore_node_get_record_by_key(
	          objects_tree->root_node,
	          key_data,
	          16,
	          &node_record,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve identifier: 0x%08" PRIx64 " from objects tree.",
		 function,
		 object_identifier );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( node_record == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing node record.",
			 function );

			return( -1 );
		}
		if( libfsrefs_block_reference_initialize(
		     &block_reference,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create block reference.",
			 function );

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
			 "%s: unable to read block reference.",
			 function );

			goto on_error;
		}
		if( libfsrefs_file_system_get_block_offsets(
		     objects_tree->file_system,
		     io_handle,
		     block_reference,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve block offsets.",
			 function );

			goto on_error;
		}
		if( libfsrefs_ministore_node_initialize(
		     &safe_root_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create object: 0x%08" PRIx64 " ministore tree root node.",
			 function,
			 object_identifier );

			goto on_error;
		}
		if( libfsrefs_ministore_node_read_file_io_handle(
		     safe_root_node,
		     io_handle,
		     file_io_handle,
		     block_reference,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read object: 0x%08" PRIx64 " ministore tree root node.",
			 function,
			 object_identifier );

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
			 "%s: unable to free block reference.",
			 function );

			goto on_error;
		}
		*root_node = safe_root_node;
	}
	return( result );

on_error:
	if( safe_root_node != NULL )
	{
		libfsrefs_ministore_node_free(
		 &safe_root_node,
		 NULL );
	}
	if( block_reference != NULL )
	{
		libfsrefs_block_reference_free(
		 &block_reference,
		 NULL );
	}
	return( 1 );
}

