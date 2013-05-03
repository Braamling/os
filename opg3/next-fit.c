#include <stdlib.h>
#include <stdio.h>

#include "mem_alloc.h"

void mem_init(long mem[MEM_SIZE]){
	
}

long  mem_get(long request){
	printf("request, %ld", request);
	return 0;
}

void mem_free(long index){
	printf("test");
}

void mem_available(long *empty, long *large, long *n_holes){
	printf("test");
}

void mem_exit(){
	printf("test");
}
