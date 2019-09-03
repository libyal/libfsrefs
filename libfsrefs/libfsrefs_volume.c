/*
 * Volume functions
 *
 * Copyright (C) 2012-2019, Joachim Metz <joachim.metz@gmail.com>
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
#include <memory.h>
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#include "libfsrefs_block_descriptor.h"
#include "libfsrefs_block_descriptors.h"
#include "libfsrefs_debug.h"
#include "libfsrefs_definitions.h"
#include "libfsrefs_directory.h"
#include "libfsrefs_io_handle.h"
#include "libfsrefs_level0_metadata.h"
#include "libfsrefs_level1_metadata.h"
#include "libfsrefs_level2_metadata.h"
#include "libfsrefs_level3_metadata.h"
#include "libfsrefs_libbfio.h"
#include "libfsrefs_libcerror.h"
#include "libfsrefs_libcnotify.h"
#include "libfsrefs_libfcache.h"
#include "libfsrefs_libfdata.h"
#include "libfsrefs_metadata_block.h"
#include "libfsrefs_volume.h"
#include "libfsrefs_volume_name.h"

/* Creates a volume
 * Make sure the value volume is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_volume_initialize(
     libfsrefs_volume_t **volume,
     libcerror_error_t **error )
{
	libfsrefs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsrefs_volume_initialize";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	if( *volume != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume value already set.",
		 function );

		return( -1 );
	}
	internal_volume = memory_allocate_structure(
	                   libfsrefs_internal_volume_t );

	if( internal_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create volume.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_volume,
	     0,
	     sizeof( libfsrefs_internal_volume_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear volume.",
		 function );

		goto on_error;
	}
	if( libfsrefs_io_handle_initialize(
	     &( internal_volume->io_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create IO handle.",
		 function );

		goto on_error;
	}
	*volume = (libfsrefs_volume_t *) internal_volume;

	return( 1 );

on_error:
	if( internal_volume != NULL )
	{
		memory_free(
		 internal_volume );
	}
	return( -1 );
}

/* Frees a volume
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_volume_free(
     libfsrefs_volume_t **volume,
     libcerror_error_t **error )
{
	libfsrefs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsrefs_volume_free";
	int result                                   = 1;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	if( *volume != NULL )
	{
		internal_volume = (libfsrefs_internal_volume_t *) *volume;

		if( internal_volume->file_io_handle != NULL )
		{
			if( libfsrefs_volume_close(
			     *volume,
			     error ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close volume.",
				 function );

				result = -1;
			}
		}
		*volume = NULL;

		if( libfsrefs_io_handle_free(
		     &( internal_volume->io_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free IO handle.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_volume );
	}
	return( result );
}

/* Signals the volume to abort its current activity
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_volume_signal_abort(
     libfsrefs_volume_t *volume,
     libcerror_error_t **error )
{
	libfsrefs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsrefs_volume_signal_abort";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsrefs_internal_volume_t *) volume;

	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
	internal_volume->io_handle->abort = 1;

	return( 1 );
}

/* Opens a volume
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_volume_open(
     libfsrefs_volume_t *volume,
     const char *filename,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle             = NULL;
	libfsrefs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsrefs_volume_open";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsrefs_internal_volume_t *) volume;

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBFSREFS_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBFSREFS_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBFSREFS_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set track offsets read in file IO handle.",
                 function );

		goto on_error;
	}
#endif
	if( libbfio_file_set_name(
	     file_io_handle,
	     filename,
	     narrow_string_length(
	      filename ) + 1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set filename in file IO handle.",
                 function );

		goto on_error;
	}
	if( libfsrefs_volume_open_file_io_handle(
	     volume,
	     file_io_handle,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open volume: %s.",
		 function,
		 filename );

		goto on_error;
	}
	internal_volume->file_io_handle_created_in_library = 1;

	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
        return( -1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Opens a volume
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_volume_open_wide(
     libfsrefs_volume_t *volume,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle             = NULL;
	libfsrefs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsrefs_volume_open_wide";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsrefs_internal_volume_t *) volume;

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBFSREFS_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBFSREFS_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBFSREFS_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set track offsets read in file IO handle.",
                 function );

		goto on_error;
	}
#endif
	if( libbfio_file_set_name_wide(
	     file_io_handle,
	     filename,
	     wide_string_length(
	      filename ) + 1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set filename in file IO handle.",
                 function );

		goto on_error;
	}
	if( libfsrefs_volume_open_file_io_handle(
	     volume,
	     file_io_handle,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open volume: %ls.",
		 function,
		 filename );

		goto on_error;
	}
	internal_volume->file_io_handle_created_in_library = 1;

	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
        return( -1 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Opens a volume using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_volume_open_file_io_handle(
     libfsrefs_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error )
{
	libfsrefs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsrefs_volume_open_file_io_handle";
	int bfio_access_flags                        = 0;
	int file_io_handle_is_open                   = 0;
	int file_io_handle_opened_in_library         = 0;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsrefs_internal_volume_t *) volume;

	if( internal_volume->file_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume - file IO handle already set.",
		 function );

		return( -1 );
	}
	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBFSREFS_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBFSREFS_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBFSREFS_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBFSREFS_ACCESS_FLAG_READ ) != 0 )
	{
		bfio_access_flags = LIBBFIO_ACCESS_FLAG_READ;
	}
	file_io_handle_is_open = libbfio_handle_is_open(
	                          file_io_handle,
	                          error );

	if( file_io_handle_is_open == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open volume.",
		 function );

		goto on_error;
	}
	else if( file_io_handle_is_open == 0 )
	{
		if( libbfio_handle_open(
		     file_io_handle,
		     bfio_access_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open file IO handle.",
			 function );

			goto on_error;
		}
		file_io_handle_opened_in_library = 1;
	}
	if( libfsrefs_volume_open_read(
	     internal_volume,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read from file IO handle.",
		 function );

		goto on_error;
	}
	internal_volume->file_io_handle                   = file_io_handle;
	internal_volume->file_io_handle_opened_in_library = file_io_handle_opened_in_library;

	return( 1 );

on_error:
	if( file_io_handle_opened_in_library != 0 )
	{
		libbfio_handle_close(
		 file_io_handle,
		 error );
	}
	return( -1 );
}

/* Closes a volume
 * Returns 0 if successful or -1 on error
 */
int libfsrefs_volume_close(
     libfsrefs_volume_t *volume,
     libcerror_error_t **error )
{
	libfsrefs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsrefs_volume_close";
	int result                                   = 0;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsrefs_internal_volume_t *) volume;

	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( internal_volume->file_io_handle_created_in_library != 0 )
		{
			if( libfsrefs_debug_print_read_offsets(
			     internal_volume->file_io_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print the read offsets.",
				 function );

				result = -1;
			}
		}
	}
#endif
	if( internal_volume->file_io_handle_opened_in_library != 0 )
	{
		if( libbfio_handle_close(
		     internal_volume->file_io_handle,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close file IO handle.",
			 function );

			result = -1;
		}
		internal_volume->file_io_handle_opened_in_library = 0;
	}
	if( internal_volume->file_io_handle_created_in_library != 0 )
	{
		if( libbfio_handle_free(
		     &( internal_volume->file_io_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file IO handle.",
			 function );

			result = -1;
		}
		internal_volume->file_io_handle_created_in_library = 0;
	}
	internal_volume->file_io_handle = NULL;

	if( libfsrefs_io_handle_clear(
	     internal_volume->io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		result = -1;
	}
	return( result );
}

/* Opens a volume for reading
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_volume_open_read(
     libfsrefs_internal_volume_t *internal_volume,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libfsrefs_directory_t *directory                               = NULL;
	libfsrefs_directory_t *root_directory                          = NULL;
	libfsrefs_block_descriptor_t *level2_metadata_block_descriptor = NULL;
	libfsrefs_block_descriptor_t *level3_metadata_block_descriptor = NULL;
	libfsrefs_block_descriptor_t *level4_metadata_block_descriptor = NULL;
	libfsrefs_block_descriptors_t *directory_block_descriptors     = NULL;
	libfsrefs_level0_metadata_t *level0_metadata                   = NULL;
	libfsrefs_level1_metadata_t *level1_primary_metadata           = NULL;
	libfsrefs_level1_metadata_t *level1_secondary_metadata         = NULL;
	libfsrefs_level2_metadata_t *level2_metadata                   = NULL;
	libfsrefs_level3_metadata_t *level3_metadata                   = NULL;
	static char *function                                          = "libfsrefs_volume_open_read";
	uint64_t value_identifier                                      = 0;
	int level2_metadata_block_descriptor_index                     = 0;
	int level3_metadata_block_descriptor_index                     = 0;
	int level4_metadata_block_descriptor_index                     = 0;
	int number_of_level2_metadata_block_descriptors                = 0;
	int number_of_level3_metadata_block_descriptors                = 0;
	int number_of_level4_metadata_block_descriptors                = 0;

	if( internal_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal volume.",
		 function );

		return( -1 );
	}
	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal volume - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading volume header:\n" );
	}
#endif
	if( libfsrefs_io_handle_read_volume_header(
	     internal_volume->io_handle,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume header.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading level 0 metadata:\n" );
	}
#endif
	if( libfsrefs_level0_metadata_initialize(
	     &level0_metadata,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create level 0 metadata.",
		 function );

		goto on_error;
	}
	if( libfsrefs_level0_metadata_read(
	     level0_metadata,
	     internal_volume->io_handle,
	     file_io_handle,
	     0x0000001eUL * internal_volume->io_handle->metadata_block_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read level 0 metadata.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading level 1 primary metadata:\n" );
	}
#endif
	if( libfsrefs_level1_metadata_initialize(
	     &level1_primary_metadata,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create level 1 primary metadata.",
		 function );

		goto on_error;
	}
	if( libfsrefs_level1_metadata_read(
	     level1_primary_metadata,
	     internal_volume->io_handle,
	     file_io_handle,
	     level0_metadata->primary_level1_metadata_block_number * internal_volume->io_handle->metadata_block_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read level 1 primary metadata.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading level 1 secondary metadata:\n" );
	}
#endif
	if( libfsrefs_level1_metadata_initialize(
	     &level1_secondary_metadata,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create level 1 secondary metadata.",
		 function );

		goto on_error;
	}
	if( libfsrefs_level1_metadata_read(
	     level1_secondary_metadata,
	     internal_volume->io_handle,
	     file_io_handle,
	     level0_metadata->secondary_level1_metadata_block_number * internal_volume->io_handle->metadata_block_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read level 1 secondary metadata.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading level 2 metadata:\n" );
	}
#endif
	if( level1_primary_metadata->sequence_number >= level1_secondary_metadata->sequence_number )
	{
		if( libfsrefs_level1_metadata_get_number_of_level2_metadata_block_descriptors(
		     level1_primary_metadata,
		     &number_of_level2_metadata_block_descriptors,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of level 2 metadata block descriptors from primary level 1 metadata.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( libfsrefs_level1_metadata_get_number_of_level2_metadata_block_descriptors(
		     level1_secondary_metadata,
		     &number_of_level2_metadata_block_descriptors,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of level 2 metadata block descriptors from secondary level 1 metadata.",
			 function );

			goto on_error;
		}
	}
	for( level2_metadata_block_descriptor_index = 0;
	     level2_metadata_block_descriptor_index < number_of_level2_metadata_block_descriptors;
	     level2_metadata_block_descriptor_index++ )
	{
		if( level1_primary_metadata->sequence_number >= level1_secondary_metadata->sequence_number )
		{
			if( libfsrefs_level1_metadata_get_level2_metadata_block_descriptor_by_index(
			     level1_primary_metadata,
			     level2_metadata_block_descriptor_index,
			     &level2_metadata_block_descriptor,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve level 2 metadata block descriptor: %d from primary level 1 metadata.",
				 function,
				 level2_metadata_block_descriptor_index );

				goto on_error;
			}
		}
		else
		{
			if( libfsrefs_level1_metadata_get_level2_metadata_block_descriptor_by_index(
			     level1_secondary_metadata,
			     level2_metadata_block_descriptor_index,
			     &level2_metadata_block_descriptor,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve level 2 metadata block descriptor: %d from secondary level 1 metadata.",
				 function,
				 level2_metadata_block_descriptor_index );

				goto on_error;
			}
		}
		if( level2_metadata_block_descriptor == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing level 2 metadata block descriptor: %d.",
			 function,
			 level2_metadata_block_descriptor_index );

			goto on_error;
		}
		if( libfsrefs_level2_metadata_initialize(
		     &level2_metadata,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create level 2 metadata.",
			 function );

			goto on_error;
		}
		if( libfsrefs_level2_metadata_read(
		     level2_metadata,
		     internal_volume->io_handle,
		     file_io_handle,
		     level2_metadata_block_descriptor->block_number * internal_volume->io_handle->metadata_block_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read level 2 metadata block: %" PRIu64 ".",
			 function,
			 level2_metadata_block_descriptor->block_number );

			goto on_error;
		}
		if( libfsrefs_level2_metadata_get_number_of_block_descriptors(
		     level2_metadata,
		     &number_of_level3_metadata_block_descriptors,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of level 3 metadata block descriptors.",
			 function );

			goto on_error;
		}
		for( level3_metadata_block_descriptor_index = 0;
		     level3_metadata_block_descriptor_index < number_of_level3_metadata_block_descriptors;
		     level3_metadata_block_descriptor_index++ )
		{
			if( libfsrefs_level2_metadata_get_block_descriptor_by_index(
			     level2_metadata,
			     level3_metadata_block_descriptor_index,
			     &level3_metadata_block_descriptor,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve level 3 metadata block descriptor: %d.",
				 function,
				 level3_metadata_block_descriptor_index );

				goto on_error;
			}
			if( level3_metadata_block_descriptor == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing level 3 metadata block descriptor: %d.",
				 function,
				 level3_metadata_block_descriptor_index );

				goto on_error;
			}
			if( level3_metadata_block_descriptor->identifier_data_size == 16 )
			{
				byte_stream_copy_to_uint64_little_endian(
				 &( level3_metadata_block_descriptor->identifier_data[ 8 ] ),
				 value_identifier );
			}
			else
			{
				value_identifier = 0;
			}
			if( value_identifier == 0x00000600UL )
			{
				if( libfsrefs_directory_initialize(
				     &root_directory,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create root directory.",
					 function );

					goto on_error;
				}
				if( libfsrefs_directory_read(
				     root_directory,
				     internal_volume->io_handle,
				     file_io_handle,
				     level3_metadata_block_descriptor->block_number * internal_volume->io_handle->metadata_block_size,
				     4,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read root directory block: %" PRIu64 ".",
					 function,
					 level3_metadata_block_descriptor->block_number );

					goto on_error;
				}
				if( libfsrefs_directory_free(
				     &root_directory,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free root directory.",
					 function );

					goto on_error;
				}
			}
			else if( value_identifier == 0x00000701UL )
			{
				if( libfsrefs_directory_initialize(
				     &directory,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create directory.",
					 function );

					goto on_error;
				}
				if( libfsrefs_directory_read(
				     directory,
				     internal_volume->io_handle,
				     file_io_handle,
				     level3_metadata_block_descriptor->block_number * internal_volume->io_handle->metadata_block_size,
				     4,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read directory block: %" PRIu64 ".",
					 function,
					 level4_metadata_block_descriptor->block_number );

					goto on_error;
				}
				if( libfsrefs_directory_free(
				     &directory,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free directory.",
					 function );

					goto on_error;
				}
#ifdef TODO
				if( libfsrefs_block_descriptors_initialize(
				     &directory_block_descriptors,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create directory block descriptors.",
					 function );

					goto on_error;
				}
				if( libfsrefs_block_descriptors_read(
				     directory_block_descriptors,
				     internal_volume->io_handle,
				     file_io_handle,
				     level3_metadata_block_descriptor->block_number * internal_volume->io_handle->metadata_block_size,
				     4,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read level 2 metadata block: %" PRIu64 ".",
					 function,
					 level3_metadata_block_descriptor->block_number );

					goto on_error;
				}
				if( libfsrefs_block_descriptors_get_number_of_block_descriptors(
				     directory_block_descriptors,
				     &number_of_level4_metadata_block_descriptors,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve number of level 4 metadata block descriptors.",
					 function );

					goto on_error;
				}
				for( level4_metadata_block_descriptor_index = 0;
				     level4_metadata_block_descriptor_index < number_of_level4_metadata_block_descriptors;
				     level4_metadata_block_descriptor_index++ )
				{
					if( libfsrefs_block_descriptors_get_block_descriptor_by_index(
					     directory_block_descriptors,
					     level4_metadata_block_descriptor_index,
					     &level4_metadata_block_descriptor,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to retrieve level 4 metadata block descriptor: %d.",
						 function,
						 level4_metadata_block_descriptor_index );

						goto on_error;
					}
					if( level4_metadata_block_descriptor == NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
						 "%s: missing level 4 metadata block descriptor: %d.",
						 function,
						 level4_metadata_block_descriptor_index );

						goto on_error;
					}
					if( libfsrefs_directory_initialize(
					     &directory,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
						 "%s: unable to create directory.",
						 function );

						goto on_error;
					}
					if( libfsrefs_directory_read(
					     directory,
					     internal_volume->io_handle,
					     file_io_handle,
					     level4_metadata_block_descriptor->block_number * internal_volume->io_handle->metadata_block_size,
					     5,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_READ_FAILED,
						 "%s: unable to read directory block: %" PRIu64 ".",
						 function,
						 level4_metadata_block_descriptor->block_number );

						goto on_error;
					}
					if( libfsrefs_directory_free(
					     &directory,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
						 "%s: unable to free directory.",
						 function );

						goto on_error;
					}
				}
				if( libfsrefs_block_descriptors_free(
				     &directory_block_descriptors,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free directory block descriptors.",
					 function );

					goto on_error;
				}
#endif /* TODO */
			}
			else
			{
				if( libfsrefs_level3_metadata_initialize(
				     &level3_metadata,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create level 3 metadata.",
					 function );

					goto on_error;
				}
				if( libfsrefs_level3_metadata_read(
				     level3_metadata,
				     internal_volume->io_handle,
				     file_io_handle,
				     level3_metadata_block_descriptor->block_number * internal_volume->io_handle->metadata_block_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read level 3 metadata block: %" PRIu64 ".",
					 function,
					 level3_metadata_block_descriptor->block_number );

					goto on_error;
				}
				if( libfsrefs_level3_metadata_free(
				     &level3_metadata,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free level 3 metadata.",
					 function );

					goto on_error;
				}
			}
		}
		if( libfsrefs_level2_metadata_free(
		     &level2_metadata,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free level 2 metadata.",
			 function );

			goto on_error;
		}
	}
	if( libfsrefs_level1_metadata_free(
	     &level1_secondary_metadata,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free level 1 secondary metadata.",
		 function );

		goto on_error;
	}
	if( libfsrefs_level1_metadata_free(
	     &level1_primary_metadata,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free level 1 primary metadata.",
		 function );

		goto on_error;
	}
	if( libfsrefs_level0_metadata_free(
	     &level0_metadata,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free level 0 metadata.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( directory != NULL )
	{
		libfsrefs_directory_free(
		 &directory,
		 NULL );
	}
	if( directory_block_descriptors != NULL )
	{
		libfsrefs_block_descriptors_free(
		 &directory_block_descriptors,
		 NULL );
	}
	if( level3_metadata != NULL )
	{
		libfsrefs_level3_metadata_free(
		 &level3_metadata,
		 NULL );
	}
	if( root_directory != NULL )
	{
		libfsrefs_directory_free(
		 &root_directory,
		 NULL );
	}
	if( level2_metadata != NULL )
	{
		libfsrefs_level2_metadata_free(
		 &level2_metadata,
		 NULL );
	}
	if( level1_secondary_metadata != NULL )
	{
		libfsrefs_level1_metadata_free(
		 &level1_secondary_metadata,
		 NULL );
	}
	if( level1_primary_metadata != NULL )
	{
		libfsrefs_level1_metadata_free(
		 &level1_primary_metadata,
		 NULL );
	}
	if( level0_metadata != NULL )
	{
		libfsrefs_level0_metadata_free(
		 &level0_metadata,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_volume_get_utf8_name_size(
     libfsrefs_volume_t *volume,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libfsrefs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsrefs_volume_get_utf8_name_size";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsrefs_internal_volume_t *) volume;

/* TODO get volume name value */

	if( libfsrefs_volume_name_get_utf8_name_size(
	     NULL,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-8 name from volume name attribute.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded name value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_volume_get_utf8_name(
     libfsrefs_volume_t *volume,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libfsrefs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsrefs_volume_get_utf8_name";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsrefs_internal_volume_t *) volume;

/* TODO get volume name value */

	if( libfsrefs_volume_name_get_utf8_name(
	     NULL,
	     utf8_string,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 name from volume name attribute.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_volume_get_utf16_name_size(
     libfsrefs_volume_t *volume,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libfsrefs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsrefs_volume_get_utf16_name_size";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsrefs_internal_volume_t *) volume;

/* TODO get volume name value */

	if( libfsrefs_volume_name_get_utf16_name_size(
	     NULL,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-16 name from volume name attribute.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded name value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_volume_get_utf16_name(
     libfsrefs_volume_t *volume,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libfsrefs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsrefs_volume_get_utf16_name";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsrefs_internal_volume_t *) volume;

/* TODO get volume name value */

	if( libfsrefs_volume_name_get_utf16_name(
	     NULL,
	     utf16_string,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 name from volume name attribute.",
		 function );

		return( -1 );
	}
	return( 1 );
}

