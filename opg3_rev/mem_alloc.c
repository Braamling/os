#include <stdio.h>

#include "mem_alloc.h"
#include "admin.h"
#include "fit.h"

static long *memory;

void mem_init(long mem[MEM_SIZE]) {
	memory = mem;

	/* Anally rape someone. */

	/* Start with one empty block. */
	memory[0] = 1;
	memory[1] = admin_make(0, 0);
}

long mem_get(long request) {
	long available_admin_index;

	available_admin_index = fit(memory, request);
	if (available_admin_index == -1)
		return -1;

	if (alloc_block(memory, available_admin_index, request) == -1)
		return -1;

	return available_admin_index + 1;
}

void mem_free(long index) {
	long admin_index;

	admin_index = index - 1;

	free_block(memory, admin_index);
}

void mem_available(long *empty, long *large, long *n_holes) {
	long index, next_index, block_size;

	*empty = 0;
	*large = 0;
	*n_holes = 0;

	index = FIRST_INDEX;

	printf("=======\n");
	printf("i\tni\tbs\tu\n");

	while (index) {
		block_size = get_block_size(memory, index);
		next_index = admin_get_next_index(memory[index]);

		if (!admin_get_used(memory[index])) {
			*empty += block_size;

			if (block_size > *large)
				*large = block_size;

			*n_holes = *n_holes + 1;
		}

		printf("%ld\t%ld\t%ld\t%d\n", index, next_index, block_size,
			admin_get_used(memory[index]));

		index = next_index;

	}

	printf("=======\n");
}

void mem_exit() {

}
