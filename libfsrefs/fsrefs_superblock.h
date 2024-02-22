/*
 * The ReFS superblock definition
 *
 * Copyright (C) 2012-2024, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _FSREFS_SUPERBLOCK_H )
#define _FSREFS_SUPERBLOCK_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsrefs_superblock fsrefs_superblock_t;

struct fsrefs_superblock
{
	/* The volume identifier
	 * Consists of 16 bytes
	 */
	uint8_t volume_identifier[ 16 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown1[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown2[ 8 ];

	/* Checkpoints data offset
	 * Consists of 4 bytes
	 */
	uint8_t checkpoints_data_offset[ 4 ];

	/* Number of checkpoints
	 * Consists of 4 bytes
	 */
	uint8_t number_of_checkpoints[ 4 ];

	/* Self reference data offset
	 * Consists of 4 bytes
	 */
	uint8_t self_reference_data_offset[ 4 ];

	/* Self reference data size
	 * Consists of 4 bytes
	 */
	uint8_t self_reference_data_size[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSREFS_SUPERBLOCK_H ) */

