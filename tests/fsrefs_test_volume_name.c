/*
 * Library volume_name type test program
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

#include "../libfsrefs/libfsrefs_volume_name.h"

#if defined( __GNUC__ ) && !defined( LIBFSREFS_DLL_IMPORT )

/* Tests the libfsrefs_volume_name_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsrefs_test_volume_name_initialize(
     void )
{
	libcerror_error_t *error             = NULL;
	libfsrefs_volume_name_t *volume_name = NULL;
	int result                           = 0;

#if defined( HAVE_FSREFS_TEST_MEMORY )
	int number_of_malloc_fail_tests      = 1;
	int number_of_memset_fail_tests      = 1;
	int test_number                      = 0;
#endif

	/* Test regular cases
	 */
	result = libfsrefs_volume_name_initialize(
	          &volume_name,
	          &error );

	FSREFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSREFS_TEST_ASSERT_IS_NOT_NULL(
	 "volume_name",
	 volume_name );

	FSREFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsrefs_volume_name_free(
	          &volume_name,
	          &error );

	FSREFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSREFS_TEST_ASSERT_IS_NULL(
	 "volume_name",
	 volume_name );

	FSREFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsrefs_volume_name_initialize(
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

	volume_name = (libfsrefs_volume_name_t *) 0x12345678UL;

	result = libfsrefs_volume_name_initialize(
	          &volume_name,
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

	volume_name = NULL;

#if defined( HAVE_FSREFS_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libfsrefs_volume_name_initialize with malloc failing
		 */
		fsrefs_test_malloc_attempts_before_fail = test_number;

		result = libfsrefs_volume_name_initialize(
		          &volume_name,
		          &error );

		if( fsrefs_test_malloc_attempts_before_fail != -1 )
		{
			fsrefs_test_malloc_attempts_before_fail = -1;

			if( volume_name != NULL )
			{
				libfsrefs_volume_name_free(
				 &volume_name,
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
			 "volume_name",
			 volume_name );

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
		/* Test libfsrefs_volume_name_initialize with memset failing
		 */
		fsrefs_test_memset_attempts_before_fail = test_number;

		result = libfsrefs_volume_name_initialize(
		          &volume_name,
		          &error );

		if( fsrefs_test_memset_attempts_before_fail != -1 )
		{
			fsrefs_test_memset_attempts_before_fail = -1;

			if( volume_name != NULL )
			{
				libfsrefs_volume_name_free(
				 &volume_name,
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
			 "volume_name",
			 volume_name );

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
	if( volume_name != NULL )
	{
		libfsrefs_volume_name_free(
		 &volume_name,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsrefs_volume_name_free function
 * Returns 1 if successful or 0 if not
 */
int fsrefs_test_volume_name_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsrefs_volume_name_free(
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

/* Tests the libfsrefs_volume_name_get_utf8_name_size function
 * Returns 1 if successful or 0 if not
 */
int fsrefs_test_volume_name_get_utf8_name_size(
     void )
{
	libcerror_error_t *error             = NULL;
	libfsrefs_volume_name_t *volume_name = NULL;
	size_t utf8_name_size                = 0;
	int result                           = 0;
	int utf8_name_size_is_set            = 0;

	/* Initialize test
	 */
	result = libfsrefs_volume_name_initialize(
	          &volume_name,
	          &error );

	FSREFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSREFS_TEST_ASSERT_IS_NOT_NULL(
	 "volume_name",
	 volume_name );

	FSREFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsrefs_volume_name_get_utf8_name_size(
	          volume_name,
	          &utf8_name_size,
	          &error );

	FSREFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSREFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_name_size_is_set = result;

	/* Test error cases
	 */
	result = libfsrefs_volume_name_get_utf8_name_size(
	          NULL,
	          &utf8_name_size,
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

	if( utf8_name_size_is_set != 0 )
	{
		result = libfsrefs_volume_name_get_utf8_name_size(
		          volume_name,
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
	result = libfsrefs_volume_name_free(
	          &volume_name,
	          &error );

	FSREFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSREFS_TEST_ASSERT_IS_NULL(
	 "volume_name",
	 volume_name );

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
	if( volume_name != NULL )
	{
		libfsrefs_volume_name_free(
		 &volume_name,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsrefs_volume_name_get_utf8_name function
 * Returns 1 if successful or 0 if not
 */
int fsrefs_test_volume_name_get_utf8_name(
     void )
{
	uint8_t utf8_name[ 512 ];

	libcerror_error_t *error             = NULL;
	libfsrefs_volume_name_t *volume_name = NULL;
	int result                           = 0;
	int utf8_name_is_set                 = 0;

	/* Initialize test
	 */
	result = libfsrefs_volume_name_initialize(
	          &volume_name,
	          &error );

	FSREFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSREFS_TEST_ASSERT_IS_NOT_NULL(
	 "volume_name",
	 volume_name );

	FSREFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsrefs_volume_name_get_utf8_name(
	          volume_name,
	          utf8_name,
	          512,
	          &error );

	FSREFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSREFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_name_is_set = result;

	/* Test error cases
	 */
	result = libfsrefs_volume_name_get_utf8_name(
	          NULL,
	          utf8_name,
	          512,
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

	if( utf8_name_is_set != 0 )
	{
		result = libfsrefs_volume_name_get_utf8_name(
		          volume_name,
		          NULL,
		          512,
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

		result = libfsrefs_volume_name_get_utf8_name(
		          volume_name,
		          utf8_name,
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

		result = libfsrefs_volume_name_get_utf8_name(
		          volume_name,
		          utf8_name,
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
	}
	/* Clean up
	 */
	result = libfsrefs_volume_name_free(
	          &volume_name,
	          &error );

	FSREFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSREFS_TEST_ASSERT_IS_NULL(
	 "volume_name",
	 volume_name );

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
	if( volume_name != NULL )
	{
		libfsrefs_volume_name_free(
		 &volume_name,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsrefs_volume_name_get_utf16_name_size function
 * Returns 1 if successful or 0 if not
 */
int fsrefs_test_volume_name_get_utf16_name_size(
     void )
{
	libcerror_error_t *error             = NULL;
	libfsrefs_volume_name_t *volume_name = NULL;
	size_t utf16_name_size               = 0;
	int result                           = 0;
	int utf16_name_size_is_set           = 0;

	/* Initialize test
	 */
	result = libfsrefs_volume_name_initialize(
	          &volume_name,
	          &error );

	FSREFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSREFS_TEST_ASSERT_IS_NOT_NULL(
	 "volume_name",
	 volume_name );

	FSREFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsrefs_volume_name_get_utf16_name_size(
	          volume_name,
	          &utf16_name_size,
	          &error );

	FSREFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSREFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_name_size_is_set = result;

	/* Test error cases
	 */
	result = libfsrefs_volume_name_get_utf16_name_size(
	          NULL,
	          &utf16_name_size,
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

	if( utf16_name_size_is_set != 0 )
	{
		result = libfsrefs_volume_name_get_utf16_name_size(
		          volume_name,
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
	result = libfsrefs_volume_name_free(
	          &volume_name,
	          &error );

	FSREFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSREFS_TEST_ASSERT_IS_NULL(
	 "volume_name",
	 volume_name );

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
	if( volume_name != NULL )
	{
		libfsrefs_volume_name_free(
		 &volume_name,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsrefs_volume_name_get_utf16_name function
 * Returns 1 if successful or 0 if not
 */
int fsrefs_test_volume_name_get_utf16_name(
     void )
{
	uint16_t utf16_name[ 512 ];

	libcerror_error_t *error             = NULL;
	libfsrefs_volume_name_t *volume_name = NULL;
	int result                           = 0;
	int utf16_name_is_set                = 0;

	/* Initialize test
	 */
	result = libfsrefs_volume_name_initialize(
	          &volume_name,
	          &error );

	FSREFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSREFS_TEST_ASSERT_IS_NOT_NULL(
	 "volume_name",
	 volume_name );

	FSREFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsrefs_volume_name_get_utf16_name(
	          volume_name,
	          utf16_name,
	          512,
	          &error );

	FSREFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSREFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_name_is_set = result;

	/* Test error cases
	 */
	result = libfsrefs_volume_name_get_utf16_name(
	          NULL,
	          utf16_name,
	          512,
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

	if( utf16_name_is_set != 0 )
	{
		result = libfsrefs_volume_name_get_utf16_name(
		          volume_name,
		          NULL,
		          512,
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

		result = libfsrefs_volume_name_get_utf16_name(
		          volume_name,
		          utf16_name,
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

		result = libfsrefs_volume_name_get_utf16_name(
		          volume_name,
		          utf16_name,
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
	}
	/* Clean up
	 */
	result = libfsrefs_volume_name_free(
	          &volume_name,
	          &error );

	FSREFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSREFS_TEST_ASSERT_IS_NULL(
	 "volume_name",
	 volume_name );

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
	if( volume_name != NULL )
	{
		libfsrefs_volume_name_free(
		 &volume_name,
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
	 "libfsrefs_volume_name_initialize",
	 fsrefs_test_volume_name_initialize );

	FSREFS_TEST_RUN(
	 "libfsrefs_volume_name_free",
	 fsrefs_test_volume_name_free );

	/* TODO: add tests for libfsrefs_volume_name_read */

	FSREFS_TEST_RUN(
	 "libfsrefs_volume_name_get_utf8_name_size",
	 fsrefs_test_volume_name_get_utf8_name_size );

	FSREFS_TEST_RUN(
	 "libfsrefs_volume_name_get_utf8_name",
	 fsrefs_test_volume_name_get_utf8_name );

	FSREFS_TEST_RUN(
	 "libfsrefs_volume_name_get_utf16_name_size",
	 fsrefs_test_volume_name_get_utf16_name_size );

	FSREFS_TEST_RUN(
	 "libfsrefs_volume_name_get_utf16_name",
	 fsrefs_test_volume_name_get_utf16_name );

#endif /* defined( __GNUC__ ) && !defined( LIBFSREFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

