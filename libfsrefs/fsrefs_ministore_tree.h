/*
 * The ReFS ministore tree metadata definition
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

#if !defined( _FSREFS_MINISTORE_TREE_H )
#define _FSREFS_MINISTORE_TREE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsrefs_ministore_tree_header fsrefs_ministore_tree_header_t;

struct fsrefs_ministore_tree_header
{
	/* Table data offset
	 * Consists of 2 bytes
	 */
	uint8_t table_data_offset[ 2 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown1[ 2 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown2[ 4 ];

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
	 * Consists of 4 bytes
	 */
	uint8_t unknown7[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown8[ 4 ];
};

typedef struct fsrefs_ministore_tree_node_header fsrefs_ministore_tree_node_header_t;

struct fsrefs_ministore_tree_node_header
{
	/* Data area start offset
	 * Consists of 4 bytes
	 */
	uint8_t data_area_start_offset[ 4 ];

	/* Data area end offset
	 * Consists of 4 bytes
	 */
	uint8_t data_area_end_offset[ 4 ];

	/* Unused data size
	 * Consists of 4 bytes
	 */
	uint8_t unused_data_size[ 4 ];

	/* Node level
	 * Consists of 1 byte
	 */
	uint8_t node_level;

	/* Node type
	 * Consists of 1 byte
	 */
	uint8_t node_type;

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown1[ 2 ];

	/* Record offsets start offset
	 * Consists of 4 bytes
	 */
	uint8_t record_offsets_start_offset[ 4 ];

	/* Number of record offsets
	 * Consists of 4 bytes
	 */
	uint8_t number_of_record_offsets[ 4 ];

	/* Record offsets end offset
	 * Consists of 4 bytes
	 */
	uint8_t record_offsets_end_offset[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown2[ 4 ];
};

typedef struct fsrefs_ministore_tree_node_record fsrefs_ministore_tree_node_record_t;

struct fsrefs_ministore_tree_node_record
{
	/* Size
	 * Consists of 4 bytes
	 */
	uint8_t size[ 4 ];

	/* Key data offset
	 * Consists of 2 bytes
	 */
	uint8_t key_data_offset[ 2 ];

	/* Key data size
	 * Consists of 2 bytes
	 */
	uint8_t key_data_size[ 2 ];

	/* Flags
	 * Consists of 2 bytes
	 */
	uint8_t flags[ 2 ];

	/* Value data offset
	 * Consists of 2 bytes
	 */
	uint8_t value_data_offset[ 2 ];

	/* Value data size
	 * Consists of 2 bytes
	 */
	uint8_t value_data_size[ 2 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSREFS_MINISTORE_TREE_H ) */

