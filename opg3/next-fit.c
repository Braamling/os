#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mem_alloc.h"

static long *memory;

void mem_init(long mem[MEM_SIZE]) {
    int i;
    
    memory = mem;

    for (i = 0; i < MEM_SIZE; i ++) {
        memory[i] = 0;
    }

    memory[4] = 18;
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
