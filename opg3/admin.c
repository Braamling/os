/* File: admin.c
 * Authors: Bas van den Heuvel & Bram van den Akker.
 * 
 * This file contains all the functions for administrating a virtual memory.
 * The memory works like a linked list. Every part of memory, gap or used
 * block, starts with a pointer to the next block. The pointer also contains a
 * bit that indicates whether the block is a gap or if it is used. The last
 * block has no pointer to a next block, but uses a 0 to indicate it is the
 * last block. */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "admin.h"
#include "mem_alloc.h"

/* Create an admin integer for a block, with a pointer to the next block and a
 * `used` indicator.
 *
 * Arguments:
 * -long next_index: The index of the next block (0 when the relevant block is
 *  the last block in memory).
 * -int used: 1 when a block is in use and 0 when a block is not used.
 *
 * Results:
 * -The created admin integer. */
long admin_make(long next_index, int used) {
	long used_mask, admin;

	/* Destroy unwanted bits from the 'used' int and move the 'used' bit to
	 * the 31st place. */
	used_mask = (used & 1) << 31;

	/* Combine the used mask with the index of the next block. */
	admin = next_index | used_mask;

	return admin;
}

/* Check if a block is used by looking at its admin integer.
 *
 * Arguments:
 * -long admin: The admin integer of the block.
 *
 * Results:
 * -1 when used and 0 when not used. */
int admin_get_used(long admin) {
	int used;

	/* Turn off all irrelevant bits and move the bit the the 1st place. */
	used = (int)((admin & (long)pow(2, 31)) >> 31);

	return used;
}

/* Find the index of the next block after another by its admin integer.
 *
 * Arguments:
 * -long admin: The admin integer of the block.
 *
 * Results:
 * -The index of the next admin block. */
long admin_get_next_index(long admin) {
	long mask, next_index;

	/* Create a mask for the 'used' it */
	mask = (long)pow(2, 31) - 1;

	/* Mask the 'used' bit. */
	next_index = admin & mask;

	return next_index;
}

/* Find whether a block is in the block space.
 *
 * Arguments:
 * -long *mem: The memory array.
 * -long block_index: The index of the block of which the size is checked.
 *
 * Results:
 * -Success: 0 if it's not in the block space and 1 if it's in the block space.
 * -Failure: n/a. */
int in_block_space(long *mem, long block_index) {
	if ((1 <= block_index) && (block_index < MEM_SIZE))
		return 1;
	else
		return 0;
}

/* Find the size of a block.
 *
 * Arguments:
 * -long *mem: The memory array.
 * -long block_index: The index of the block.
 *
 * Results:
 * -Success: The size of the block.
 * -Failure: -1. */
long get_block_size(long *mem, long block_index) {
	long next_index, size;

	if (!in_block_space(mem, block_index)) 
		return -1;

	next_index = admin_get_next_index(mem[block_index]);

	/* For the last block, calculate the block size by looking at its index and
	 * the total size of the memory. For any other block, calculate it by
	 * looking at its index and the index of the block following it. */
	if (next_index == 0) {
		size = MEM_SIZE - block_index - ADMIN_SIZE;
	}
	else {
		size = next_index - block_index - ADMIN_SIZE;
	}

	return size;
}

/* Merge two empty blocks together.
 *
 * Arguments:
 * -long *mem: The memory array.
 * -long first_index: The index of the first block.
 * -long second_index: The index of the second block.
 *
 * Results:
 * -Success: 0.
 * -Failure: -1. */
int merge_block(long *mem, long first_index, long second_index) {
	if (first_index >= second_index)
		return -1;

	if (!in_block_space(mem, first_index))
		return -1;

	if (!in_block_space(mem, second_index))
		return -1;

	/* Copying the second block's pointer is enough. */
	mem[first_index] = mem[second_index];

	mem[0] --;

	return 0;
}

/* Free memory and merge double gaps.
 *
 * Arguments:
 * -long *mem: The memory array.
 * -long addr_index: The index of the address that has to be freed.
 *
 * Results:
 * -Success: 0.
 * -Failure: -1. */
int free_block(long *mem, long block_admin_index) {
	long index, next_index;

	if (!in_block_space(mem, block_admin_index))
		return -1;

	/* Set the block unused */
	next_index = admin_get_next_index(mem[block_admin_index]);
	mem[block_admin_index] = admin_make(next_index, 0);

	index = FIRST_INDEX;

	/* Merge all blocks that are empty and next to each other */
	while (index) {
		next_index = admin_get_next_index(mem[index]);

		if (next_index == 0)
			break;

		if (!admin_get_used(mem[index])){
			if (!admin_get_used(mem[next_index])) {
				merge_block(mem, index, next_index);
				next_index = index;
			}
		}

		index = next_index;
	}

	return 0;
}

/* Find the amount of blocks.
 *
 * Arguments:
 * -long *mem: The memory array.
 *
 * Results:
 * -The amount of blocks. */
int get_block_count(long *mem) {
	return mem[0];
}

/* Allocate a block in memory.
 *
 * Arguments:
 * -long *mem: The memory array.
 * -long gap_addr_index: The index of the admin integer of the gap that has to
 *  be used for the alloc.
 * -long size: The size of the memory request.
 *
 * Results:
 * -Success: 0.
 * -Failure: -1. */
int alloc_block(long *mem, long gap_admin_index, long size) {
	long old_gap_size, gap_next_index, block_next_index, block_admin_index;

	if (admin_get_used(mem[gap_admin_index]))
		return -1;

	old_gap_size = get_block_size(mem, gap_admin_index);
	if (old_gap_size < size)
		return -1;

	gap_next_index = admin_get_next_index(mem[gap_admin_index]);

	/* The block will point to the remainder of the gap. */
	block_next_index = gap_admin_index + size + 1;

	block_admin_index = gap_admin_index;
	gap_admin_index = block_next_index;

	/* If the old gap was of inequal length to the size of the to be allocated
	 * block, the gap will have a new admin integer. Otherwise, there is no
	 * gap left. If the gap was the last block in memory, the block will be the
	 * new last block. */
	if (old_gap_size != size)
		mem[gap_admin_index] = admin_make(gap_next_index, 0);
	else if (gap_next_index == 0)
		block_next_index = 0;

	mem[block_admin_index] = admin_make(block_next_index, 1);

	return 0;
}
