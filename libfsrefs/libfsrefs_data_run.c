/*
 * Data run functions
 *
 * Copyright (C) 2012-2024, Joachim Metz <joachim.metz@gmail.com>
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

#include "libfsrefs_data_run.h"
#include "libfsrefs_io_handle.h"
#include "libfsrefs_libcerror.h"
#include "libfsrefs_libcnotify.h"

#include "fsrefs_directory_object.h"

/* Creates a data run
 * Make sure the value data_run is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_data_run_initialize(
     libfsrefs_data_run_t **data_run,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_data_run_initialize";

	if( data_run == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data run.",
		 function );

		return( -1 );
	}
	if( *data_run != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data run value already set.",
		 function );

		return( -1 );
	}
	*data_run = memory_allocate_structure(
	             libfsrefs_data_run_t );

	if( *data_run == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data run.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *data_run,
	     0,
	     sizeof( libfsrefs_data_run_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear data run.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *data_run != NULL )
	{
		memory_free(
		 *data_run );

		*data_run = NULL;
	}
	return( -1 );
}

/* Frees a data run
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_data_run_free(
     libfsrefs_data_run_t **data_run,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_data_run_free";

	if( data_run == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data run.",
		 function );

		return( -1 );
	}
	if( *data_run != NULL )
	{
		memory_free(
		 *data_run );

		*data_run = NULL;
	}
	return( 1 );
}

/* Reads a data run
 * Returns 1 if successful or -1 on error
 */
int libfsrefs_data_run_read_data(
     libfsrefs_data_run_t *data_run,
     libfsrefs_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsrefs_data_run_read_data";

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit  = 0;
#endif

	if( data_run == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data run.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle.",
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
	if( data_size != sizeof( fsrefs_data_run_t ) )
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
		 "%s: data run data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_data_run_t *) data )->logical_offset,
		 value_64bit );
		libcnotify_printf(
		 "%s: logical offset\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_data_run_t *) data )->size,
		 value_64bit );
		libcnotify_printf(
		 "%s: size\t\t\t\t\t: %" PRIu64 " blocks\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_data_run_t *) data )->physical_offset,
		 value_64bit );
		libcnotify_printf(
		 "%s: physical offset\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsrefs_data_run_t *) data )->unknown1,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

