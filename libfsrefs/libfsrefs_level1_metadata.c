/*
 * Level 1 metadata functions
 *
 * Copyright (C) 2012-2021, Joachim Metz <joachim.metz@gmail.com>
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
#include "libfsrefs_level1_metadata.h"
#include "libfsrefs_metadata_block.h"

/* Creates level 1 metadata
 * Make sure the value level1_metadata is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_level1_metadata_initialize(
     libfsrefs_level1_metadata_t **level1_metadata,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_level1_metadata_initialize";

	if( level1_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid level 1 metadata.",
		 function );

		return( -1 );
	}
	if( *level1_metadata != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid level 1 metadata value already set.",
		 function );

		return( -1 );
	}
	*level1_metadata = memory_allocate_structure(
	                    libfsrefs_level1_metadata_t );

	if( *level1_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create level 1 metadata.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *level1_metadata,
	     0,
	     sizeof( libfsrefs_level1_metadata_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear level 1 metadata.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( ( *level1_metadata )->level2_metadata_block_descriptors_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create level 2 metadata block descriptors array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *level1_metadata != NULL )
	{
		memory_free(
		 *level1_metadata );

		*level1_metadata = NULL;
	}
	return( -1 );
}

/* Frees level 1 metadata
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_level1_metadata_free(
     libfsrefs_level1_metadata_t **level1_metadata,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_level1_metadata_free";
	int result            = 1;

	if( level1_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid level 1 metadata.",
		 function );

		return( -1 );
	}
	if( *level1_metadata != NULL )
	{
		if( libcdata_array_free(
		     &( ( *level1_metadata )->level2_metadata_block_descriptors_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfsrefs_block_descriptor_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free level 2 metadata block descriptors array.",
			 function );

			result = -1;
		}
		memory_free(
		 *level1_metadata );

		*level1_metadata = NULL;
	}
	return( result );
}

/* Reads the level 1 metadata
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_level1_metadata_read(
     libfsrefs_level1_metadata_t *level1_metadata,
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	libfsrefs_block_descriptor_t *block_descriptor = NULL;
	libfsrefs_metadata_block_t *metadata_block     = NULL;
	static char *function                          = "libfsrefs_level1_metadata_read";
	size_t metadata_block_data_offset              = 0;
	uint32_t entry_index                           = 0;
	uint32_t entry_offset                          = 0;
	uint32_t number_of_entries                     = 0;
	int array_entry_index                          = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit                           = 0;
	uint32_t value_32bit                           = 0;
	uint16_t value_16bit                           = 0;
#endif

	if( level1_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid level 1 metadata.",
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
	     1,
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
	if( metadata_block->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid metadata block - missing data.",
		 function );

		goto on_error;
	}
	level1_metadata->sequence_number = metadata_block->sequence_number;

/* TODO add structures */
	byte_stream_copy_to_uint32_little_endian(
	 &( metadata_block->data[ 56 ] ),
	 entry_offset );

	byte_stream_copy_to_uint32_little_endian(
	 &( metadata_block->data[ 88 ] ),
	 number_of_entries );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 &( metadata_block->data[ 48 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint16_little_endian(
		 &( metadata_block->data[ 52 ] ),
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown2\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 &( metadata_block->data[ 54 ] ),
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown3\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: unknown4 offset\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 entry_offset );

		byte_stream_copy_to_uint32_little_endian(
		 &( metadata_block->data[ 60 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: table entry size\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint64_little_endian(
		 &( metadata_block->data[ 64 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: sequence number\t\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( metadata_block->data[ 72 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown5\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( metadata_block->data[ 76 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown6\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: unknown7:\n",
		 function );
		libcnotify_print_data(
		 &( metadata_block->data[ 80 ] ),
		 8,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

		libcnotify_printf(
		 "%s: number of table entries\t\t\t: %" PRIu32 "\n",
		 function,
		 number_of_entries );

		libcnotify_printf(
		 "%s: table entry offsets data:\n",
		 function );
		libcnotify_print_data(
		 &( metadata_block->data[ 92 ] ),
		 4 * number_of_entries,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
/* TODO check if entry offset is in bounds */

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 &( metadata_block->data[ entry_offset ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: self entry: block number\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 &( metadata_block->data[ entry_offset + 8 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: self entry: unknown1\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 &( metadata_block->data[ entry_offset + 16 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: self entry: checksum\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "\n" );
	}
#endif
	metadata_block_data_offset = 92;

	for( entry_index = 0;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
		byte_stream_copy_to_uint32_little_endian(
		 &( metadata_block->data[ metadata_block_data_offset ] ),
		 entry_offset );

		metadata_block_data_offset += 4;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: entry: %02" PRIu32 " offset\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 entry_index,
			 entry_offset );
		}
#endif
		if( libfsrefs_block_descriptor_initialize(
		     &block_descriptor,
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
/* TODO check if entry offset is in bounds */

		byte_stream_copy_to_uint64_little_endian(
		 &( metadata_block->data[ entry_offset ] ),
		 block_descriptor->block_number );

		byte_stream_copy_to_uint64_little_endian(
		 &( metadata_block->data[ entry_offset + 8 ] ),
		 block_descriptor->unknown );

		byte_stream_copy_to_uint64_little_endian(
		 &( metadata_block->data[ entry_offset + 16 ] ),
		 block_descriptor->checksum );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: entry: %02" PRIu32 " block number\t\t\t: %" PRIu64 "\n",
			 function,
			 entry_index,
			 block_descriptor->block_number );

			libcnotify_printf(
			 "%s: entry: %02" PRIu32 " unknown1\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 entry_index,
			 block_descriptor->unknown );

			libcnotify_printf(
			 "%s: entry: %02" PRIu32 " checksum\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 entry_index,
			 block_descriptor->checksum );

			libcnotify_printf(
			 "\n" );
		}
#endif
		if( libcdata_array_append_entry(
		     level1_metadata->level2_metadata_block_descriptors_array,
		     &array_entry_index,
		     (intptr_t *) block_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append block descriptor to array.",
			 function );

			goto on_error;
		}
		block_descriptor = NULL;
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
	if( block_descriptor != NULL )
	{
		libfsrefs_block_descriptor_free(
		 &block_descriptor,
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

/* Retrieves the number of level 2 metadata block descriptors
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_level1_metadata_get_number_of_level2_metadata_block_descriptors(
     libfsrefs_level1_metadata_t *level1_metadata,
     int *number_of_block_descriptors,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_level1_metadata_get_number_of_level2_metadata_block_descriptors";

	if( level1_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid level 1 metadata.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     level1_metadata->level2_metadata_block_descriptors_array,
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

/* Retrieves a specific level 2 metadata block descriptor
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_level1_metadata_get_level2_metadata_block_descriptor_by_index(
     libfsrefs_level1_metadata_t *level1_metadata,
     int block_descriptor_index,
     libfsrefs_block_descriptor_t **block_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_level1_metadata_get_level2_metadata_block_descriptor_by_index";

	if( level1_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid level 1 metadata.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     level1_metadata->level2_metadata_block_descriptors_array,
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

