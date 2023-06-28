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
#include <memory.h>
#include <types.h>

#include "libfsrefs_checkpoint.h"
#include "libfsrefs_file_system.h"
#include "libfsrefs_io_handle.h"
#include "libfsrefs_libcerror.h"
#include "libfsrefs_ministore_node.h"
#include "libfsrefs_superblock.h"

/* Creates a file system
 * Make sure the value file_system is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_file_system_initialize(
     libfsrefs_file_system_t **file_system,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_file_system_initialize";

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
	if( *file_system != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file system value already set.",
		 function );

		return( -1 );
	}
	*file_system = memory_allocate_structure(
	                libfsrefs_file_system_t );

	if( *file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file system.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *file_system,
	     0,
	     sizeof( libfsrefs_file_system_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file system.",
		 function );

		memory_free(
		 *file_system );

		*file_system = NULL;

		return( -1 );
	}
	return( 1 );

on_error:
	if( *file_system != NULL )
	{
		memory_free(
		 *file_system );

		*file_system = NULL;
	}
	return( -1 );
}

/* Frees a file system
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_file_system_free(
     libfsrefs_file_system_t **file_system,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_file_system_free";
	int result            = 1;

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
	if( *file_system != NULL )
	{
		if( ( *file_system )->checkpoint != NULL )
		{
			if( libfsrefs_checkpoint_free(
			     &( ( *file_system )->checkpoint ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free checkpoint.",
				 function );

				result = -1;
			}
		}
		if( ( *file_system )->superblock != NULL )
		{
			if( libfsrefs_superblock_free(
			     &( ( *file_system )->superblock ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free superblock.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *file_system );

		*file_system = NULL;
	}
	return( result );
}

/* Reads the superblock
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_file_system_read_superblock(
     libfsrefs_file_system_t *file_system,
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_file_system_read_superblock";

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
	if( file_system->superblock != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file system - superblock value already set.",
		 function );

		return( -1 );
	}
	if( libfsrefs_superblock_initialize(
	     &( file_system->superblock ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create superblock.",
		 function );

		goto on_error;
	}
	if( libfsrefs_superblock_read_file_io_handle(
	     file_system->superblock,
	     io_handle,
	     file_io_handle,
	     file_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read superblock at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		goto on_error;
	}
	return( 1 );

on_error:
	if( file_system->superblock != NULL )
	{
		libfsrefs_superblock_free(
		 &( file_system->superblock ),
		 NULL );
	}
	return( -1 );
}

/* Reads the checkpoints
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_file_system_read_checkpoints(
     libfsrefs_file_system_t *file_system,
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libfsrefs_checkpoint_t *primary_checkpoint   = NULL;
	libfsrefs_checkpoint_t *secondary_checkpoint = NULL;
	static char *function                        = "libfsrefs_file_system_read_checkpoints";
	off64_t checkpoint_offset                    = 0;

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
	if( file_system->superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file system - missing superblock.",
		 function );

		return( -1 );
	}
	if( file_system->checkpoint != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file system - checkpoint value already set.",
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
/* TODO support more than 2 checkpoints */

	if( libfsrefs_checkpoint_initialize(
	     &primary_checkpoint,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create primary checkpoint.",
		 function );

		goto on_error;
	}
	checkpoint_offset = file_system->superblock->primary_checkpoint_block_number * io_handle->metadata_block_size;

	if( libfsrefs_checkpoint_read_file_io_handle(
	     primary_checkpoint,
	     io_handle,
	     file_io_handle,
	     checkpoint_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read primary checkpoint at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 checkpoint_offset,
		 checkpoint_offset );

		goto on_error;
	}
	if( libfsrefs_checkpoint_initialize(
	     &secondary_checkpoint,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create secondary checkpoint.",
		 function );

		goto on_error;
	}
	checkpoint_offset = file_system->superblock->secondary_checkpoint_block_number * io_handle->metadata_block_size;

	if( libfsrefs_checkpoint_read_file_io_handle(
	     secondary_checkpoint,
	     io_handle,
	     file_io_handle,
	     checkpoint_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read secondary checkpoint at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 checkpoint_offset,
		 checkpoint_offset );

		goto on_error;
	}
	if( primary_checkpoint->sequence_number >= secondary_checkpoint->sequence_number )
	{
		if( libfsrefs_checkpoint_free(
		     &secondary_checkpoint,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free secondary checkpoint.",
			 function );

			goto on_error;
		}
		file_system->checkpoint = primary_checkpoint;
	}
	else
	{
		if( libfsrefs_checkpoint_free(
		     &primary_checkpoint,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free primary checkpoint.",
			 function );

			goto on_error;
		}
		file_system->checkpoint = secondary_checkpoint;
	}
	return( 1 );

on_error:
	if( secondary_checkpoint != NULL )
	{
		libfsrefs_checkpoint_free(
		 &secondary_checkpoint,
		 NULL );
	}
	if( primary_checkpoint != NULL )
	{
		libfsrefs_checkpoint_free(
		 &primary_checkpoint,
		 NULL );
	}
	file_system->checkpoint = NULL;

	return( -1 );
}

/* Reads the container trees
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_file_system_read_container_trees(
     libfsrefs_file_system_t *file_system,
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libfsrefs_ministore_node_t *root_node = NULL;
	static char *function                 = "libfsrefs_file_system_read_container_trees";

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
	if( libfsrefs_file_system_get_ministore_tree(
	     file_system,
	     io_handle,
	     file_io_handle,
	     7,
	     &root_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve ministore tree: 7 (containers) root node.",
		 function );

		goto on_error;
	}
	if( libfsrefs_ministore_node_free(
	     &root_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free ministore tree: 7 (containers) root node.",
		 function );

		goto on_error;
	}
	if( libfsrefs_file_system_get_ministore_tree(
	     file_system,
	     io_handle,
	     file_io_handle,
	     8,
	     &root_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve ministore tree: 8 (containers) root node.",
		 function );

		goto on_error;
	}
	if( libfsrefs_ministore_node_free(
	     &root_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free ministore tree: 8 (containers) root node.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( root_node != NULL )
	{
		libfsrefs_ministore_node_free(
		 &root_node,
		 NULL );
	}
	return( -1 );
}

/* Reads a ministore node
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_file_system_read_ministore_node(
     libfsrefs_file_system_t *file_system,
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsrefs_block_descriptor_t *block_descriptor,
     libfsrefs_ministore_node_t **node,
     libcerror_error_t **error )
{
	libfsrefs_ministore_node_t *safe_node = NULL;
	static char *function                 = "libfsrefs_file_system_read_ministore_node";
	off64_t node_offset                   = 0;
	uint64_t block_number                 = 0;
	uint64_t container_identifier         = 0;

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
	if( block_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block descriptor.",
		 function );

		return( -1 );
	}
	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	if( libfsrefs_ministore_node_initialize(
	     &safe_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create ministore tree node.",
		 function );

		goto on_error;
	}
	block_number = block_descriptor->block_number1;

	if( ( io_handle->container_size != 0 )
	 && ( block_descriptor->block_number1 > io_handle->container_size ) )
	{
		container_identifier = block_number / io_handle->container_size;
		block_number        %= io_handle->container_size;

/* TODO look up container block range */
	}
	node_offset = block_number * io_handle->metadata_block_size;

	if( libfsrefs_ministore_node_read_file_io_handle(
	     safe_node,
	     io_handle,
	     file_io_handle,
	     node_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read ministore tree node block: %" PRIu64 " at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 block_descriptor->block_number1,
		 node_offset,
		 node_offset );

		goto on_error;
	}
	*node = safe_node;

	return( 1 );

on_error:
	if( safe_node != NULL )
	{
		libfsrefs_ministore_node_free(
		 &safe_node,
		 NULL );
	}
	return( 1 );
}

/* Retrieves the number of ministore trees
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_file_system_get_number_of_ministore_trees(
     libfsrefs_file_system_t *file_system,
     int *number_of_ministore_trees,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_file_system_get_number_of_ministore_trees";

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
	if( libfsrefs_checkpoint_get_number_of_ministore_tree_block_descriptors(
	     file_system->checkpoint,
	     number_of_ministore_trees,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of ministore tree block descriptors from checkpoint.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific ministore tree
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_file_system_get_ministore_tree(
     libfsrefs_file_system_t *file_system,
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     int ministore_tree_index,
     libfsrefs_ministore_node_t **root_node,
     libcerror_error_t **error )
{
	libfsrefs_block_descriptor_t *block_descriptor = NULL;
	static char *function                          = "libfsrefs_file_system_get_ministore_tree";

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
	if( libfsrefs_checkpoint_get_ministore_tree_block_descriptor_by_index(
	     file_system->checkpoint,
	     ministore_tree_index,
	     &block_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve ministore tree: %d block descriptor from checkpoint.",
		 function,
		 ministore_tree_index );

		return( -1 );
	}
	if( libfsrefs_file_system_read_ministore_node(
	     file_system,
	     io_handle,
	     file_io_handle,
	     block_descriptor,
	     root_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to create ministore tree: %d root node.",
		 function,
		 ministore_tree_index );

		return( -1 );
	}
	return( 1 );
}

