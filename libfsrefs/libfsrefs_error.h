/*
 * Error functions
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

#if !defined( _LIBFSREFS_INTERNAL_ERROR_H )
#define _LIBFSREFS_INTERNAL_ERROR_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#if !defined( HAVE_LOCAL_LIBFSREFS )
#include <libfsrefs/error.h>
#endif

#include "libfsrefs_extern.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if !defined( HAVE_LOCAL_LIBFSREFS )

LIBFSREFS_EXTERN \
void libfsrefs_error_free(
      libfsrefs_error_t **error );

LIBFSREFS_EXTERN \
int libfsrefs_error_fprint(
     libfsrefs_error_t *error,
     FILE *stream );

LIBFSREFS_EXTERN \
int libfsrefs_error_sprint(
     libfsrefs_error_t *error,
     char *string,
     size_t size );

LIBFSREFS_EXTERN \
int libfsrefs_error_backtrace_fprint(
     libfsrefs_error_t *error,
     FILE *stream );

LIBFSREFS_EXTERN \
int libfsrefs_error_backtrace_sprint(
     libfsrefs_error_t *error,
     char *string,
     size_t size );

#endif /* !defined( HAVE_LOCAL_LIBFSREFS ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSREFS_INTERNAL_ERROR_H ) */

