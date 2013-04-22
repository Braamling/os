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

int pcb_insert_before(pcb *item, pcb *next_item){

	/* Check whether there is a previous node. */
	if(next_item->prev->next != NULL){
		next_item->prev->next = item;
	}else{
		next_item->prev->next = NULL;
	}
	item->prev = next_item->prev;
	next_item->prev = item;
	item->next = next_item;

	return 0;
}

int pcb_insert_after(pcb *item, pcb *prev_item){

	/* Check whether there is a next node. */
	if(prev_item->next->prev != NULL){
		prev_item->next->prev = item;
	}else{
		prev_item->next->prev = NULL;
	}
	item->next = prev_item->next;
	item->prev = prev_item;
	prev_item->next = item;

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
