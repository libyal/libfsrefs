/*
 * The REFS volume header functions
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

#include "libfsrefs_io_handle.h"
#include "libfsrefs_libbfio.h"
#include "libfsrefs_libcerror.h"
#include "libfsrefs_libcnotify.h"
#include "libfsrefs_volume_header.h"

#include "fsrefs_volume_header.h"

/* Creates a volume header
 * Make sure the value volume_header is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_volume_header_initialize(
     libfsrefs_volume_header_t **volume_header,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_volume_header_initialize";

	if( volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume header.",
		 function );

		return( -1 );
	}
	if( *volume_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume header value already set.",
		 function );

		return( -1 );
	}
	*volume_header = memory_allocate_structure(
	                  libfsrefs_volume_header_t );

	if( *volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create volume header.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *volume_header,
	     0,
	     sizeof( libfsrefs_volume_header_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear volume header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *volume_header != NULL )
	{
		memory_free(
		 *volume_header );

		*volume_header = NULL;
	}
	return( -1 );
}

/* Frees a volume header
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_volume_header_free(
     libfsrefs_volume_header_t **volume_header,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_volume_header_free";

	if( volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume header.",
		 function );

		return( -1 );
	}
	if( *volume_header != NULL )
	{
		memory_free(
		 *volume_header );

		*volume_header = NULL;
	}
	return( 1 );
}

/* Reads the volume header
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_volume_header_read_data(
     libfsrefs_volume_header_t *volume_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function      = "libfsrefs_volume_header_read_data";
	uint64_t number_of_sectors = 0;
	uint32_t sectors_per_block = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit       = 0;
	uint32_t value_32bit       = 0;
	uint16_t value_16bit       = 0;
#endif

	if( volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume header.",
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
	if( ( data_size < sizeof( fsrefs_volume_header_t ) )
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
		 "%s: volume header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsrefs_volume_header_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( memory_compare(
	     ( (fsrefs_volume_header_t *) data )->file_system_signature,
	     fsrefs_volume_file_system_signature,
	     8 ) != 0 )
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
	 ( (fsrefs_volume_header_t *) data )->number_of_sectors,
	 number_of_sectors );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsrefs_volume_header_t *) data )->bytes_per_sector,
	 volume_header->bytes_per_sector );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsrefs_volume_header_t *) data )->sectors_per_block,
	 sectors_per_block );

	volume_header->major_format_version = ( (fsrefs_volume_header_t *) data )->major_format_version;
	volume_header->minor_format_version = ( (fsrefs_volume_header_t *) data )->minor_format_version;

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsrefs_volume_header_t *) data )->volume_serial_number,
	 volume_header->volume_serial_number );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: unknown1\n",
		 function );
		libcnotify_print_data(
		 ( (fsrefs_volume_header_t *) data )->unknown1,
		 3,
		 0 );

		libcnotify_printf(
		 "%s: file system signature\t\t: %c%c%c%c\n",
		 function,
		 ( (fsrefs_volume_header_t *) data )->file_system_signature[ 0 ],
		 ( (fsrefs_volume_header_t *) data )->file_system_signature[ 1 ],
		 ( (fsrefs_volume_header_t *) data )->file_system_signature[ 2 ],
		 ( (fsrefs_volume_header_t *) data )->file_system_signature[ 3 ] );

		libcnotify_printf(
		 "%s: unknown2\n",
		 function );
		libcnotify_print_data(
		 ( (fsrefs_volume_header_t *) data )->unknown2,
		 9,
		 0 );

		libcnotify_printf(
		 "%s: unknown3\t\t\t\t: %c%c%c%c\n",
		 function,
		 ( (fsrefs_volume_header_t *) data )->unknown3[ 0 ],
		 ( (fsrefs_volume_header_t *) data )->unknown3[ 1 ],
		 ( (fsrefs_volume_header_t *) data )->unknown3[ 2 ],
		 ( (fsrefs_volume_header_t *) data )->unknown3[ 3 ] );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsrefs_volume_header_t *) data )->unknown4,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown4\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsrefs_volume_header_t *) data )->checksum,
		 value_16bit );
		libcnotify_printf(
		 "%s: checksum\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: number of sectors\t\t\t: %" PRIu64 "\n",
		 function,
		 number_of_sectors );

		libcnotify_printf(
		 "%s: bytes per sector\t\t\t: %" PRIu32 "\n",
		 function,
		 volume_header->bytes_per_sector );

		libcnotify_printf(
		 "%s: sectors per block\t\t\t: %" PRIu32 "\n",
		 function,
		 sectors_per_block );

		libcnotify_printf(
		 "%s: major format version\t\t\t: %" PRIu8 "\n",
		 function,
		 volume_header->major_format_version );

		libcnotify_printf(
		 "%s: minor format version\t\t\t: %" PRIu8 "\n",
		 function,
		 volume_header->minor_format_version );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsrefs_volume_header_t *) data )->unknown7,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown7\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsrefs_volume_header_t *) data )->unknown8,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown8\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_volume_header_t *) data )->unknown9,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown9\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "%s: volume serial number\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 volume_header->volume_serial_number );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_volume_header_t *) data )->unknown10,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown10\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "%s: unknown11\n",
		 function );
		libcnotify_print_data(
		 ( (fsrefs_volume_header_t *) data )->unknown11,
		 440,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( ( volume_header->bytes_per_sector != 256 )
	 && ( volume_header->bytes_per_sector != 512 )
	 && ( volume_header->bytes_per_sector != 1024 )
	 && ( volume_header->bytes_per_sector != 2048 )
	 && ( volume_header->bytes_per_sector != 4096 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported bytes per sector: %" PRIu16 ".",
		 function,
		 volume_header->bytes_per_sector );

		return( -1 );
	}
	if( number_of_sectors > (size64_t) ( ( UINT64_MAX / volume_header->bytes_per_sector ) + 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of sectors value out of bounds.",
		 function );

		return( -1 );
	}
	volume_header->volume_size  = number_of_sectors * volume_header->bytes_per_sector;
	volume_header->volume_size += volume_header->bytes_per_sector;

	if( sectors_per_block > (size32_t) ( UINT32_MAX / volume_header->bytes_per_sector ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid sectors per block value out of bounds.",
		 function );

		return( -1 );
	}
	volume_header->block_size = (size32_t) sectors_per_block * volume_header->bytes_per_sector;

	if( ( volume_header->block_size != 4096 )
	 && ( volume_header->block_size != 65536 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported block size: %" PRIu32 ".",
		 function,
		 volume_header->block_size );

		return( -1 );
	}
	if( ( volume_header->major_format_version != 1 )
	 && ( volume_header->major_format_version != 3 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format version: %" PRIu8 ".%" PRIu8 ".",
		 function,
		 volume_header->major_format_version,
		 volume_header->minor_format_version );

		return( -1 );
	}
	if( volume_header->major_format_version == 1 )
	{
		volume_header->metadata_block_size = 16 * 1024;
	}
	else
	{
		volume_header->metadata_block_size = volume_header->block_size;
	}

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: metadata block size\t\t\t: %" PRIu32 "\n",
		 function,
		 volume_header->metadata_block_size );

		libcnotify_printf(
		 "%s: block size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 volume_header->block_size );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

/* Reads the volume header
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_volume_header_read_file_io_handle(
     libfsrefs_volume_header_t *volume_header,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	uint8_t volume_header_data[ sizeof( fsrefs_volume_header_t ) ];

	static char *function = "libfsrefs_volume_header_read_file_io_handle";
	ssize_t read_count    = 0;

	if( volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume header.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading volume header at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              volume_header_data,
	              sizeof( fsrefs_volume_header_t ),
	              file_offset,
	              error );

	if( read_count != (ssize_t) sizeof( fsrefs_volume_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume header data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	if( libfsrefs_volume_header_read_data(
	     volume_header,
	     volume_header_data,
	     sizeof( fsrefs_volume_header_t ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume header data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the volume size
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_volume_header_get_volume_size(
     libfsrefs_volume_header_t *volume_header,
     size64_t *volume_size,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_volume_header_get_volume_size";

	if( volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume header.",
		 function );

		return( -1 );
	}
	if( volume_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume size.",
		 function );

		return( -1 );
	}
	*volume_size = volume_header->volume_size;

	return( 1 );
}

