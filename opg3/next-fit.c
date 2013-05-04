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
	printf("index: %ld, used: %d, address: %ld.\n",
			index, used, address);
	printf("used is: %d.\n", address_is_used(address));
	exit(0);
}

long mem_get(long request) {
	printf("Mem 4: %ld.\n", memory[4]);
	return -1;
}

void mem_free(long index) {
	printf("test");
}

void mem_available(long *empty, long *large, long *n_holes) {
	printf("test");
}

void mem_exit() {
	printf("test");
}