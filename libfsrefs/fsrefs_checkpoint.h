/*
 * The ReFS checkpoint definition
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

#if !defined( _FSREFS_CHECKPOINT_H )
#define _FSREFS_CHECKPOINT_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsrefs_checkpoint_header fsrefs_checkpoint_header_t;

struct fsrefs_checkpoint_header
{
	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];

	/* Major format version
	 * Consists of 2 bytes
	 */
	uint8_t major_format_version[ 2 ];

	/* Minor format version
	 * Consists of 2 bytes
	 */
	uint8_t minor_format_version[ 2 ];

	/* Self reference data offset
	 * Consists of 4 bytes
	 */
	uint8_t self_reference_data_offset[ 4 ];

	/* Self reference data size
	 * Consists of 4 bytes
	 */
	uint8_t self_reference_data_size[ 4 ];
};

typedef struct fsrefs_checkpoint_trailer_v1 fsrefs_checkpoint_trailer_v1_t;

struct fsrefs_checkpoint_trailer_v1
{
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
	 * Consists of 8 bytes
	 */
	uint8_t unknown5[ 8 ];

	/* Number of offsets
	 * Consists of 4 bytes
	 */
	uint8_t number_of_offsets[ 4 ];
};

typedef struct fsrefs_checkpoint_trailer_v3 fsrefs_checkpoint_trailer_v3_t;

struct fsrefs_checkpoint_trailer_v3
{
	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown2[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown3[ 8 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown4[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown5[ 4 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown6[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown7[ 8 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown8[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown9[ 4 ];

	/* Number of offsets
	 * Consists of 4 bytes
	 */
	uint8_t number_of_offsets[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSREFS_CHECKPOINT_H ) */

