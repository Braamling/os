#include <stdio.h>

#include "address.h"
#include "mem_alloc.h"

static long mem[MEM_SIZE];

int main() {
	long addr1, addr2, empty, large, n_holes;

	mem_init(mem);
	mem_available(&empty, &large, &n_holes);
	printf("empty: %ld, large: %ld, n_holes: %ld.\n", empty, large, n_holes);

	addr1 = mem_get(128);
	mem_available(&empty, &large, &n_holes);
	printf("empty: %ld, large: %ld, n_holes: %ld.\n", empty, large, n_holes);

	addr2 = mem_get(128);
	mem_available(&empty, &large, &n_holes);
	printf("empty: %ld, large: %ld, n_holes: %ld.\n", empty, large, n_holes);

	mem_free(addr1);
	mem_available(&empty, &large, &n_holes);
	printf("empty: %ld, large: %ld, n_holes: %ld.\n", empty, large, n_holes);

	addr1 = mem_get(127);
	mem_available(&empty, &large, &n_holes);
	printf("empty: %ld, large: %ld, n_holes: %ld.\n", empty, large, n_holes);

	mem_exit();
	return 0;
}
