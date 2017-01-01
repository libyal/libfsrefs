/*
 * Library level1_metadata type test program
 *
 * Copyright (C) 2012-2017, Joachim Metz <joachim.metz@gmail.com>
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
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "fsrefs_test_libcerror.h"
#include "fsrefs_test_libfsrefs.h"
#include "fsrefs_test_macros.h"
#include "fsrefs_test_memory.h"
#include "fsrefs_test_unused.h"

#include "../libfsrefs/libfsrefs_level1_metadata.h"

#if defined( __GNUC__ )

/* Tests the libfsrefs_level1_metadata_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsrefs_test_level1_metadata_initialize(
     void )
{
	libcerror_error_t *error                     = NULL;
	libfsrefs_level1_metadata_t *level1_metadata = NULL;
	int result                                   = 0;

#if defined( HAVE_FSREFS_TEST_MEMORY )
	int number_of_malloc_fail_tests              = 1;
	int number_of_memset_fail_tests              = 1;
	int test_number                              = 0;
#endif

	/* Test regular cases
	 */
	result = libfsrefs_level1_metadata_initialize(
	          &level1_metadata,
	          &error );

	FSREFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        FSREFS_TEST_ASSERT_IS_NOT_NULL(
         "level1_metadata",
         level1_metadata );

        FSREFS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libfsrefs_level1_metadata_free(
	          &level1_metadata,
	          &error );

	FSREFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        FSREFS_TEST_ASSERT_IS_NULL(
         "level1_metadata",
         level1_metadata );

        FSREFS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libfsrefs_level1_metadata_initialize(
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

	level1_metadata = (libfsrefs_level1_metadata_t *) 0x12345678UL;

	result = libfsrefs_level1_metadata_initialize(
	          &level1_metadata,
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

	level1_metadata = NULL;

#if defined( HAVE_FSREFS_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libfsrefs_level1_metadata_initialize with malloc failing
		 */
		fsrefs_test_malloc_attempts_before_fail = test_number;

		result = libfsrefs_level1_metadata_initialize(
		          &level1_metadata,
		          &error );

		if( fsrefs_test_malloc_attempts_before_fail != -1 )
		{
			fsrefs_test_malloc_attempts_before_fail = -1;

			if( level1_metadata != NULL )
			{
				libfsrefs_level1_metadata_free(
				 &level1_metadata,
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
			 "level1_metadata",
			 level1_metadata );

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
		/* Test libfsrefs_level1_metadata_initialize with memset failing
		 */
		fsrefs_test_memset_attempts_before_fail = test_number;

		result = libfsrefs_level1_metadata_initialize(
		          &level1_metadata,
		          &error );

		if( fsrefs_test_memset_attempts_before_fail != -1 )
		{
			fsrefs_test_memset_attempts_before_fail = -1;

			if( level1_metadata != NULL )
			{
				libfsrefs_level1_metadata_free(
				 &level1_metadata,
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
			 "level1_metadata",
			 level1_metadata );

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
	if( level1_metadata != NULL )
	{
		libfsrefs_level1_metadata_free(
		 &level1_metadata,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsrefs_level1_metadata_free function
 * Returns 1 if successful or 0 if not
 */
int fsrefs_test_level1_metadata_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsrefs_level1_metadata_free(
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

/* Tests the libfsrefs_level1_metadata_get_number_of_level2_metadata_block_descriptors function
 * Returns 1 if successful or 0 if not
 */
int fsrefs_test_level1_metadata_get_number_of_level2_metadata_block_descriptors(
     void )
{
	libcerror_error_t *error                               = NULL;
	libfsrefs_level1_metadata_t *level1_metadata           = NULL;
	int number_of_level2_metadata_block_descriptors        = 0;
	int number_of_level2_metadata_block_descriptors_is_set = 0;
	int result                                             = 0;

	/* Initialize test
	 */
	result = libfsrefs_level1_metadata_initialize(
	          &level1_metadata,
	          &error );

	FSREFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSREFS_TEST_ASSERT_IS_NOT_NULL(
	 "level1_metadata",
	 level1_metadata );

	FSREFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsrefs_level1_metadata_get_number_of_level2_metadata_block_descriptors(
	          level1_metadata,
	          &number_of_level2_metadata_block_descriptors,
	          &error );

	FSREFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSREFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	number_of_level2_metadata_block_descriptors_is_set = result;

	/* Test error cases
	 */
	result = libfsrefs_level1_metadata_get_number_of_level2_metadata_block_descriptors(
	          NULL,
	          &number_of_level2_metadata_block_descriptors,
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

	if( number_of_level2_metadata_block_descriptors_is_set != 0 )
	{
		result = libfsrefs_level1_metadata_get_number_of_level2_metadata_block_descriptors(
		          level1_metadata,
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
	result = libfsrefs_level1_metadata_free(
	          &level1_metadata,
	          &error );

	FSREFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSREFS_TEST_ASSERT_IS_NULL(
	 "level1_metadata",
	 level1_metadata );

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
	if( level1_metadata != NULL )
	{
		libfsrefs_level1_metadata_free(
		 &level1_metadata,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) */

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

#if defined( __GNUC__ )

	FSREFS_TEST_RUN(
	 "libfsrefs_level1_metadata_initialize",
	 fsrefs_test_level1_metadata_initialize );

	FSREFS_TEST_RUN(
	 "libfsrefs_level1_metadata_free",
	 fsrefs_test_level1_metadata_free );

	/* TODO: add tests for libfsrefs_level1_metadata_read */

	FSREFS_TEST_RUN(
	 "libfsrefs_level1_metadata_get_number_of_level2_metadata_block_descriptors",
	 fsrefs_test_level1_metadata_get_number_of_level2_metadata_block_descriptors );

	/* TODO: add tests for libfsrefs_level1_metadata_get_level2_metadata_block_descriptor_by_index */

#endif /* defined( __GNUC__ ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

