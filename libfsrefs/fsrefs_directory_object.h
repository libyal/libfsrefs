/*
 * The ReFS directory object definitions
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

#if !defined( _FSREFS_DIRECTORY_OBJECT_H )
#define _FSREFS_DIRECTORY_OBJECT_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsrefs_attribute_non_resident fsrefs_attribute_non_resident_t;

struct fsrefs_attribute_non_resident
{
	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown2[ 8 ];

	/* Allocated file data size
	 * Consists of 8 bytes
	 */
	uint8_t allocated_data_size[ 8 ];

	/* File data size
	 * Consists of 8 bytes
	 */
	uint8_t data_size[ 8 ];

	/* Valid file data size
	 * Consists of 8 bytes
	 */
	uint8_t valid_data_size[ 8 ];

	/* Unknown
	 * Consists of 60 bytes
	 */
	uint8_t unknown3[ 60 ];
};

typedef struct fsrefs_attribute_resident fsrefs_attribute_resident_t;

struct fsrefs_attribute_resident
{
	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];

	/* Inline data offset
	 * Consists of 4 bytes
	 */
	uint8_t inline_data_offset[ 4 ];

	/* Inline data size
	 * Consists of 4 bytes
	 */
	uint8_t inline_data_size[ 4 ];

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

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown6[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown7[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown8[ 8 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown9[ 4 ];
};

typedef struct fsrefs_data_run fsrefs_data_run_t;

struct fsrefs_data_run
{
	/* Logical offset
	 * Consists of 8 bytes
	 */
	uint8_t logical_offset[ 8 ];

	/* Size
	 * Consists of 8 bytes
	 */
	uint8_t size[ 8 ];

	/* Physical offset
	 * Consists of 8 bytes
	 */
	uint8_t physical_offset[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown1[ 8 ];
};

typedef struct fsrefs_directory_values fsrefs_directory_values_t;

struct fsrefs_directory_values
{
	/* The object identifier
	 * Consists of 8 bytes
	 */
	uint8_t object_identifier[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown1[ 8 ];

	/* The creation date and time
	 * Consists of 8 bytes
	 * Contains a FILETIME
	 */
	uint8_t creation_time[ 8 ];

	/* The last modification date and time
	 * Consists of 8 bytes
	 * Contains a FILETIME
	 */
	uint8_t modification_time[ 8 ];

	/* The entry last modification date and time
	 * Consists of 8 bytes
	 * Contains a FILETIME
	 */
	uint8_t entry_modification_time[ 8 ];

	/* The last access date and time
	 * Consists of 8 bytes
	 * Contains a FILETIME
	 */
	uint8_t access_time[ 8 ];

	/* Unknown
	 * Consists of 16 bytes
	 */
	uint8_t unknown2[ 16 ];

	/* The file attribute flags
	 * Consists of 4 bytes
	 */
	uint8_t file_attribute_flags[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown3[ 4 ];
};

typedef struct fsrefs_file_values fsrefs_file_values_t;

struct fsrefs_file_values
{
	/* The creation date and time
	 * Consists of 8 bytes
	 * Contains a FILETIME
	 */
	uint8_t creation_time[ 8 ];

	/* The last modification date and time
	 * Consists of 8 bytes
	 * Contains a FILETIME
	 */
	uint8_t modification_time[ 8 ];

	/* The entry last modification date and time
	 * Consists of 8 bytes
	 * Contains a FILETIME
	 */
	uint8_t entry_modification_time[ 8 ];

	/* The last access date and time
	 * Consists of 8 bytes
	 * Contains a FILETIME
	 */
	uint8_t access_time[ 8 ];

	/* The file attribute flags
	 * Consists of 4 bytes
	 */
	uint8_t file_attribute_flags[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];

	/* File system identifier (lower 64-bit)
	 * Consists of 8 bytes
	 */
	uint8_t identifier_lower[ 8 ];

	/* File system identifier (upper 64-bit)
	 * Consists of 4 bytes
	 */
	uint8_t identifier_upper[ 8 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown4[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown5[ 4 ];

	/* File data size
	 * Consists of 8 bytes
	 */
	uint8_t data_size[ 8 ];

	/* Allocated file data size
	 * Consists of 8 bytes
	 */
	uint8_t allocated_data_size[ 8 ];

	/* Unknown
	 * Consists of 32 bytes
	 */
	uint8_t unknown6[ 32 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown7[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown8[ 8 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSREFS_DIRECTORY_OBJECT_H ) */

