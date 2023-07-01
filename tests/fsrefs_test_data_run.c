/*
 * Library data_run type test program
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
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "fsrefs_test_libcerror.h"
#include "fsrefs_test_libfsrefs.h"
#include "fsrefs_test_macros.h"
#include "fsrefs_test_memory.h"
#include "fsrefs_test_unused.h"

#include "../libfsrefs/libfsrefs_data_run.h"

uint8_t fsrefs_test_data_run_data1[ 32 ] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBFSREFS_DLL_IMPORT )

/* Tests the libfsrefs_data_run_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsrefs_test_data_run_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libfsrefs_data_run_t *data_run  = NULL;
	int result                      = 0;

#if defined( HAVE_FSREFS_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = libfsrefs_data_run_initialize(
	          &data_run,
	          &error );

	FSREFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSREFS_TEST_ASSERT_IS_NOT_NULL(
	 "data_run",
	 data_run );

	FSREFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsrefs_data_run_free(
	          &data_run,
	          &error );

	FSREFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSREFS_TEST_ASSERT_IS_NULL(
	 "data_run",
	 data_run );

	FSREFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsrefs_data_run_initialize(
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

	data_run = (libfsrefs_data_run_t *) 0x12345678UL;

	result = libfsrefs_data_run_initialize(
	          &data_run,
	          &error );

	data_run = NULL;

	FSREFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSREFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_FSREFS_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libfsrefs_data_run_initialize with malloc failing
		 */
		fsrefs_test_malloc_attempts_before_fail = test_number;

		result = libfsrefs_data_run_initialize(
		          &data_run,
		          &error );

		if( fsrefs_test_malloc_attempts_before_fail != -1 )
		{
			fsrefs_test_malloc_attempts_before_fail = -1;

			if( data_run != NULL )
			{
				libfsrefs_data_run_free(
				 &data_run,
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
			 "data_run",
			 data_run );

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
		/* Test libfsrefs_data_run_initialize with memset failing
		 */
		fsrefs_test_memset_attempts_before_fail = test_number;

		result = libfsrefs_data_run_initialize(
		          &data_run,
		          &error );

		if( fsrefs_test_memset_attempts_before_fail != -1 )
		{
			fsrefs_test_memset_attempts_before_fail = -1;

			if( data_run != NULL )
			{
				libfsrefs_data_run_free(
				 &data_run,
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
			 "data_run",
			 data_run );

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
	if( data_run != NULL )
	{
		libfsrefs_data_run_free(
		 &data_run,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsrefs_data_run_free function
 * Returns 1 if successful or 0 if not
 */
int fsrefs_test_data_run_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsrefs_data_run_free(
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

/* Tests the libfsrefs_data_run_read_data function
 * Returns 1 if successful or 0 if not
 */
int fsrefs_test_data_run_read_data(
     void )
{
	libcerror_error_t *error             = NULL;
	libfsrefs_data_run_t *data_run = NULL;
	int result                           = 0;

	/* Initialize test
	 */
	result = libfsrefs_data_run_initialize(
	          &data_run,
	          &error );

	FSREFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSREFS_TEST_ASSERT_IS_NOT_NULL(
	 "data_run",
	 data_run );

	FSREFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsrefs_data_run_read_data(
	          data_run,
	          fsrefs_test_data_run_data1,
	          32,
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
	result = libfsrefs_data_run_read_data(
	          NULL,
	          fsrefs_test_data_run_data1,
	          32,
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

	result = libfsrefs_data_run_read_data(
	          data_run,
	          NULL,
	          32,
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

	result = libfsrefs_data_run_read_data(
	          data_run,
	          fsrefs_test_data_run_data1,
	          (size_t) SSIZE_MAX + 1,
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

	result = libfsrefs_data_run_read_data(
	          data_run,
	          fsrefs_test_data_run_data1,
	          0,
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

/* TODO: Test error case where data is invalid
 */

	/* Clean up
	 */
	result = libfsrefs_data_run_free(
	          &data_run,
	          &error );

	FSREFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSREFS_TEST_ASSERT_IS_NULL(
	 "data_run",
	 data_run );

	FSREFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( data_run != NULL )
	{
		libfsrefs_data_run_free(
		 &data_run,
		 NULL );
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
	 "libfsrefs_data_run_initialize",
	 fsrefs_test_data_run_initialize );

	FSREFS_TEST_RUN(
	 "libfsrefs_data_run_free",
	 fsrefs_test_data_run_free );

	FSREFS_TEST_RUN(
	 "libfsrefs_data_run_read_data",
	 fsrefs_test_data_run_read_data );

#endif /* defined( __GNUC__ ) && !defined( LIBFSREFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBFSREFS_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFSREFS_DLL_IMPORT ) */
}

