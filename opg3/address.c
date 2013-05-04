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
	long addr_end, i;

	addr_end = get_address_end(mem);

	if (addr_end == (get_address_max(mem) + ADDR_START))
		return -1;

	for (i = addr_end; i >= index; i --)
		mem[i + 1] = mem[i];

	mem[i] = -1;

	return 0;
}

int move_addresses_left(long *mem, long index) {
	long addr_end, i;

	addr_end = get_address_end(mem);

	if (mem[index] != -1)
		return -1;

	for (i = (index + 1); i <= addr_end; i ++)
		mem[i - 1] = mem[i];

	return 0;
}

int get_block_size(long *mem, long index) {
	if (index >= MEM_SIZE)
		return -1;

	if (index < 0)
		return -1;

	return mem[index];
}

int insert_address(long* mem, long index, long address) {
	/* No address can be written outside the address space */
	if (get_address_max(mem) <= get_address_count(mem))
		return -1;

	if (!in_addr_space(mem, index))
		return -1;

	if (move_addresses_right(mem, index) == -1)
		return -1;

	mem[index] = address;

	return 0;
}

int in_addr_space(long *mem, long index){
	if (get_address_max(mem) < index || index <= ADDR_COUNT_INDEX)
		return 0;
	else
		return 1;
}

int remove_address(long* mem, long index) {
	/* No address can be written outside the address space */
	if (!in_addr_space(mem, index))
		return -1;	

	mem[index] = -1;

	if (move_addresses_left(mem, index) == -1)
		return -1;

	return 0;
}
