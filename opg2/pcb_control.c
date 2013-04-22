#include <stdlib.h>

#include "schedule.h"
#include "pcb_control.h"

int pcb_insert_before(pcb *item, pcb *next_item){
	if (item == NULL)
		return -1;

	if (next_item == NULL)
		return -1;
	
	item->next = next_item;

	item->prev = next_item->prev;

	if (item->prev != NULL) {
		item->prev->next = item;
	}

	next_item->prev = item;

	return 0;
}

int pcb_insert_after(pcb *item, pcb *prev_item){
	if (item == NULL)
		return -1;

	if (prev_item == NULL)
		return -1;

	item->prev = prev_item;

	item->next = prev_item->next;

	if (item->next != NULL) {
		item->next->prev = item;
	}

	prev_item->next = item;

	return 0;
}

int pcb_move_before(pcb *item, pcb *new_next){
	if(pcb_remove(item) == -1){
		return -1;
	}
	if(pcb_insert_before(item, new_next) == -1){
		return -1;
	}
	return 0;
}

int pcb_move_after(pcb *item, pcb *new_next){
	if(pcb_remove(item) == -1){
		return -1;
	}
	if(pcb_insert_after(item, new_next) == -1){
		return -1;
	}
	return 0;
}

/* Remove a PCB from a list of PCB's. */
pcb *pcb_remove(pcb *item) {
	pcb *new_next;

	if (item == NULL)
		return NULL;

	if (item->prev != NULL)
		item->prev->next = item->next;

	new_next = item->next;

	if (item->next != NULL)
		item->next->prev = item->prev;

	item->prev = NULL;
	item->next = NULL;

	return new_next;
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

/* Get the first item in a list of PCB's. */
pcb *pcb_get_first(pcb *list) {
	pcb *item;

	if (list == NULL)
		return NULL;

	item = list;
	while (item->prev != NULL)
		item = item->prev;

	return item;
}

/* Get the last item in a list of PCB's. */
pcb *pcb_get_last(pcb *list) {
	pcb *item;

	if (list == NULL)
		return NULL;

	item = list;
	while (item->next != NULL)
		item = item->next;

	return item;
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
