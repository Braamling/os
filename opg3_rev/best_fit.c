/* File: best_fit.c
 * Authors: Bas van den Heuvel & Bram van den Akker
 *
 * This file contains the fit function for the best fit algorithm. The function
 * is used in mem_alloc.c */

#include "fit.h"
#include "admin.h"
#include "mem_alloc.h"

/* Find the best fit block.
 *
 * Arguments:
 * -long *mem: The memory array.
 * -long request: The requested memory size
 *
 * Results:
 * -Success: The best fit block.
 * -Failure: -1. */
long fit(long *mem, long request){
	long admin_index, size, best_fit_size, best_fit_index;

	admin_index = FIRST_INDEX;

	/* Set the size and index to -1 to indicate that no fit is found yet. */
	best_fit_size = -1;
	best_fit_index = -1;

	while (1) {
		size = get_block_size(mem, admin_index);

		if (!admin_get_used(mem[admin_index]) && (size >= request)) {

			/* If the block is the same size as the request it will be used. */
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
