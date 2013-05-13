/* File: next_fit.c
 * Authors: Bas van den Heuvel & Bram van den Akker
 *
 * This file contains the fit function for the next fit algorithme. 
 * The function is used in mem_alloc.c */

#include "fit.h"
#include "admin.h"
#include "mem_alloc.h"

/* find the first empty block with the size greater or equal than request 
 *
 * Arguments:
 * -long *mem: The memory array.
 * -long request: The requested memory size
 *
 * Results:
 * -Success: The first fit block.
 * -Failure: -1. */
long fit(long *mem, long request){
	long admin_index, size;

	admin_index = FIRST_INDEX;

	while (1) {
		size = get_block_size(mem, admin_index);

		if (!admin_get_used(mem[admin_index]) && (size >= request)) {
			return admin_index;
		}

		admin_index = admin_get_next_index(mem[admin_index]);

		/* When no fit is found return -1 */
		if (admin_index == 0) {
			return -1;
		}
	}

	return -1;
}