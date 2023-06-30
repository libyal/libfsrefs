/*
 * Info handle
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
#include <file_stream.h>
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "fsrefstools_libbfio.h"
#include "fsrefstools_libcerror.h"
#include "fsrefstools_libclocale.h"
#include "fsrefstools_libfsrefs.h"
#include "fsrefstools_libuna.h"
#include "info_handle.h"

#if !defined( LIBFSREFS_HAVE_BFIO )

extern \
int libfsrefs_volume_open_file_io_handle(
     libfsrefs_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libfsrefs_error_t **error );

#endif /* !defined( LIBFSREFS_HAVE_BFIO ) */

#define INFO_HANDLE_NOTIFY_STREAM	stdout

/* Copies a string of a decimal value to a 64-bit value
 * Returns 1 if successful or -1 on error
 */
int fsrefstools_system_string_copy_from_64_bit_in_decimal(
     const system_character_t *string,
     size_t string_size,
     uint64_t *value_64bit,
     libcerror_error_t **error )
{
	static char *function              = "fsrefstools_system_string_copy_from_64_bit_in_decimal";
	size_t string_index                = 0;
	system_character_t character_value = 0;
	uint8_t maximum_string_index       = 20;
	int8_t sign                        = 1;

	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( value_64bit == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value 64-bit.",
		 function );

		return( -1 );
	}
	*value_64bit = 0;

	if( string[ string_index ] == (system_character_t) '-' )
	{
		string_index++;
		maximum_string_index++;

		sign = -1;
	}
	else if( string[ string_index ] == (system_character_t) '+' )
	{
		string_index++;
		maximum_string_index++;
	}
	while( string_index < string_size )
	{
		if( string[ string_index ] == 0 )
		{
			break;
		}
		if( string_index > (size_t) maximum_string_index )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_LARGE,
			 "%s: string too large.",
			 function );

			return( -1 );
		}
		*value_64bit *= 10;

		if( ( string[ string_index ] >= (system_character_t) '0' )
		 && ( string[ string_index ] <= (system_character_t) '9' ) )
		{
			character_value = (system_character_t) ( string[ string_index ] - (system_character_t) '0' );
		}
		else
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported character value: %" PRIc_SYSTEM " at index: %d.",
			 function,
			 string[ string_index ],
			 string_index );

			return( -1 );
		}
		*value_64bit += character_value;

		string_index++;
	}
	if( sign == -1 )
	{
		*value_64bit *= (uint64_t) -1;
	}
	return( 1 );
}

/* Creates an info handle
 * Make sure the value info_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int info_handle_initialize(
     info_handle_t **info_handle,
     uint8_t calculate_md5,
     libcerror_error_t **error )
{
	static char *function = "info_handle_initialize";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( *info_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid info handle value already set.",
		 function );

		return( -1 );
	}
	*info_handle = memory_allocate_structure(
	                info_handle_t );

	if( *info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create info handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *info_handle,
	     0,
	     sizeof( info_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear info handle.",
		 function );

		goto on_error;
	}
	if( libbfio_file_range_initialize(
	     &( ( *info_handle )->input_file_io_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize input file IO handle.",
		 function );

		goto on_error;
	}
	if( libfsrefs_volume_initialize(
	     &( ( *info_handle )->input_volume ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize input volume.",
		 function );

		goto on_error;
	}
	( *info_handle )->calculate_md5 = calculate_md5;
	( *info_handle )->notify_stream = INFO_HANDLE_NOTIFY_STREAM;

	return( 1 );

on_error:
	if( *info_handle != NULL )
	{
		if( ( *info_handle )->input_file_io_handle != NULL )
		{
			libbfio_handle_free(
			 &( ( *info_handle )->input_file_io_handle ),
			 NULL );
		}
		memory_free(
		 *info_handle );

		*info_handle = NULL;
	}
	return( -1 );
}

/* Frees an info handle
 * Returns 1 if successful or -1 on error
 */
int info_handle_free(
     info_handle_t **info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_free";
	int result            = 1;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( *info_handle != NULL )
	{
		if( libfsrefs_volume_free(
		     &( ( *info_handle )->input_volume ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free input volume.",
			 function );

			result = -1;
		}
		if( libbfio_handle_free(
		     &( ( *info_handle )->input_file_io_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free input file IO handle.",
			 function );

			result = -1;
		}
		memory_free(
		 *info_handle );

		*info_handle = NULL;
	}
	return( result );
}

/* Signals the info handle to abort
 * Returns 1 if successful or -1 on error
 */
int info_handle_signal_abort(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_signal_abort";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	info_handle->abort = 1;

	if( info_handle->input_volume != NULL )
	{
		if( libfsrefs_volume_signal_abort(
		     info_handle->input_volume,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to signal input volume to abort.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Sets the bodyfile
 * Returns 1 if successful or -1 on error
 */
int info_handle_set_bodyfile(
     info_handle_t *info_handle,
     const system_character_t *filename,
     libcerror_error_t **error )
{
	static char *function = "info_handle_set_bodyfile";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->bodyfile_stream != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid info handle - bodyfile stream value already set.",
		 function );

		return( -1 );
	}
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	info_handle->bodyfile_stream = file_stream_open_wide(
	                                filename,
	                                L"wb" );
#else
	info_handle->bodyfile_stream = file_stream_open(
	                                filename,
	                                "wb" );
#endif
	if( info_handle->bodyfile_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open bodyfile stream.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the volume offset
 * Returns 1 if successful or -1 on error
 */
int info_handle_set_volume_offset(
     info_handle_t *info_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "info_handle_set_volume_offset";
	size_t string_length  = 0;
	uint64_t value_64bit  = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

	if( fsrefstools_system_string_copy_from_64_bit_in_decimal(
	     string,
	     string_length + 1,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy string to 64-bit decimal.",
		 function );

		return( -1 );
	}
	info_handle->volume_offset = (off64_t) value_64bit;

	return( 1 );
}

/* Opens the input
 * Returns 1 if successful or -1 on error
 */
int info_handle_open_input(
     info_handle_t *info_handle,
     const system_character_t *filename,
     libcerror_error_t **error )
{
	static char *function  = "info_handle_open_input";
	size_t filename_length = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	filename_length = system_string_length(
	                   filename );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libbfio_file_range_set_name_wide(
	     info_handle->input_file_io_handle,
	     filename,
	     filename_length,
	     error ) != 1 )
#else
	if( libbfio_file_range_set_name(
	     info_handle->input_file_io_handle,
	     filename,
	     filename_length,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open set file name.",
		 function );

		return( -1 );
	}
	if( libbfio_file_range_set(
	     info_handle->input_file_io_handle,
	     info_handle->volume_offset,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open set volume offset.",
		 function );

		return( -1 );
	}
	if( libfsrefs_volume_open_file_io_handle(
	     info_handle->input_volume,
	     info_handle->input_file_io_handle,
	     LIBFSREFS_OPEN_READ,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open input volume.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Closes the input
 * Returns the 0 if succesful or -1 on error
 */
int info_handle_close_input(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_close_input";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libfsrefs_volume_close(
	     info_handle->input_volume,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close input volume.",
		 function );

		return( -1 );
	}
	return( 0 );
}

/* Prints a file entry or data stream name
 * Returns 1 if successful or -1 on error
 */
int info_handle_name_value_fprint(
     info_handle_t *info_handle,
     const system_character_t *value_string,
     size_t value_string_length,
     libcerror_error_t **error )
{
	system_character_t *escaped_value_string     = NULL;
	static char *function                        = "info_handle_name_value_fprint";
	libuna_unicode_character_t unicode_character = 0;
	size_t escaped_value_string_index            = 0;
	size_t escaped_value_string_size             = 0;
	size_t value_string_index                    = 0;
	int print_count                              = 0;
	int result                                   = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( value_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value string.",
		 function );

		return( -1 );
	}
	/* To ensure normalization in the escaped string is handled correctly
	 * it stored in a temporary variable. Note that there is a worst-case of
	 * a 1 to 4 ratio for each escaped character.
	 */
	if( value_string_length > (size_t) ( ( SSIZE_MAX - 1 ) / ( sizeof( system_character_t ) * 4 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid value string length value exceeds maximum.",
		 function );

		return( -1 );
	}
	escaped_value_string_size = ( value_string_length * 4 ) + 1;

	escaped_value_string = (system_character_t *) memory_allocate(
	                                               sizeof( system_character_t ) * escaped_value_string_size );

	if( escaped_value_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create escaped value string.",
		 function );

		goto on_error;
	}
	while( value_string_index < value_string_length )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libuna_unicode_character_copy_from_utf16(
		          &unicode_character,
		          (libuna_utf16_character_t *) value_string,
		          value_string_length,
		          &value_string_index,
		          error );
#else
		result = libuna_unicode_character_copy_from_utf8(
		          &unicode_character,
		          (libuna_utf8_character_t *) value_string,
		          value_string_length,
		          &value_string_index,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
			 "%s: unable to copy Unicode character from value string.",
			 function );

			goto on_error;
		}
		/* Replace:
		 *   values <= 0x1f and 0x7f by \x##
		 */
		if( ( unicode_character <= 0x1f )
		 || ( unicode_character == 0x7f ) )
		{
			print_count = system_string_sprintf(
			               &( escaped_value_string[ escaped_value_string_index ] ),
			               escaped_value_string_size - escaped_value_string_index,
			               "\\x%02" PRIx32 "",
			               unicode_character );

			if( print_count < 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_CONVERSION,
				 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
				 "%s: unable to copy escaped Unicode character to escaped value string.",
				 function );

				goto on_error;
			}
			escaped_value_string_index += print_count;
		}
		else
		{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libuna_unicode_character_copy_to_utf16(
			          unicode_character,
			          (libuna_utf16_character_t *) escaped_value_string,
			          escaped_value_string_size,
			          &escaped_value_string_index,
			          error );
#else
			result = libuna_unicode_character_copy_to_utf8(
			          unicode_character,
			          (libuna_utf8_character_t *) escaped_value_string,
			          escaped_value_string_size,
			          &escaped_value_string_index,
			          error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_CONVERSION,
				 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
				 "%s: unable to copy Unicode character to escaped value string.",
				 function );

				goto on_error;
			}
		}
	}
	escaped_value_string[ escaped_value_string_index ] = 0;

	fprintf(
	 info_handle->notify_stream,
	 "%" PRIs_SYSTEM "",
	 escaped_value_string );

	memory_free(
	 escaped_value_string );

	return( 1 );

on_error:
	if( escaped_value_string != NULL )
	{
		memory_free(
		 escaped_value_string );
	}
	return( -1 );
}

/* Prints file entry as part of the file system hierarchy or to a bodyfile
 * Returns 1 if successful or -1 on error
 */
int info_handle_file_entry_fprint(
     info_handle_t *info_handle,
     libfsrefs_file_entry_t *file_entry,
     const system_character_t *path,
     const system_character_t *file_entry_name,
     libcerror_error_t **error )
{
	static char *function         = "info_handle_file_entry_fprint";
	size_t file_entry_name_length = 0;
	size_t path_length            = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		return( -1 );
	}
	path_length = system_string_length(
	               path );

	if( file_entry_name != NULL )
	{
		file_entry_name_length = system_string_length(
		                          file_entry_name );
	}
	if( info_handle_name_value_fprint(
	     info_handle,
	     path,
	     path_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print path string.",
		 function );

		goto on_error;
	}
	if( file_entry_name != NULL )
	{
		if( info_handle_name_value_fprint(
		     info_handle,
		     file_entry_name,
		     file_entry_name_length,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print file entry name string.",
			 function );

			goto on_error;
		}
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	return( 1 );

on_error:
	return( -1 );
}

/* Prints file entry information as part of the file system hierarchy
 * Returns 1 if successful or -1 on error
 */
int info_handle_file_system_hierarchy_fprint_file_entry(
     info_handle_t *info_handle,
     libfsrefs_file_entry_t *file_entry,
     const system_character_t *path,
     libcerror_error_t **error )
{
	libfsrefs_file_entry_t *sub_file_entry = NULL;
	system_character_t *file_entry_name    = NULL;
	system_character_t *sub_path           = NULL;
	static char *function                  = "info_handle_file_system_hierarchy_fprint_file_entry";
	size_t file_entry_name_size            = 0;
	size_t path_length                     = 0;
	size_t sub_path_size                   = 0;
	int number_of_sub_file_entries         = 0;
	int result                             = 0;
	int sub_file_entry_index               = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		return( -1 );
	}
	path_length = system_string_length(
	               path );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfsrefs_file_entry_get_utf16_name_size(
	          file_entry,
	          &file_entry_name_size,
	          error );
#else
	result = libfsrefs_file_entry_get_utf8_name_size(
	          file_entry,
	          &file_entry_name_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file entry name string size.",
		 function );

		goto on_error;
	}
	if( ( result == 1 )
	 && ( file_entry_name_size > 0 ) )
	{
		file_entry_name = system_string_allocate(
		                   file_entry_name_size );

		if( file_entry_name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create file entry name string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfsrefs_file_entry_get_utf16_name(
		          file_entry,
		          (uint16_t *) file_entry_name,
		          file_entry_name_size,
		          error );
#else
		result = libfsrefs_file_entry_get_utf8_name(
		          file_entry,
		          (uint8_t *) file_entry_name,
		          file_entry_name_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file entry name string.",
			 function );

			goto on_error;
		}
	}
	if( ( info_handle->bodyfile_stream != NULL )
	 || ( file_entry_name != NULL ) )
	{
		if( info_handle_file_entry_fprint(
		     info_handle,
		     file_entry,
		     path,
		     file_entry_name,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print file entry.",
			 function );

			goto on_error;
		}
	}
	if( libfsrefs_file_entry_get_number_of_sub_file_entries(
	     file_entry,
	     &number_of_sub_file_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub file entries.",
		 function );

		goto on_error;
	}
	if( number_of_sub_file_entries > 0 )
	{
		sub_path_size = path_length + 1;

		if( file_entry_name != NULL )
		{
			sub_path_size += file_entry_name_size;
		}
		sub_path = system_string_allocate(
		            sub_path_size );

		if( sub_path == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create sub path.",
			 function );

			goto on_error;
		}
		if( system_string_copy(
		     sub_path,
		     path,
		     path_length ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy path to sub path.",
			 function );

			goto on_error;
		}
		if( file_entry_name != NULL )
		{
			if( system_string_copy(
			     &( sub_path[ path_length ] ),
			     file_entry_name,
			     file_entry_name_size - 1 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy file entry name to sub path.",
				 function );

				goto on_error;
			}
			sub_path[ sub_path_size - 2 ] = (system_character_t) LIBFSREFS_SEPARATOR;
		}
		sub_path[ sub_path_size - 1 ] = (system_character_t) 0;

		for( sub_file_entry_index = 0;
		     sub_file_entry_index < number_of_sub_file_entries;
		     sub_file_entry_index++ )
		{
			if( libfsrefs_file_entry_get_sub_file_entry_by_index(
			     file_entry,
			     sub_file_entry_index,
			     &sub_file_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve sub file entry: %d.",
				 function,
				 sub_file_entry_index );

				goto on_error;
			}
			if( info_handle_file_system_hierarchy_fprint_file_entry(
			     info_handle,
			     sub_file_entry,
			     sub_path,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print file entry: %d information.",
				 function,
				 sub_file_entry_index );

				goto on_error;
			}
			if( libfsrefs_file_entry_free(
			     &sub_file_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free sub file entry: %d.",
				 function,
				 sub_file_entry_index );

				goto on_error;
			}
		}
		if( sub_path != NULL )
		{
			memory_free(
			 sub_path );

			sub_path = NULL;
		}
	}
	if( file_entry_name != NULL )
	{
		memory_free(
		 file_entry_name );

		file_entry_name = NULL;
	}
	return( 1 );

on_error:
	if( sub_file_entry != NULL )
	{
		libfsrefs_file_entry_free(
		 &sub_file_entry,
		 NULL );
	}
	if( sub_path != NULL )
	{
		memory_free(
		 sub_path );
	}
	if( file_entry_name != NULL )
	{
		memory_free(
		 file_entry_name );
	}
	return( -1 );
}

/* Prints the file system hierarchy information
 * Returns 1 if successful or -1 on error
 */
int info_handle_file_system_hierarchy_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	libfsrefs_file_entry_t *file_entry = NULL;
	static char *function              = "info_handle_file_system_hierarchy_fprint";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->bodyfile_stream == NULL )
	{
		fprintf(
		 info_handle->notify_stream,
		 "Resiliant File System (ReFS) information:\n\n" );

		fprintf(
		 info_handle->notify_stream,
		 "File system hierarchy:\n" );
	}
	if( libfsrefs_volume_get_root_directory(
	     info_handle->input_volume,
	     &file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve root directory file entry.",
		 function );

		goto on_error;
	}
	if( info_handle_file_system_hierarchy_fprint_file_entry(
	     info_handle,
	     file_entry,
	     _SYSTEM_STRING( "\\" ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print root directory file entry information.",
		 function );

		goto on_error;
	}
	if( libfsrefs_file_entry_free(
	     &file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file entry.",
		 function );

		goto on_error;
	}
	if( info_handle->bodyfile_stream == NULL )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\n" );
	}
	return( 1 );

on_error:
	if( file_entry != NULL )
	{
		libfsrefs_file_entry_free(
		 &file_entry,
		 NULL );
	}
	return( -1 );
}

/* Prints the volume information
 * Returns 1 if successful or -1 on error
 */
int info_handle_volume_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	system_character_t *volume_name = NULL;
	static char *function           = "info_handle_volume_fprint";
	size32_t cluster_block_size     = 0;
	size_t volume_name_size         = 0;
	uint64_t serial_number          = 0;
	uint16_t bytes_per_sector       = 0;
	uint8_t major_version           = 0;
	uint8_t minor_version           = 0;
	int result                      = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "Resiliant File System (ReFS) information:\n\n" );

	fprintf(
	 info_handle->notify_stream,
	 "\nVolume information:\n" );

	fprintf(
	 info_handle->notify_stream,
	 "\tName\t\t\t\t:" );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfsrefs_volume_get_utf16_name_size(
	          info_handle->input_volume,
	          &volume_name_size,
	          error );
#else
	result = libfsrefs_volume_get_utf8_name_size(
	          info_handle->input_volume,
	          &volume_name_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve volume name string size.",
		 function );

		goto on_error;
	}
	else if( ( result != 0 )
	      && ( volume_name_size > 0 ) )
	{
		volume_name = system_string_allocate(
		               volume_name_size );

		if( volume_name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create volume name string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfsrefs_volume_get_utf16_name(
		          info_handle->input_volume,
		          (uint16_t *) volume_name,
		          volume_name_size,
		          error );
#else
		result = libfsrefs_volume_get_utf8_name(
		          info_handle->input_volume,
		          (uint8_t *) volume_name,
		          volume_name_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve volume name string.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 " %" PRIs_SYSTEM "",
		 volume_name );

		memory_free(
		 volume_name );

		volume_name = NULL;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	fprintf(
	 info_handle->notify_stream,
	 "\tVersion\t\t\t\t:" );

	if( libfsrefs_volume_get_version(
	     info_handle->input_volume,
	     &major_version,
	     &minor_version,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve version.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		fprintf(
		 info_handle->notify_stream,
		 " %" PRIu8 ".%" PRIu8 "",
		 major_version,
		 minor_version );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	if( libfsrefs_volume_get_serial_number(
	     info_handle->input_volume,
	     &serial_number,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve serial number.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tSerial number\t\t\t: %08" PRIx64 "\n",
	 serial_number );

	if( libfsrefs_volume_get_bytes_per_sector(
	     info_handle->input_volume,
	     &bytes_per_sector,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve bytes per sector.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tBytes per sector\t\t: %" PRIu16 "\n",
	 bytes_per_sector );

	if( libfsrefs_volume_get_cluster_block_size(
	     info_handle->input_volume,
	     &cluster_block_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve cluster block size.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tCluster block size\t\t: %" PRIu32 "\n",
	 cluster_block_size );

/* TODO */

	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	return( 1 );

on_error:
	if( volume_name != NULL )
	{
		memory_free(
		 volume_name );
	}
	return( -1 );
}

