#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "address.h"
#include "mem_alloc.h"

static long *memory;

void mem_init(long mem[MEM_SIZE]) {
	long index = 230;
	int used = 1;
	long address = address_set(index, used);
	memory = mem;
	printf("index: %ld, used: %d, address: %ld.\n",
			index, used, address);
	printf("used is: %d.\n", address_is_used(address));
	printf("index is: %ld. \n", get_index(address));
	exit(0);
}

long mem_get(long request) {
	long addr_index, block_index, block_size;
	int not_found;
	addr_index = ADDR_START;

	while(1) {
		if(!address_is_used(mem[addr_index])) {
			if((block_index = get_index(mem[addr_index])) != -1){
				block_size = get_block_size(mem, block_index);
				if(block_size => request){
					alloc_mem(mem, addr_index, request);
					return block_index + 2;
				}
			}
		}

		if(!(in_addr_space(mem, addr_index ++))){
			return -1;
		}
	}
	return -1;
}

void mem_free(long index) {
	long addr_index;

	addr_index = mem[index - 2];

	free_mem(mem, addr_index);
}

void mem_available(long *empty, long *large, long *n_holes) {
	
}

void mem_exit() {

}
