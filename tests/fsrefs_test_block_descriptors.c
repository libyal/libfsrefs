/*
 * Library block_descriptors type test program
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

#include "../libfsrefs/libfsrefs_block_descriptors.h"

#if defined( __GNUC__ ) && !defined( LIBFSREFS_DLL_IMPORT )

/* Tests the libfsrefs_block_descriptors_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsrefs_test_block_descriptors_initialize(
     void )
{
	libcerror_error_t *error                         = NULL;
	libfsrefs_block_descriptors_t *block_descriptors = NULL;
	int result                                       = 0;

#if defined( HAVE_FSREFS_TEST_MEMORY )
	int number_of_malloc_fail_tests                  = 1;
	int number_of_memset_fail_tests                  = 1;
	int test_number                                  = 0;
#endif

	/* Test regular cases
	 */
	result = libfsrefs_block_descriptors_initialize(
	          &block_descriptors,
	          &error );

	FSREFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSREFS_TEST_ASSERT_IS_NOT_NULL(
	 "block_descriptors",
	 block_descriptors );

	FSREFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsrefs_block_descriptors_free(
	          &block_descriptors,
	          &error );

	FSREFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSREFS_TEST_ASSERT_IS_NULL(
	 "block_descriptors",
	 block_descriptors );

	FSREFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsrefs_block_descriptors_initialize(
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

	block_descriptors = (libfsrefs_block_descriptors_t *) 0x12345678UL;

	result = libfsrefs_block_descriptors_initialize(
	          &block_descriptors,
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

	block_descriptors = NULL;

#if defined( HAVE_FSREFS_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libfsrefs_block_descriptors_initialize with malloc failing
		 */
		fsrefs_test_malloc_attempts_before_fail = test_number;

		result = libfsrefs_block_descriptors_initialize(
		          &block_descriptors,
		          &error );

		if( fsrefs_test_malloc_attempts_before_fail != -1 )
		{
			fsrefs_test_malloc_attempts_before_fail = -1;

			if( block_descriptors != NULL )
			{
				libfsrefs_block_descriptors_free(
				 &block_descriptors,
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
			 "block_descriptors",
			 block_descriptors );

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
		/* Test libfsrefs_block_descriptors_initialize with memset failing
		 */
		fsrefs_test_memset_attempts_before_fail = test_number;

		result = libfsrefs_block_descriptors_initialize(
		          &block_descriptors,
		          &error );

		if( fsrefs_test_memset_attempts_before_fail != -1 )
		{
			fsrefs_test_memset_attempts_before_fail = -1;

			if( block_descriptors != NULL )
			{
				libfsrefs_block_descriptors_free(
				 &block_descriptors,
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
			 "block_descriptors",
			 block_descriptors );

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
	if( block_descriptors != NULL )
	{
		libfsrefs_block_descriptors_free(
		 &block_descriptors,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsrefs_block_descriptors_free function
 * Returns 1 if successful or 0 if not
 */
int fsrefs_test_block_descriptors_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsrefs_block_descriptors_free(
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

/* Tests the libfsrefs_block_descriptors_get_number_of_block_descriptors function
 * Returns 1 if successful or 0 if not
 */
int fsrefs_test_block_descriptors_get_number_of_block_descriptors(
     void )
{
	libcerror_error_t *error                         = NULL;
	libfsrefs_block_descriptors_t *block_descriptors = NULL;
	int number_of_block_descriptors                  = 0;
	int number_of_block_descriptors_is_set           = 0;
	int result                                       = 0;

	/* Initialize test
	 */
	result = libfsrefs_block_descriptors_initialize(
	          &block_descriptors,
	          &error );

	FSREFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSREFS_TEST_ASSERT_IS_NOT_NULL(
	 "block_descriptors",
	 block_descriptors );

	FSREFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsrefs_block_descriptors_get_number_of_block_descriptors(
	          block_descriptors,
	          &number_of_block_descriptors,
	          &error );

	FSREFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSREFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	number_of_block_descriptors_is_set = result;

	/* Test error cases
	 */
	result = libfsrefs_block_descriptors_get_number_of_block_descriptors(
	          NULL,
	          &number_of_block_descriptors,
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

	if( number_of_block_descriptors_is_set != 0 )
	{
		result = libfsrefs_block_descriptors_get_number_of_block_descriptors(
		          block_descriptors,
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
	}
	/* Clean up
	 */
	result = libfsrefs_block_descriptors_free(
	          &block_descriptors,
	          &error );

	FSREFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSREFS_TEST_ASSERT_IS_NULL(
	 "block_descriptors",
	 block_descriptors );

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
	if( block_descriptors != NULL )
	{
		libfsrefs_block_descriptors_free(
		 &block_descriptors,
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
	 "libfsrefs_block_descriptors_initialize",
	 fsrefs_test_block_descriptors_initialize );

	FSREFS_TEST_RUN(
	 "libfsrefs_block_descriptors_free",
	 fsrefs_test_block_descriptors_free );

	/* TODO: add tests for libfsrefs_block_descriptors_read */

	FSREFS_TEST_RUN(
	 "libfsrefs_block_descriptors_get_number_of_block_descriptors",
	 fsrefs_test_block_descriptors_get_number_of_block_descriptors );

	/* TODO: add tests for libfsrefs_block_descriptors_get_block_descriptor_by_index */

#endif /* defined( __GNUC__ ) && !defined( LIBFSREFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

