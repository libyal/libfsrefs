/*
 * Block descriptor functions
 *
 * Copyright (C) 2012-2022, Joachim Metz <joachim.metz@gmail.com>
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

#include "libfsrefs_block_descriptor.h"

/* Creates a block descriptor
 * Make sure the value block_descriptor is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_block_descriptor_initialize(
     libfsrefs_block_descriptor_t **block_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_block_descriptor_initialize";

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
	if( *block_descriptor != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid block descriptor value already set.",
		 function );

		return( -1 );
	}
	*block_descriptor = memory_allocate_structure(
	                     libfsrefs_block_descriptor_t );

	if( *block_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create block descriptor.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *block_descriptor,
	     0,
	     sizeof( libfsrefs_block_descriptor_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear block descriptor.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *block_descriptor != NULL )
	{
		memory_free(
		 *block_descriptor );

		*block_descriptor = NULL;
	}
	return( -1 );
}

/* Frees a block descriptor
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_block_descriptor_free(
     libfsrefs_block_descriptor_t **block_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_block_descriptor_free";

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
	if( *block_descriptor != NULL )
	{
		if( ( *block_descriptor )->identifier_data != NULL )
		{
			memory_free(
			 ( *block_descriptor )->identifier_data );
		}
		memory_free(
		 *block_descriptor );

		*block_descriptor = NULL;
	}
	return( 1 );
}

/* Sets the identifier
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_block_descriptor_set_identifier(
     libfsrefs_block_descriptor_t *block_descriptor,
     const uint8_t *identifier_data,
     uint16_t identifier_data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_block_descriptor_set_identifier";

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
	if( block_descriptor->identifier_data != NULL )
	{
		memory_free(
		 block_descriptor->identifier_data );
	}
	if( ( identifier_data != NULL )
	 && ( identifier_data_size > 0 ) )
	{
		block_descriptor->identifier_data = (uint8_t *) memory_allocate(
		                                                 sizeof( uint8_t ) * identifier_data_size );

		if( block_descriptor->identifier_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create identifier data.",
			 function );

			goto on_error;
		}
		block_descriptor->identifier_data_size = identifier_data_size;

		if( memory_copy(
		     block_descriptor->identifier_data,
		     identifier_data,
		     sizeof( uint8_t ) * identifier_data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy identifier data.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( block_descriptor->identifier_data != NULL )
	{
		memory_free(
		 block_descriptor->identifier_data );

		block_descriptor->identifier_data = NULL;
	}
	block_descriptor->identifier_data_size = 0;

	return( -1 );
}

