/* File: mem_alloc.c
 *
 * Authors: Bas van den Heuvel & Bram van den Akker
 *
 * This file contains all functions used by the memory simulator. It uses
 * functionality from admin.c and the algorithms from next_fit.c and best_fit.c
 * to manage virtual memory.
 *
 * The comments for each function were copied from mem_alloc.h. Since these
 * comments weren't written by us, but merely provided for us, we did not
 * translate them into English. */

#include <stdio.h>

#include "mem_alloc.h"
#include "admin.h"
#include "fit.h"

static long *memory;
long a, b, c;

/* mem_init wordt aangeroepen voor enige andere procedure uit deze file wordt
 * gebruikt. Hij initialseert de memory-manager en zorgt ervoor dat het te
 * beheren geheugen daar bekend is. */
void mem_init(long mem[MEM_SIZE]) {
	memory = mem;

	/* Start with one empty block. */
	memory[0] = 1;
	memory[1] = admin_make(0, 0);
}

/* mem_get alloceert een stuk geheugen ter grootte van request - indien
 * mogelijk en geeft de index van het eerste element van dat stuk geheugen
 * terug. Indien de aanvraag niet gehonoreerd kan worden, wordt -1
 * teruggegeven. */
long mem_get(long request) {
	long available_admin_index;

	/* Find the gap to put the request in. The algorithm used depends on with
	 * what .o file the program is compiled. */
	available_admin_index = fit(memory, request);
	if (available_admin_index == -1)
		return -1;

	if (alloc_block(memory, available_admin_index, request) == -1)
		return -1;

	/* The actual memory available starts one place from the requested block.
	 * The first place is used for administration purposes. */
	return available_admin_index + 1;
}

/* mem_free wordt aangeroepen om een eerder verkregen stuk geheugen op locatie
 * index weer vrij te geven. Index moet een geldige waarde hebben. */
void mem_free(long index) {
	long admin_index;

	/* The index of the block's administration is one place before the memory
	 * index that has been given to the simulator. */
	admin_index = index - 1;

	free_block(memory, admin_index);
}

/* mem_available vertelt de gebruiker hoeveel geheugen er nog beschikbaar is:
 * -empty: totale hoeveelheid vrije ruimte.
 * -large: omvang van het grootste gat, gecorrigeerd voor administratie.
 * -n_holes: het aantal gaten. */
void mem_available(long *empty, long *large, long *n_holes) {
	long index, next_index, block_size;

	*empty = 0;
	*large = 0;
	*n_holes = 0;

	index = FIRST_INDEX;

	/*printf("=======\n");
	printf("i\tni\tbs\tu\n");*/

	while (index) {

		/* Though the block size is not relevant if the block is not used, it
		 * is requested outside the if statement to check this. This is done
		 * for debuggin purposes (see the outcommented lines of code). */
		block_size = get_block_size(memory, index);
		next_index = admin_get_next_index(memory[index]);

		if (!admin_get_used(memory[index])) {
			*empty += block_size;

			if (block_size > *large)
				*large = block_size;

			*n_holes = *n_holes + 1;
		}

		/*printf("%ld\t%ld\t%ld\t%d\n", index, next_index, block_size,
			admin_get_used(memory[index]));*/

		index = next_index;

	}

	/*printf("=======\n");*/
}

/* mem_exit wordt alleen als laatste routine aangeroepen en beeindigt het
 * gebruik van de memory manager. Deze routine zorgt zonodig voor opruimen en
 * afronden. */
void mem_exit() {

}
