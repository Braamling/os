#include <math.h>

#include "admin.h"
#include "mem_alloc.h"

long admin_make(long next_index, int used) {
	long used_mask, admin;

	/* Destroy unwanted bits from the 'used' int and Move the 'used' bit to
	 * the 31st place. */
	used_mask = (used & 1) << 31;

	/* Combine the used mask with the index of the next block. */
	admin = next_index | used_mask;

	return admin;
}

int admin_get_used(long admin) {
	int used;

	/* Turn off all irrelevant bits and move the bit the the 1st place. */
	used = (int)((admin & (long)pow(2, 31)) >> 31);

	return used;
}

long admin_get_next_index(long admin) {
	long mask, next_index;

	/* Create a mask for the 'used' it */
	mask = (long)pow(2, 31) - 1;

	/* Mask the 'used' bit. */
	next_index = admin & mask;

	return next_index;
}

int in_block_space(long *mem, long block_index) {
	if ((1 <= block_index) && (block_index < MEM_SIZE))
		return 1;
	else
		return 0;
}

long get_block_size(long *mem, long block_index) {
	long next_index, size;

	if (!in_block_space(mem, block_index)) 
		return -1;

	next_index = admin_get_next_index(mem[block_index]);


	if (next_index == 0) {
		size = MEM_SIZE - block_index - 1;
	}
	else {
		size = next_index - block_index - 1;
	}

	return size;
}

<<<<<<< HEAD
long merge_block(long *mem, long first_index, long second_index) {
	if (first_index < second_index)
		return -1;

	if (!in_block_space(mem, first_index))
		return -1;

	if (!in_block_space(mem, second_index))
		return -1;

	mem[first_index] = mem[second_index];

	return 0;
}

long free_block(long *mem, long block_index) {
	int index, next_index;

	if (!in_block_space(mem, block_index))
		return -1;

	mem[block_index] = admin_make(block_index, 0);

	index = 1;

	while (index) {
		next_index = admin_get_next_index(mem[index]);

		if (next_index != 0) {
			if (!admin_get_used(mem[next_index]) && 
					!admin_get_used(mem[index])) {
				merge_block(mem, index, next_index);
			}

			index = next_index;
		}
	}

	return 0;
}
=======
int get_block_count(long *mem) {
	return mem[0];
}
>>>>>>> 8ae1e04e673e8ebb982e5a026e651e7ad1e97587
