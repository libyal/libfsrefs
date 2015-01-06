/*
 * Level 3 metadata functions
 *
 * Copyright (C) 2012-2015, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libfsrefs_io_handle.h"
#include "libfsrefs_libbfio.h"
#include "libfsrefs_libcerror.h"
#include "libfsrefs_libcnotify.h"
#include "libfsrefs_level3_metadata.h"
#include "libfsrefs_metadata_block.h"
#include "libfsrefs_metadata_table.h"
#include "libfsrefs_metadata_value.h"
#include "libfsrefs_volume_information.h"
#include "libfsrefs_volume_name.h"

#include "fsrefs_metadata_block.h"

/* Creates level 3 metadata
 * Make sure the value level3_metadata is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_level3_metadata_initialize(
     libfsrefs_level3_metadata_t **level3_metadata,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_level3_metadata_initialize";

	if( level3_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid level 3 metadata.",
		 function );

		return( -1 );
	}
	if( *level3_metadata != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid level 3 metadata value already set.",
		 function );

		return( -1 );
	}
	*level3_metadata = memory_allocate_structure(
	                    libfsrefs_level3_metadata_t );

	if( *level3_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create level 3 metadata.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *level3_metadata,
	     0,
	     sizeof( libfsrefs_level3_metadata_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear level 3 metadata.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *level3_metadata != NULL )
	{
		memory_free(
		 *level3_metadata );

		*level3_metadata = NULL;
	}
	return( -1 );
}

/* Frees level 3 metadata
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_level3_metadata_free(
     libfsrefs_level3_metadata_t **level3_metadata,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_level3_metadata_free";

	if( level3_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid level 3 metadata.",
		 function );

		return( -1 );
	}
	if( *level3_metadata != NULL )
	{
		memory_free(
		 *level3_metadata );

		*level3_metadata = NULL;
	}
	return( 1 );
}

/* Reads the level 3 metadata
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_level3_metadata_read(
     libfsrefs_level3_metadata_t *level3_metadata,
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	libfsrefs_metadata_block_t *metadata_block         = NULL;
	libfsrefs_metadata_table_t *metadata_table         = NULL;
	libfsrefs_metadata_value_t *metadata_value         = NULL;
	libfsrefs_metadata_table_t *sub_metadata_table     = NULL;
	libfsrefs_volume_information_t *volume_information = NULL;
	libfsrefs_volume_name_t *volume_name               = NULL;
	static char *function                              = "libfsrefs_level3_metadata_initialize";
	uint64_t value_identifier                          = 0;
	uint32_t value_data_size                           = 0;
	int metadata_table_value_index                     = 0;
	int number_of_metadata_table_values                = 0;

	if( level3_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid level 3 metadata.",
		 function );

		return( -1 );
	}
	if( libfsrefs_metadata_block_initialize(
	     &metadata_block,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create metadata block.",
		 function );

		goto on_error;
	}
	if( libfsrefs_metadata_block_read(
	     metadata_block,
	     io_handle,
	     file_io_handle,
	     file_offset,
	     3,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read metadata block.",
		 function );

		goto on_error;
	}
	if( libfsrefs_metadata_table_initialize(
	     &metadata_table,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create metadata table.",
		 function );

		goto on_error;
	}
	if( libfsrefs_metadata_table_read(
	     metadata_table,
	     metadata_block->data,
	     (size_t) metadata_block->data_size - sizeof( fsrefs_metadata_block_header_t ),
	     sizeof( fsrefs_metadata_block_header_t ),
	     3,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read metadata table.",
		 function );

		goto on_error;
	}
	if( libfsrefs_metadata_table_get_number_of_values(
	     metadata_table,
	     &number_of_metadata_table_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values from metdata table.",
		 function );

		goto on_error;
	}
	for( metadata_table_value_index = 0;
	     metadata_table_value_index < number_of_metadata_table_values;
	     metadata_table_value_index++ )
	{
		if( libfsrefs_metadata_table_get_value_by_index(
		     metadata_table,
		     metadata_table_value_index,
		     &metadata_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %d from metadata table.",
			 function,
			 metadata_table_value_index );

			goto on_error;
		}
		if( metadata_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing metadata value: %d.",
			 function,
			 metadata_table_value_index );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: value: %02d identifier data:\n",
			 function,
			 metadata_table_value_index );
			libcnotify_print_data(
			 metadata_value->identifier_data,
			 metadata_value->identifier_data_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif
		if( metadata_value->identifier_data_size == 4 )
		{
			byte_stream_copy_to_uint32_little_endian(
			 metadata_value->identifier_data,
			 value_identifier );
		}
		else if( metadata_value->identifier_data_size == 8 )
		{
			byte_stream_copy_to_uint64_little_endian(
			 metadata_value->identifier_data,
			 value_identifier );
		}
		else
		{
			value_identifier = 0;
		}
		switch( value_identifier )
		{
			case 0x00000530UL:
				if( libfsrefs_metadata_table_initialize(
				     &sub_metadata_table,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create sub metadata table.",
					 function );

					goto on_error;
				}
				if( libfsrefs_metadata_table_read(
				     sub_metadata_table,
				     metadata_value->data,
				     (size_t) metadata_value->data_size,
				     0,
				     4,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read sub metadata table.",
					 function );

					goto on_error;
				}
				if( libfsrefs_metadata_table_free(
				     &sub_metadata_table,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free sub metadata table.",
					 function );

					goto on_error;
				}
				break;

			case 0x00000510UL:
				if( libfsrefs_volume_name_initialize(
				     &volume_name,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create volume name.",
					 function );

					goto on_error;
				}
				if( libfsrefs_volume_name_read(
				     volume_name,
				     metadata_value,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read volume name.",
					 function );

					goto on_error;
				}
				if( libfsrefs_volume_name_free(
				     &volume_name,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free volume name.",
					 function );

					goto on_error;
				}
				break;

			case 0x00000520UL:
				if( libfsrefs_volume_information_initialize(
				     &volume_information,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create volume information.",
					 function );

					goto on_error;
				}
				if( libfsrefs_volume_information_read(
				     volume_information,
				     metadata_value,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read volume information.",
					 function );

					goto on_error;
				}
				if( libfsrefs_volume_information_free(
				     &volume_information,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free volume information.",
					 function );

					goto on_error;
				}
				break;

			default:
/* TODO
				if( ( ( metadata_value->flags & 0x0008 ) != 0 )
				 || ( metadata_value->data_size > 1024 ) )
 */
				byte_stream_copy_to_uint32_little_endian(
				 metadata_value->data,
				 value_data_size );

				if( ( value_data_size != 0 )
				 && ( value_data_size < metadata_value->data_size ) )
				{
					if( libfsrefs_metadata_table_initialize(
					     &sub_metadata_table,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
						 "%s: unable to create sub metadata table.",
						 function );

						goto on_error;
					}
					if( libfsrefs_metadata_table_read(
					     sub_metadata_table,
					     metadata_value->data,
					     (size_t) metadata_value->data_size,
					     0,
					     4,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_READ_FAILED,
						 "%s: unable to read sub metadata table.",
						 function );

						goto on_error;
					}
					if( libfsrefs_metadata_table_free(
					     &sub_metadata_table,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
						 "%s: unable to free sub metadata table.",
						 function );

						goto on_error;
					}
				}
				break;
		}
	}
	if( libfsrefs_metadata_table_free(
	     &metadata_table,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free metadata table.",
		 function );

		goto on_error;
	}
	if( libfsrefs_metadata_block_free(
	     &metadata_block,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free metadata block.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( sub_metadata_table != NULL )
	{
		libfsrefs_metadata_table_free(
		 &sub_metadata_table,
		 NULL );
	}
	if( volume_information != NULL )
	{
		libfsrefs_volume_information_free(
		 &volume_information,
		 NULL );
	}
	if( volume_name != NULL )
	{
		libfsrefs_volume_name_free(
		 &volume_name,
		 NULL );
	}
	if( metadata_table != NULL )
	{
		libfsrefs_metadata_table_free(
		 &metadata_table,
		 NULL );
	}
	if( metadata_block != NULL )
	{
		libfsrefs_metadata_block_free(
		 &metadata_block,
		 NULL );
	}
	return( -1 );
}

