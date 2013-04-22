#include <stdlib.h>

#include "schedule.h"
#include "pcb_control.h"

/* Remove a PCB from a list of PCB's. */
int pcb_remove(pcb *item) {
	item->prev->next = item->next;
	item->next->prev = item->prev;

	return 0;
}

/* Get the next item in a list of PCB's. */
pcb *pcb_get_next(pcb *item) {
	if (item == NULL)
		return NULL;

	return item->next;
}

/* Get the previous item in a list of PCB's. */
pcb *pcb_get_prev(pcb *item) {
	if (item == NULL)
		return NULL;

	return item->prev;
}

/* Get the amount of MEM the PCB needs. */
long pcb_get_mem_need(pcb *item) {
	if (item == NULL)
		return -1;

	return item->MEM_need;
}

/* Set the base of the PCB's MEM. */
int pcb_set_mem_base(pcb *item, long MEM_base) {
	if (item == NULL)
		return -1;

	item->MEM_base = MEM_base;
	return 0;
}
