/*
 * The internal type definitions
 *
 * Copyright (C) 2012-2019, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBFSREFS_INTERNAL_TYPES_H )
#define _LIBFSREFS_INTERNAL_TYPES_H

#include <common.h>
#include <types.h>

/* Define HAVE_LOCAL_LIBFSREFS for local use of libfsrefs
 * The definitions in <libfsrefs/types.h> are copied here
 * for local use of libfsrefs
 */
#if defined( HAVE_LOCAL_LIBFSREFS )

/* The following type definitions hide internal data structures
 */
#if defined( HAVE_DEBUG_OUTPUT ) && !defined( WINAPI )
typedef struct libfsrefs_file_entry {}	libfsrefs_file_entry_t;
typedef struct libfsrefs_volume {}	libfsrefs_volume_t;

#else
typedef intptr_t libfsrefs_file_entry_t;
typedef intptr_t libfsrefs_volume_t;

#endif /* defined( HAVE_DEBUG_OUTPUT ) && !defined( WINAPI ) */

#endif /* defined( HAVE_LOCAL_LIBFSREFS ) */

#endif /* !defined( _LIBFSREFS_INTERNAL_TYPES_H ) */

