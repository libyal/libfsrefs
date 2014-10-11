/*
 * The REFS volume header definition
 *
 * Copyright (C) 2012-2014, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _FSREFS_VOLUME_HEADER_H )
#define _FSREFS_VOLUME_HEADER_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsrefs_volume_header fsrefs_volume_header_t;

struct fsrefs_volume_header
{
	/* Unknown
	 * Consists of 3 bytes
	 */
	uint8_t unknown1[ 3 ];

	/* The file system signature
	 * Consists of 4 bytes
	 * Contains: "ReFS"
	 */
	uint8_t file_system_signature[ 4 ];

	/* Unknown
	 * Consists of 9 bytes
	 */
	uint8_t unknown2[ 9 ];

	/* Unknown
	 * Consists of 4 bytes
	 * Contains: "FSRS"
	 */
	uint8_t unknown3[ 4 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown4[ 2 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown5[ 2 ];

	/* The number of sectors
	 * Consists of 8 bytes
	 */
	uint8_t number_of_sectors[ 8 ];

	/* The number of bytes per sector
	 * Consists of 4 bytes
	 */
	uint8_t bytes_per_sector[ 4 ];

	/* The number of sectors per block
	 * Consists of 4 bytes
	 */
	uint8_t sectors_per_block[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown7[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown8[ 4 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown9[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown10[ 8 ];

	/* Unknown
	 * Consists of 15872 bytes
	 */
	uint8_t unknown11[ 15872 ];
};

#if defined( __cplusplus )
}
#endif

#endif

