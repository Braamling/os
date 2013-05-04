#include <stdio.h>
#include <math.h>

#include "mem_alloc.h"
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
	mask = (long)pow(2, 31) - 1;

	/* Mask the 'used' bit. */
	index = address & mask;

	return index;
}

long get_free_address_slot(long *mem) {
	long i, addr_end;

	addr_end = get_address_end(mem);

	for (i = ADDR_START; (i <= addr_end) && (mem[i] != -1); i ++);

	if (i == addr_end)
		return -1;

	return i;
}

long get_address_max(long *mem) {
	return mem[ADDR_MAX_INDEX];
}

long get_address_count(long *mem) {
	return mem[ADDR_COUNT_INDEX];
}

long get_address_end(long *mem) {
	return get_address_count(mem) + ADDR_START;
}

int move_addresses_right(long *mem, long index) {
	return 0;
}

int get_block_size(long *mem, long index) {
	if (!mem) {
		return -1;
	}

	if (index < 0) {
		return -1;
	}

	return mem[index];
}

int insert_address(long* mem, long index) {
	if (!mem) {
		return -1;
	}

	if (mem[ADDR_MAX_INDEX] <= mem[ADDR_COUNT_INDEX]) {
		return -1;
	}

	if (mem[ADDR_MAX_INDEX] < index) {
		return -1;
	}

	return 0;
}
