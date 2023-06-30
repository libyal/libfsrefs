/*
 * Block tree node functions
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

#if !defined( _LIBFSREFS_BLOCK_TREE_NODE_H )
#define _LIBFSREFS_BLOCK_TREE_NODE_H

#include <common.h>
#include <types.h>

#include "libfsrefs_block_descriptor.h"
#include "libfsrefs_libcerror.h"
#include "libfsrefs_libcdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsrefs_block_tree_node libfsrefs_block_tree_node_t;

struct libfsrefs_block_tree_node
{
	/* The (range) start offset
	 */
	off64_t start_offset;

	/* The (range) end offset
	 */
	off64_t end_offset;

	/* The (range) size
	 */
	off64_t size;

	/* The sub node size
	 */
	size64_t sub_node_size;

	/* Sub branch or leaf nodes array
	 */
	libcdata_array_t *sub_nodes_array;

	/* Value to indicate the node is a leaf node
	 */
	uint8_t is_leaf_node;
};

int libfsrefs_block_tree_node_initialize(
     libfsrefs_block_tree_node_t **block_tree_node,
     off64_t offset,
     size64_t size,
     size64_t leaf_value_size,
     libcerror_error_t **error );

int libfsrefs_block_tree_node_free(
     libfsrefs_block_tree_node_t **block_tree_node,
     int (*value_free_function)(
            intptr_t **value,
            libcerror_error_t **error ),
     libcerror_error_t **error );

int libfsrefs_block_tree_node_get_sub_node_at_offset(
     libfsrefs_block_tree_node_t *block_tree_node,
     off64_t offset,
     libfsrefs_block_tree_node_t **sub_block_tree_node,
     libcerror_error_t **error );

int libfsrefs_block_tree_node_set_sub_node_at_offset(
     libfsrefs_block_tree_node_t *block_tree_node,
     off64_t offset,
     libfsrefs_block_tree_node_t *sub_block_tree_node,
     libcerror_error_t **error );

int libfsrefs_block_tree_node_get_leaf_value_at_offset(
     libfsrefs_block_tree_node_t *block_tree_node,
     off64_t offset,
     libfsrefs_block_descriptor_t **block_descriptor,
     off64_t *block_offset,
     libcerror_error_t **error );

int libfsrefs_block_tree_node_set_leaf_value_at_offset(
     libfsrefs_block_tree_node_t *block_tree_node,
     off64_t offset,
     libfsrefs_block_descriptor_t *block_descriptor,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSREFS_BLOCK_TREE_NODE_H ) */

