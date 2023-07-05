/*
 * Data run functions
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

#if !defined( _LIBFSREFS_DATA_RUN_H )
#define _LIBFSREFS_DATA_RUN_H

#include <common.h>
#include <types.h>

#include "libfsrefs_io_handle.h"
#include "libfsrefs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsrefs_data_run libfsrefs_data_run_t;

struct libfsrefs_data_run
{
	/* The start block number
	 */
	uint64_t start_block_number;

	/* The number of blocks
	 */
	uint64_t number_of_blocks;
};

int libfsrefs_data_run_initialize(
     libfsrefs_data_run_t **data_run,
     libcerror_error_t **error );

int libfsrefs_data_run_free(
     libfsrefs_data_run_t **data_run,
     libcerror_error_t **error );

int libfsrefs_data_run_read_data(
     libfsrefs_data_run_t *data_run,
     libfsrefs_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSREFS_DATA_RUN_H ) */

