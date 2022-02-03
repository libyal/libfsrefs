/*
 * Library level3_metadata type test program
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
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "fsrefs_test_libcerror.h"
#include "fsrefs_test_libfsrefs.h"
#include "fsrefs_test_macros.h"
#include "fsrefs_test_memory.h"
#include "fsrefs_test_unused.h"

#include "../libfsrefs/libfsrefs_level3_metadata.h"

#if defined( __GNUC__ ) && !defined( LIBFSREFS_DLL_IMPORT )

/* Tests the libfsrefs_level3_metadata_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsrefs_test_level3_metadata_initialize(
     void )
{
	libcerror_error_t *error                     = NULL;
	libfsrefs_level3_metadata_t *level3_metadata = NULL;
	int result                                   = 0;

#if defined( HAVE_FSREFS_TEST_MEMORY )
	int number_of_malloc_fail_tests              = 1;
	int number_of_memset_fail_tests              = 1;
	int test_number                              = 0;
#endif

	/* Test regular cases
	 */
	result = libfsrefs_level3_metadata_initialize(
	          &level3_metadata,
	          &error );

	FSREFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSREFS_TEST_ASSERT_IS_NOT_NULL(
	 "level3_metadata",
	 level3_metadata );

	FSREFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsrefs_level3_metadata_free(
	          &level3_metadata,
	          &error );

	FSREFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSREFS_TEST_ASSERT_IS_NULL(
	 "level3_metadata",
	 level3_metadata );

	FSREFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsrefs_level3_metadata_initialize(
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

	level3_metadata = (libfsrefs_level3_metadata_t *) 0x12345678UL;

	result = libfsrefs_level3_metadata_initialize(
	          &level3_metadata,
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

	level3_metadata = NULL;

#if defined( HAVE_FSREFS_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libfsrefs_level3_metadata_initialize with malloc failing
		 */
		fsrefs_test_malloc_attempts_before_fail = test_number;

		result = libfsrefs_level3_metadata_initialize(
		          &level3_metadata,
		          &error );

		if( fsrefs_test_malloc_attempts_before_fail != -1 )
		{
			fsrefs_test_malloc_attempts_before_fail = -1;

			if( level3_metadata != NULL )
			{
				libfsrefs_level3_metadata_free(
				 &level3_metadata,
				 NULL );
			}
		}
		else
		{
			FSREFS_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			FSREFS_TEST_ASSERT_IS_NULL(
			 "level3_metadata",
			 level3_metadata );

			FSREFS_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
	for( test_number = 0;
	     test_number < number_of_memset_fail_tests;
	     test_number++ )
	{
		/* Test libfsrefs_level3_metadata_initialize with memset failing
		 */
		fsrefs_test_memset_attempts_before_fail = test_number;

		result = libfsrefs_level3_metadata_initialize(
		          &level3_metadata,
		          &error );

		if( fsrefs_test_memset_attempts_before_fail != -1 )
		{
			fsrefs_test_memset_attempts_before_fail = -1;

			if( level3_metadata != NULL )
			{
				libfsrefs_level3_metadata_free(
				 &level3_metadata,
				 NULL );
			}
		}
		else
		{
			FSREFS_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			FSREFS_TEST_ASSERT_IS_NULL(
			 "level3_metadata",
			 level3_metadata );

			FSREFS_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_FSREFS_TEST_MEMORY ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( level3_metadata != NULL )
	{
		libfsrefs_level3_metadata_free(
		 &level3_metadata,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsrefs_level3_metadata_free function
 * Returns 1 if successful or 0 if not
 */
int fsrefs_test_level3_metadata_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsrefs_level3_metadata_free(
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

#endif /* defined( __GNUC__ ) && !defined( LIBFSREFS_DLL_IMPORT ) */

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

#if defined( __GNUC__ ) && !defined( LIBFSREFS_DLL_IMPORT )

	FSREFS_TEST_RUN(
	 "libfsrefs_level3_metadata_initialize",
	 fsrefs_test_level3_metadata_initialize );

	FSREFS_TEST_RUN(
	 "libfsrefs_level3_metadata_free",
	 fsrefs_test_level3_metadata_free );

	/* TODO: add tests for libfsrefs_level3_metadata_read */

#endif /* defined( __GNUC__ ) && !defined( LIBFSREFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

