#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "address.h"
#include "mem_alloc.h"

static long *memory;

void mem_init(long mem[MEM_SIZE]) {
	long addr_end, gap_index, gap_size, i;

	memory = mem;

	memory[ADDR_MAX_INDEX] = ADDR_START + ADDR_SPACE;
	memory[ADDR_COUNT_INDEX] = 1;

	addr_end = get_address_end(memory);

	gap_index = addr_end + 1;
	gap_size = MEM_SIZE - gap_index;

	memory[ADDR_START] = address_set(gap_index, 0);
	if (set_block_size(memory, gap_index, gap_size) == -1) {
		exit(EXIT_FAILURE);
	}

	memory[gap_index] = ADDR_START;

	for (i = (ADDR_START + 1); i <= addr_end; i ++)
		memory[i] = -1;
}

long mem_get(long request) {
	long addr_index, block_index, block_size;

	addr_index = ADDR_START;

	while (1) {
		if (!address_is_used(memory[addr_index])) {

			if ((block_index = get_index(memory[addr_index])) != -1) {

				block_size = get_block_size(memory, block_index);
				if (block_size >= request) {
					alloc_mem(memory, addr_index, request);
					/*printf("get %ld\n", get_address_count(memory));*/
					return block_index + 2;
				}
			}
		}

		addr_index ++;
		if (!in_addr_space(memory, addr_index))
			return -1;

	}
	return -1;
}

void mem_free(long index) {
	long addr_index;


	addr_index = memory[index - 2];
	if (!in_addr_space(memory, addr_index))
		return;

	free_mem(memory, addr_index);
	/*printf("free %ld\n", get_address_count(memory));*/
}

void mem_available(long *empty, long *large, long *n_holes) {
	long addr_count, i, addr, index, size;

	addr_count = get_address_count(memory);

	*empty = 0;
	*large = 0;
	*n_holes = 0;

	for (i = 0; i < addr_count; i ++) {
		addr = memory[i + ADDR_START];
		index = get_index(addr);
		size = get_block_size(memory, index);

		if (!address_is_used(addr)) {
			*empty += size + 2;
			*n_holes += 1;
			if (size > *large)
				*large = size;
		}
	}
}

void mem_exit() {

}
