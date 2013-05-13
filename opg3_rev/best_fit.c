#include "fit.h"
#include "admin.h"
#include "mem_alloc.h"

long fit(long *mem, long request){
	long admin_index, size, best_fit_size, best_fit_index;

	admin_index = FIRST_INDEX;
	best_fit_size = -1;
	best_fit_index = -1;

	while (1) {
		size = get_block_size(mem, admin_index);

		if (!admin_get_used(mem[admin_index]) && (size >= request)) {
			if (size == request)
				return admin_index;
			if (size < best_fit_size || best_fit_size == -1) {
				best_fit_size = size;
				best_fit_index = admin_index;
			}
		}

		admin_index = admin_get_next_index(mem[admin_index]);

		if (admin_index == 0) {
			if (best_fit_size != -1)
				return best_fit_index;
			return -1;
		}
	}

	return -1;
}