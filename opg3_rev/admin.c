#include "admin.h"


int in_block_space(long *mem, long block_index) {
	if ((1 < block_index) && (block_index < MEM_SIZE))
		return 1;
	else
		return 0;
}

int get_block_size(long *mem, long block_index) {
	long next_index, size;

	if (!in_block_space(mem, block_index)) 
		return -1;

	next_index = admin_get_next_index(mem, block_index);


	if(next_index == 0){
		size = MEM_SIZE - block_index - 1;
	}else{
		size = next_index - block_index - 1;
	}

	return size;
}