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
	if (set_block_size(memory, gap_index, gap_size) == -1)
		exit(EXIT_FAILURE);

	memory[gap_index] = ADDR_START;

	for (i = (ADDR_START + 1); i <= addr_end; i ++)
		memory[i] = -1;
}

long mem_get(long request) {
	return -1;
}

void mem_free(long index) {

}

void mem_available(long *empty, long *large, long *n_holes) {
	
}

void mem_exit() {

}
