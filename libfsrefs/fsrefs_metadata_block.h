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

#if !defined( _FSREFS_METADATA_BLOCK_H )
#define _FSREFS_METADATA_BLOCK_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsrefs_metadata_block_header_v1 fsrefs_metadata_block_header_v1_t;

struct fsrefs_metadata_block_header_v1
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

typedef struct fsrefs_metadata_block_header_v3 fsrefs_metadata_block_header_v3_t;

struct fsrefs_metadata_block_header_v3
{
	/* The signature
	 * Consists of 4 bytes
	 */
	uint8_t signature[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown2[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown3[ 4 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown4[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown5[ 8 ];

	/* The first metadata block number
	 * Consists of 8 bytes
	 */
	uint8_t block_number1[ 8 ];

	/* The second metadata block number
	 * Consists of 8 bytes
	 */
	uint8_t block_number2[ 8 ];

	/* The third metadata block number
	 * Consists of 8 bytes
	 */
	uint8_t block_number3[ 8 ];

	/* The fourth metadata block number
	 * Consists of 8 bytes
	 */
	uint8_t block_number4[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown6[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown7[ 8 ];
};

typedef struct fsrefs_metadata_block_descriptor_v1 fsrefs_metadata_block_descriptor_v1_t;

struct fsrefs_metadata_block_descriptor_v1
{
	/* The metadata block number
	 * Consists of 8 bytes
	 */
	uint8_t block_number[ 8 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown1[ 2 ];

	/* Checksum type
	 * Consists of 1 byte
	 */
	uint8_t checksum_type;

	/* Checksum data offset
	 * Consists of 1 byte
	 */
	uint8_t checksum_data_offset;

	/* Checksum data size
	 * Consists of 2 bytes
	 */
	uint8_t checksum_data_size[ 2 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown2[ 2 ];
};

typedef struct fsrefs_metadata_block_descriptor_v3 fsrefs_metadata_block_descriptor_v3_t;

struct fsrefs_metadata_block_descriptor_v3
{
	/* The first metadata block number
	 * Consists of 8 bytes
	 */
	uint8_t block_number1[ 8 ];

	/* The second metadata block number
	 * Consists of 8 bytes
	 */
	uint8_t block_number2[ 8 ];

	/* The third metadata block number
	 * Consists of 8 bytes
	 */
	uint8_t block_number3[ 8 ];

	/* The fourth metadata block number
	 * Consists of 8 bytes
	 */
	uint8_t block_number4[ 8 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown1[ 2 ];

	/* Checksum type
	 * Consists of 1 byte
	 */
	uint8_t checksum_type;

	/* Checksum data offset
	 * Consists of 1 byte
	 */
	uint8_t checksum_data_offset;

	/* Checksum data size
	 * Consists of 2 bytes
	 */
	uint8_t checksum_data_size[ 2 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown2[ 2 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSREFS_METADATA_BLOCK_H ) */

