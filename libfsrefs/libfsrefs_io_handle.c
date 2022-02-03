/*
 * Input/Output (IO) handle functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libfsrefs_debug.h"
#include "libfsrefs_definitions.h"
#include "libfsrefs_io_handle.h"
#include "libfsrefs_libbfio.h"
#include "libfsrefs_libcerror.h"
#include "libfsrefs_libcnotify.h"
#include "libfsrefs_libfcache.h"
#include "libfsrefs_libfdata.h"
#include "libfsrefs_unused.h"

#include "fsrefs_volume_header.h"

const char fsrefs_volume_file_system_signature[ 4 ] = "ReFS";

/* Creates an IO handle
 * Make sure the value io_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_io_handle_initialize(
     libfsrefs_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_io_handle_initialize";

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
	if( *io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid IO handle value already set.",
		 function );

		return( -1 );
	}
	*io_handle = memory_allocate_structure(
	              libfsrefs_io_handle_t );

	if( *io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create IO handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *io_handle,
	     0,
	     sizeof( libfsrefs_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *io_handle != NULL )
	{
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( -1 );
}

/* Frees an IO handle
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_io_handle_free(
     libfsrefs_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_io_handle_free";
	int result            = 1;

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
	if( *io_handle != NULL )
	{
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( result );
}

/* Clears the IO handle
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_io_handle_clear(
     libfsrefs_io_handle_t *io_handle,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_io_handle_clear";

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
	if( memory_set(
	     io_handle,
	     0,
	     sizeof( libfsrefs_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads the volume header
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_io_handle_read_volume_header(
     libfsrefs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	fsrefs_volume_header_t volume_header_data;

	static char *function      = "libfsrefs_io_handle_read_volume_header";
	ssize_t read_count         = 0;
	uint32_t sectors_per_block = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit  = 0;
	uint32_t value_32bit  = 0;
	uint16_t value_16bit  = 0;
#endif

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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading volume header at offset: 0 (0x00000000)\n",
		 function );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              (uint8_t *) &volume_header_data,
	              sizeof( fsrefs_volume_header_t ),
	              0,
	              error );

	if( read_count != (ssize_t) sizeof( fsrefs_volume_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume header data at offset: 0 (0x00000000).",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: volume header:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) &volume_header_data,
		 sizeof( fsrefs_volume_header_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( memory_compare(
	     volume_header_data.file_system_signature,
	     fsrefs_volume_file_system_signature,
	     4 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid volume system signature.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint64_little_endian(
	 volume_header_data.number_of_sectors,
	 io_handle->number_of_sectors );

	byte_stream_copy_to_uint32_little_endian(
	 volume_header_data.bytes_per_sector,
	 io_handle->bytes_per_sector );

	byte_stream_copy_to_uint32_little_endian(
	 volume_header_data.sectors_per_block,
	 sectors_per_block );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: unknown1\n",
		 function );
		libcnotify_print_data(
		 volume_header_data.unknown1,
		 3,
		 0 );

		libcnotify_printf(
		 "%s: file system signature\t\t: %c%c%c%c\n",
		 function,
		 volume_header_data.file_system_signature[ 0 ],
		 volume_header_data.file_system_signature[ 1 ],
		 volume_header_data.file_system_signature[ 2 ],
		 volume_header_data.file_system_signature[ 3 ] );

		libcnotify_printf(
		 "%s: unknown2\n",
		 function );
		libcnotify_print_data(
		 volume_header_data.unknown2,
		 9,
		 0 );

		libcnotify_printf(
		 "%s: unknown3\t\t\t: %c%c%c%c\n",
		 function,
		 volume_header_data.unknown3[ 0 ],
		 volume_header_data.unknown3[ 1 ],
		 volume_header_data.unknown3[ 2 ],
		 volume_header_data.unknown3[ 3 ] );

		byte_stream_copy_to_uint16_little_endian(
		 volume_header_data.unknown4,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown4\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 volume_header_data.unknown5,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown5\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: number of sectors\t\t: %" PRIu64 "\n",
		 function,
		 io_handle->number_of_sectors );

		libcnotify_printf(
		 "%s: bytes per sector\t\t: %" PRIu32 "\n",
		 function,
		 io_handle->bytes_per_sector );

		libcnotify_printf(
		 "%s: sectors per block\t\t: %" PRIu32 "\n",
		 function,
		 sectors_per_block );

		byte_stream_copy_to_uint32_little_endian(
		 volume_header_data.unknown7,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown7\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 volume_header_data.unknown8,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown8\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint64_little_endian(
		 volume_header_data.unknown9,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown9\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 volume_header_data.unknown10,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown10\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );
	}
#endif
	io_handle->block_size          = (size_t) io_handle->bytes_per_sector * (size_t) sectors_per_block;
	io_handle->metadata_block_size = 16 * 1024;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: unknown11\n",
		 function );
		libcnotify_print_data(
		 volume_header_data.unknown11,
		 15872,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	return( 1 );
}

