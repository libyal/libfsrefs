/*
 * The REFS metadata block definition
 *
 * Copyright (C) 2012-2018, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _FSREFS_METADATA_BLOCK_H )
#define _FSREFS_METADATA_BLOCK_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsrefs_metadata_block_header fsrefs_metadata_block_header_t;

struct fsrefs_metadata_block_header
{
	/* The metadata block number
	 * Consists of 8 bytes
	 */
	uint8_t block_number[ 8 ];

	/* The sequence number
	 * Consists of 8 bytes
	 */
	uint8_t sequence_number[ 8 ];

	/* The object identifier
	 * Consists of 16 bytes
	 */
	uint8_t object_identifier[ 16 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown1[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown2[ 8 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSREFS_METADATA_BLOCK_H ) */

