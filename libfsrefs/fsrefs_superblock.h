/*
 * The ReFS metadata block definition
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

#if !defined( _FSREFS_SUPERBLOCK_H )
#define _FSREFS_SUPERBLOCK_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsrefs_superblock_v1 fsrefs_superblock_v1_t;

struct fsrefs_superblock_v1
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

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown3[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown4[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown5[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown6[ 4 ];

	/* Unknown
	 * Consists of 64 bytes
	 */
	uint8_t unknown7[ 64 ];

	/* Primary checkpoint block number
	 * Consists of 8 bytes
	 */
	uint8_t primary_checkpoint_block_number[ 8 ];

	/* Secondary checkpoint block number
	 * Consists of 8 bytes
	 */
	uint8_t secondary_checkpoint_block_number[ 8 ];

	/* Block number
	 * Consists of 8 bytes
	 */
	uint8_t block_number[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown8[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown9[ 8 ];
};

typedef struct fsrefs_superblock_v3 fsrefs_superblock_v3_t;

struct fsrefs_superblock_v3
{
	/* The volume identifier
	 * Consists of 16 bytes
	 */
	uint8_t volume_identifier[ 16 ];

	/* Unknown
	 * Consists of 136 bytes
	 */
	uint8_t unknown1[ 136 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSREFS_SUPERBLOCK_H ) */

