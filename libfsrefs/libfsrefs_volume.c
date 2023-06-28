/*
 * Volume functions
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
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#include "libfsrefs_block_descriptor.h"
#include "libfsrefs_debug.h"
#include "libfsrefs_definitions.h"
#include "libfsrefs_file_entry.h"
#include "libfsrefs_file_system.h"
#include "libfsrefs_io_handle.h"
#include "libfsrefs_libbfio.h"
#include "libfsrefs_libcerror.h"
#include "libfsrefs_libcnotify.h"
#include "libfsrefs_libcthreads.h"
#include "libfsrefs_libfcache.h"
#include "libfsrefs_libfdata.h"
#include "libfsrefs_libuna.h"
#include "libfsrefs_ministore_node.h"
#include "libfsrefs_node_record.h"
#include "libfsrefs_objects_tree.h"
#include "libfsrefs_volume.h"
#include "libfsrefs_volume_header.h"

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

		memory_free(
		 internal_volume );

		return( -1 );
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
#if defined( HAVE_LIBFSREFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_volume->read_write_lock ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize read/write lock.",
		 function );

		goto on_error;
	}
#endif
	*volume = (libfsrefs_volume_t *) internal_volume;

	return( 1 );

on_error:
	if( internal_volume != NULL )
	{
		if( internal_volume->io_handle != NULL )
		{
			libfsrefs_io_handle_free(
			 &( internal_volume->io_handle ),
			 NULL );
		}
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

#if defined( HAVE_LIBFSREFS_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( internal_volume->read_write_lock ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free read/write lock.",
			 function );

			result = -1;
		}
#endif
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
#if defined( HAVE_LIBFSREFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		goto on_error;
	}
#endif
	internal_volume->file_io_handle_created_in_library = 1;

#if defined( HAVE_LIBFSREFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		internal_volume->file_io_handle_created_in_library = 0;

		goto on_error;
	}
#endif
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
#if defined( HAVE_LIBFSREFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		goto on_error;
	}
#endif
	internal_volume->file_io_handle_created_in_library = 1;

#if defined( HAVE_LIBFSREFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		internal_volume->file_io_handle_created_in_library = 0;

		goto on_error;
	}
#endif
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
	if( libfsrefs_internal_volume_open_read(
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
#if defined( HAVE_LIBFSREFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		goto on_error;
	}
#endif
	internal_volume->file_io_handle                   = file_io_handle;
	internal_volume->file_io_handle_opened_in_library = (uint8_t) file_io_handle_opened_in_library;

#if defined( HAVE_LIBFSREFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		internal_volume->file_io_handle                   = NULL;
		internal_volume->file_io_handle_opened_in_library = 0;

		goto on_error;
	}
#endif
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

	if( internal_volume->file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing file IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSREFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
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
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

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
	if( internal_volume->volume_header != NULL )
	{
		if( libfsrefs_volume_header_free(
		     &( internal_volume->volume_header ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free volume header.",
			 function );

			result = -1;
		}
	}
	if( internal_volume->file_system != NULL )
	{
		if( libfsrefs_file_system_free(
		     &( internal_volume->file_system ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file system.",
			 function );

			result = -1;
		}
	}
	if( internal_volume->objects_tree != NULL )
	{
		if( libfsrefs_objects_tree_free(
		     &( internal_volume->objects_tree ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free objects tree.",
			 function );

			result = -1;
		}
	}
	if( internal_volume->volume_information_object != NULL )
	{
		if( libfsrefs_ministore_node_free(
		     &( internal_volume->volume_information_object ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free volume information object.",
			 function );

			result = -1;
		}
		internal_volume->volume_name_record = NULL;
	}
#if defined( HAVE_LIBFSREFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Opens a volume for reading
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_internal_volume_open_read(
     libfsrefs_internal_volume_t *internal_volume,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function         = "libfsrefs_internal_volume_open_read";
	off64_t superblock_offset     = 0;
	int number_of_ministore_trees = 0;

	if( internal_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
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
	if( internal_volume->file_system != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume - file system value already set.",
		 function );

		return( -1 );
	}
	if( internal_volume->objects_tree != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume - objects tree value already set.",
		 function );

		return( -1 );
	}
	if( internal_volume->volume_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume - volume header value already set.",
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
	if( libfsrefs_volume_header_initialize(
	     &( internal_volume->volume_header ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create volume header.",
		 function );

		goto on_error;
	}
	if( libfsrefs_volume_header_read_file_io_handle(
	     internal_volume->volume_header,
	     file_io_handle,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume header at offset: 0 (0x00000000).",
		 function );

		goto on_error;
	}
	internal_volume->io_handle->bytes_per_sector     = internal_volume->volume_header->bytes_per_sector;
	internal_volume->io_handle->volume_size          = internal_volume->volume_header->volume_size;
	internal_volume->io_handle->major_format_version = internal_volume->volume_header->major_format_version;
	internal_volume->io_handle->minor_format_version = internal_volume->volume_header->minor_format_version;
	internal_volume->io_handle->cluster_block_size   = internal_volume->volume_header->cluster_block_size;
	internal_volume->io_handle->metadata_block_size  = internal_volume->volume_header->metadata_block_size;
	internal_volume->io_handle->container_size       = internal_volume->volume_header->container_size;

	if( libfsrefs_file_system_initialize(
	     &( internal_volume->file_system ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file system.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading superblock:\n" );
	}
#endif
	superblock_offset = 30 * internal_volume->io_handle->metadata_block_size;

	if( libfsrefs_file_system_read_superblock(
	     internal_volume->file_system,
	     internal_volume->io_handle,
	     file_io_handle,
	     superblock_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read superblock at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 superblock_offset,
		 superblock_offset );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading checkpoints:\n" );
	}
#endif
	if( libfsrefs_file_system_read_checkpoints(
	     internal_volume->file_system,
	     internal_volume->io_handle,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read checkpoints.",
		 function );

		goto on_error;
	}
	if( libfsrefs_file_system_get_number_of_ministore_trees(
	     internal_volume->file_system,
	     &number_of_ministore_trees,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of ministore trees.",
		 function );

		goto on_error;
	}
	if( number_of_ministore_trees == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of ministore trees value out of bounds.",
		 function );

		goto on_error;
	}
	if( number_of_ministore_trees > 8 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "Reading container trees:\n" );
		}
#endif
		if( libfsrefs_file_system_read_container_trees(
		     internal_volume->file_system,
		     internal_volume->io_handle,
		     file_io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read container trees.",
			 function );

			goto on_error;
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading objects tree\n" );
	}
#endif
	if( libfsrefs_objects_tree_initialize(
	     &( internal_volume->objects_tree ),
	     internal_volume->file_system,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create objects tree.",
		 function );

		goto on_error;
	}
	if( libfsrefs_objects_tree_read(
	     internal_volume->objects_tree,
	     internal_volume->io_handle,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read objects tree.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading volume information object\n" );
	}
#endif
	if( libfsrefs_objects_get_ministore_tree_by_identifier(
	     internal_volume->objects_tree,
	     internal_volume->io_handle,
	     file_io_handle,
	     (uint64_t) 0x00000500UL,
	     &( internal_volume->volume_information_object ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve volume information object from objects tree.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( internal_volume->volume_information_object != NULL )
	{
		libfsrefs_ministore_node_free(
		 &( internal_volume->volume_information_object ),
		 NULL );
	}
	if( internal_volume->objects_tree != NULL )
	{
		libfsrefs_objects_tree_free(
		 &( internal_volume->objects_tree ),
		 NULL );
	}
	if( internal_volume->file_system != NULL )
	{
		libfsrefs_file_system_free(
		 &( internal_volume->file_system ),
		 NULL );
	}
	if( internal_volume->volume_header != NULL )
	{
		libfsrefs_volume_header_free(
		 &( internal_volume->volume_header ),
		 NULL );
	}
	return( -1 );
}

/* Retrieves the bytes per sector
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_volume_get_bytes_per_sector(
     libfsrefs_volume_t *volume,
     uint16_t *bytes_per_sector,
     libcerror_error_t **error )
{
	libfsrefs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsrefs_volume_get_bytes_per_sector";
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
	internal_volume = (libfsrefs_internal_volume_t *) volume;

#if defined( HAVE_LIBFSREFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsrefs_volume_header_get_bytes_per_sector(
	     internal_volume->volume_header,
	     bytes_per_sector,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve bytes per sector.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSREFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the cluster block size
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_volume_get_cluster_block_size(
     libfsrefs_volume_t *volume,
     size32_t *cluster_block_size,
     libcerror_error_t **error )
{
	libfsrefs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsrefs_volume_get_cluster_block_size";
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
	internal_volume = (libfsrefs_internal_volume_t *) volume;

#if defined( HAVE_LIBFSREFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsrefs_volume_header_get_cluster_block_size(
	     internal_volume->volume_header,
	     cluster_block_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve cluster block size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSREFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the volume name record
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_internal_volume_get_volume_name_record(
     libfsrefs_internal_volume_t *internal_volume,
     libcerror_error_t **error )
{
	uint8_t key_data[ 8 ];

	static char *function      = "libfsrefs_internal_volume_get_volume_name_record";
	uint64_t object_identifier = 0x00000510UL;

	if( internal_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	if( internal_volume->volume_name_record != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume - volume name record already set.",
		 function );

		return( -1 );
	}
	byte_stream_copy_from_uint64_little_endian(
	 key_data,
	 object_identifier );

	if( libfsrefs_ministore_node_get_record(
	     internal_volume->volume_information_object,
	     key_data,
	     8,
	     &( internal_volume->volume_name_record ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve identifier: 0x%08" PRIx64 " from volume information object.",
		 function,
		 object_identifier );

		return( -1 );
	}
	return( 1 );
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
	internal_volume = (libfsrefs_internal_volume_t *) volume;

#if defined( HAVE_LIBFSREFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_volume->volume_name_record == NULL )
	{
		if( libfsrefs_internal_volume_get_volume_name_record(
		     internal_volume,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve volume name record.",
			 function );

			result = -1;
		}
	}
	if( internal_volume->volume_name_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing volume name record.",
		 function );

		result = -1;
	}
	else if( libuna_utf8_string_size_from_utf16_stream(
	          internal_volume->volume_name_record->value_data,
	          internal_volume->volume_name_record->value_data_size,
	          LIBUNA_ENDIAN_LITTLE,
	          utf8_string_size,
	          error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-8 name from volume name record.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSREFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
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
	internal_volume = (libfsrefs_internal_volume_t *) volume;

#if defined( HAVE_LIBFSREFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_volume->volume_name_record == NULL )
	{
		if( libfsrefs_internal_volume_get_volume_name_record(
		     internal_volume,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve volume name record.",
			 function );

			result = -1;
		}
	}
	if( internal_volume->volume_name_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing volume name record.",
		 function );

		result = -1;
	}
	else if( libuna_utf8_string_copy_from_utf16_stream(
	          utf8_string,
	          utf8_string_size,
	          internal_volume->volume_name_record->value_data,
	          internal_volume->volume_name_record->value_data_size,
	          LIBUNA_ENDIAN_LITTLE,
	          error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 name from volume name record.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSREFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
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
	internal_volume = (libfsrefs_internal_volume_t *) volume;

#if defined( HAVE_LIBFSREFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_volume->volume_name_record == NULL )
	{
		if( libfsrefs_internal_volume_get_volume_name_record(
		     internal_volume,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve volume name record.",
			 function );

			result = -1;
		}
	}
	if( internal_volume->volume_name_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing volume name record.",
		 function );

		result = -1;
	}
	else if( libuna_utf16_string_size_from_utf16_stream(
	          internal_volume->volume_name_record->value_data,
	          internal_volume->volume_name_record->value_data_size,
	          LIBUNA_ENDIAN_LITTLE,
	          utf16_string_size,
	          error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-16 name from volume name record.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSREFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
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
	internal_volume = (libfsrefs_internal_volume_t *) volume;

#if defined( HAVE_LIBFSREFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_volume->volume_name_record == NULL )
	{
		if( libfsrefs_internal_volume_get_volume_name_record(
		     internal_volume,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve volume name record.",
			 function );

			result = -1;
		}
	}
	if( internal_volume->volume_name_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing volume name record.",
		 function );

		result = -1;
	}
	else if( libuna_utf16_string_copy_from_utf16_stream(
	          utf16_string,
	          utf16_string_size,
	          internal_volume->volume_name_record->value_data,
	          internal_volume->volume_name_record->value_data_size,
	          LIBUNA_ENDIAN_LITTLE,
	          error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 name from volume name record.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSREFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the format version
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_volume_get_version(
     libfsrefs_volume_t *volume,
     uint8_t *major_version,
     uint8_t *minor_version,
     libcerror_error_t **error )
{
	libfsrefs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsrefs_volume_get_version";

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
	if( major_version == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid major version.",
		 function );

		return( -1 );
	}
	if( minor_version == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid minor version.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSREFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*major_version = internal_volume->io_handle->major_format_version;
	*minor_version = internal_volume->io_handle->minor_format_version;

#if defined( HAVE_LIBFSREFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Retrieves the serial number
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_volume_get_serial_number(
     libfsrefs_volume_t *volume,
     uint64_t *serial_number,
     libcerror_error_t **error )
{
	libfsrefs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsrefs_volume_get_serial_number";
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
	internal_volume = (libfsrefs_internal_volume_t *) volume;

#if defined( HAVE_LIBFSREFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsrefs_volume_header_get_volume_serial_number(
	     internal_volume->volume_header,
	     serial_number,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve volume serial number.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSREFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the root directory file entry
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_volume_get_root_directory(
     libfsrefs_volume_t *volume,
     libfsrefs_file_entry_t **file_entry,
     libcerror_error_t **error )
{
	libfsrefs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsrefs_volume_get_root_directory";
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
	internal_volume = (libfsrefs_internal_volume_t *) volume;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( *file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file entry value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSREFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libfsrefs_file_entry_initialize(
	     file_entry,
	     internal_volume->io_handle,
	     internal_volume->file_io_handle,
	     internal_volume->objects_tree,
	     NULL,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create root directory file entry.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSREFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

