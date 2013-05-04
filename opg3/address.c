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


int insert_address(long *mem, long index, long address) {
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

int in_block_space(long *mem, long index){
	if (get_address_max(mem) < index || index > MEM_SIZE)
		return 1;
	else
		return 0;
}

int remove_address(long *mem, long index) {
	if (!in_addr_space(mem, index))
		return -1;	

	mem[index] = -1;

	if (move_addresses_left(mem, index) == -1)
		return -1;

	return 0;
}

int free_mem(long *mem, long addr_index){
	long block_index, temp_block_index, temp_size, new_size;

	if(!address_is_used(mem[addr_index]))
		return -1;

	block_index = get_index(mem[addr_index]);

	/* Merge the block after the to be free'ed memory */
	if(!address_is_used(mem[addr_index + 1])){

		temp_size = get_block_size(mem, block_index);

		if (temp_size == -1)
			return -1;

		temp_block_index = get_index(mem[addr_index + 1]);

		if (temp_block_index == -1) 
			return -1;

		new_size = temp_size + get_block_size(mem, temp_block_index);

		set_block_size(mem, block_index, new_size);

		remove_address(mem, mem[addr_index + 1]);
	}

	/* Merge the block before the to be free'ed memory */
	if(!address_is_used(mem[addr_index - 1])){

		temp_size = get_block_size(mem, block_index);

		if (temp_size == -1)
			return -1;

		block_index = get_index(mem[addr_index - 1]);

		if (block_index == -1) 
			return -1;

		new_size = temp_size + get_block_size(mem, block_index);

		set_block_size(mem, block_index, new_size);

		remove_address(mem, mem[addr_index]);
	}


	return 0;
}

int get_block_size(long *mem, long block_index) {
	if(!in_block_space(mem, block_index))
		return -1;

	return mem[block_index+1];
}


int set_block_size(long *mem, long block_index, long size){
	if(!in_block_space(mem, block_index))
		return -1;

	mem[block_index + 1] = size;
	return 0;
}

