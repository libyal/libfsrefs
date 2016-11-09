/*
 * Library support functions test program
 *
 * Copyright (C) 2012-2016, Joachim Metz <joachim.metz@gmail.com>
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
#include <file_stream.h>
#include <narrow_string.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "fsrefs_test_libcerror.h"
#include "fsrefs_test_libfsrefs.h"
#include "fsrefs_test_macros.h"
#include "fsrefs_test_unused.h"

/* Tests the libfsrefs_get_version function
 * Returns 1 if successful or 0 if not
 */
int fsrefs_test_get_version(
     void )
{
	const char *version_string = NULL;
	int result                 = 0;

	version_string = libfsrefs_get_version();

	result = narrow_string_compare(
	          version_string,
	          LIBFSREFS_VERSION_STRING,
	          9 );

	FSREFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	return( 1 );

on_error:
	return( 0 );
}

/* Tests the libfsrefs_get_access_flags_read function
 * Returns 1 if successful or 0 if not
 */
int fsrefs_test_get_access_flags_read(
     void )
{
	int access_flags = 0;

	access_flags = libfsrefs_get_access_flags_read();

	FSREFS_TEST_ASSERT_EQUAL_INT(
	 "access_flags",
	 access_flags,
	 LIBFSREFS_ACCESS_FLAG_READ );

	return( 1 );

on_error:
	return( 0 );
}

/* Tests the libfsrefs_get_codepage function
 * Returns 1 if successful or 0 if not
 */
int fsrefs_test_get_codepage(
     void )
{
	libcerror_error_t *error = NULL;
	int codepage             = 0;
	int result               = 0;

	result = libfsrefs_get_codepage(
	          &codepage,
	          &error );

	FSREFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        FSREFS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libfsrefs_get_codepage(
	          NULL,
	          &error );

	FSREFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

        FSREFS_TEST_ASSERT_IS_NOT_NULL(
         "error",
         error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsrefs_set_codepage function
 * Returns 1 if successful or 0 if not
 */
int fsrefs_test_set_codepage(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	result = libfsrefs_set_codepage(
	          0,
	          &error );

	FSREFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        FSREFS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libfsrefs_set_codepage(
	          -1,
	          &error );

	FSREFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

        FSREFS_TEST_ASSERT_IS_NOT_NULL(
         "error",
         error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc FSREFS_TEST_ATTRIBUTE_UNUSED,
     wchar_t * const argv[] FSREFS_TEST_ATTRIBUTE_UNUSED )
#else
int main(
     int argc FSREFS_TEST_ATTRIBUTE_UNUSED,
     char * const argv[] FSREFS_TEST_ATTRIBUTE_UNUSED )
#endif
{
	FSREFS_TEST_UNREFERENCED_PARAMETER( argc )
	FSREFS_TEST_UNREFERENCED_PARAMETER( argv )

	FSREFS_TEST_RUN(
	 "libfsrefs_get_version",
	 fsrefs_test_get_version );

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

