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

#include "libfsrefs_directory_object.h"
#include "libfsrefs_libcerror.h"
#include "libfsrefs_ministore_node.h"
#include "libfsrefs_node_record.h"
#include "libfsrefs_objects_tree.h"

/* Creates a directory object
 * Make sure the value directory_object is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_directory_object_initialize(
     libfsrefs_directory_object_t **directory_object,
     libfsrefs_objects_tree_t *objects_tree,
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
	( *directory_object )->objects_tree = objects_tree;

	return( 1 );

on_error:
	if( *directory_object != NULL )
	{
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

/* Reads an directory object
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_directory_object_read(
     libfsrefs_directory_object_t *directory_object,
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint64_t object_identifier,
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
	if( libfsrefs_objects_get_ministore_tree_by_identifier(
	     directory_object->objects_tree,
	     io_handle,
	     file_io_handle,
	     object_identifier,
	     &( directory_object->root_node ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory object: 0x%08" PRIx64 " from objects tree.",
		 function,
		 object_identifier );

		return( -1 );
	}
	return( 1 );
}

