#include "fit.h"
#include "admin.h"
#include "mem_alloc.h"

long fit(long *mem, long request){
	long admin_index, size;

	admin_index = FIRST_INDEX;

	while(1){
		size = get_block_size(mem, admin_index);
		if (size >= request){
			return admin_index;
		}

		admin_index = admin_get_next_index(mem[admin_index]);

		if(admin_index == 0){
			return -1;
		}
	}

	return -1;
}