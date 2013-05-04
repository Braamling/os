#include <stdio.h>
#include <math.h>

#include "address.h"

long address_set(long index, int used) {
	long used_mask;
	long address;

	/* Destroy unwanted bits from the 'used' int. */
	used_mask = used & 1;

	/* Move the 'used' bit to the 31st place. */
	used_mask = used_mask << 31;
	
	/* Combine the used mask with the memory index. */
	address = index | used_mask;

	return address;
}

int address_is_used(long address) {

	/* Turn off all irrelevant bits. */
	address = address & (long)pow(2, 31);

	/* Move the bit the the 1st place. */
	address = address >> 31;

	return (int)address;
}

/* Get the index of an address. Give the address as a long. */ 
long get_index(long address) {
	long index, mask;

	/* Create a mask for the 'used' it */
	mask = (long)pow(2, 31)-1;

	/* Mask the 'used' bit. */
	index = address & mask;

	return (long)index;
}

int get_block_size(long *mem, long index) {
	if (!mem)
		return -1;

	if (index < 0)
		return -1;

	return mem[index];
}

int insert_address(long* mem, long index, long address) {
	if (!mem)
		return -1;

	if (mem[ADDR_MAX_INDEX] <= mem[ADDR_COUNT_INDEX])
		return -1;

	if (mem[ADDR_MAX_INDEX] < index)
		return -1;

	move_right(index);

	mem[index] = address;

	return 0;
}



