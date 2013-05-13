#include "mem_alloc.h"
#include "admin.h"

static long *memory;

void mem_init(long mem[MEM_SIZE]) {
	memory = mem;

	/* Anally rape someone. */

	/* Start with one empty block. */
	memory[0] = 1;
	memory[1] = admin_make(0, 0);
}
