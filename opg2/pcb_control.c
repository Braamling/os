#include <stdlib.h>

#include "schedule.h"
#include "pcb_control.h"

/* Get the next item in a list of PCB's.
 *
 * Arguments:
 * -pcb *item: The item to get the next item from.
 *
 * Results:
 * -Success: The next item relative to the item.
 * -Failure: NULL. */
pcb *pcb_get_next(pcb *item) {
	if (!item)
		return NULL;

	return item->next;
}

/* Get the previous item in a list of PCB's.
 *
 * Arguments:
 * -pcb *item: The item to get the previous item from.
 *
 * Results:
 * -Success: The previous item relative to the item.
 * -Failure: NULL. */
pcb *pcb_get_prev(pcb *item) {
	if (!item)
		return NULL;

	return item->prev;
}

/* Get the amount of MEM the PCB needs.
 *
 * Arguments:
 * -pcb *item: The item to get the memory needed from.
 *
 * Results:
 * -Success: The memory the item needs.
 * -Failure: -1. */
long pcb_get_mem_need(pcb *item) {
	if (!item)
		return -1;

	return item->MEM_need;
}

/* Set the base of MEM for the PCB.
 *
 * Arguments:
 * -pcb *item: The item to set the memory base to.
 *
 * Results:
 * -Success: 0.
 * -Failure: -1. */
int pcb_set_mem_base(pcb *item, long MEM_base) {
	if (!item)
		return -1;

	item->MEM_base = MEM_base;
	return 0;
}

/* Find the head of a list.
 *
 * Arguments:
 * -pcb *item: The item in the list to look through.
 *
 * Results:
 * -Success: The head of the list.
 * -Failure: NULL. */
pcb *pcb_find_head(pcb *item) {
	if (!item)
		return NULL;

	while (item->prev)
		item = pcb_get_prev(item);

	return item;
}

/* Find the tail of a list.
 *
 * Arguments:
 * -pcb *item: The item in the list to look through.
 *
 * Results:
 * -Success: The tail of the list.
 * -Failure: NULL. */
pcb *pcb_find_tail(pcb *item) {
	if (!item)
		return NULL;

	while (item->next)
		item = pcb_get_next(item);

	return item;
}

/* Insert an item into a list of PCB's before another one.
 *
 * Arguments:
 * -pcb *item: The item to insert into the list.
 * -pcb *before: The before which the item will be inserted.
 *
 * Results:
 * -Success: The head of the list.
 * -Failure: NULL. */
pcb *pcb_insert_before(pcb *item, pcb *before) {
	if (!item)
		return NULL;

	if (!before)
		return NULL;

	item->next = before->next;
	before->next = item;

	if (item->next)
		item->next->prev = item;

	item->prev = before;

	return pcb_find_head(item);
}

/* Insert an item into a list of PCB's after another one.
 *
 * Arguments:
 * -pcb *item: The item to insert into the list.
 * -pcb *after: The after which the item will be inserted.
 *
 * Results:
 * -Success: The head of the list.
 * -Failure: NULL. */
pcb *pcb_insert_after(pcb *item, pcb *after) {
	if (!item)
		return NULL;

	if (!after)
		return NULL;

	item->prev = after->prev;
	after->prev = item;

	if (item->prev)
		item->prev->next = item;

	item->next = after;

	return pcb_find_head(item);
}

/* Remove an item from a list of PCB's.
 *
 * Arguments:
 * -pcb *item: The item in the list.
 *
 * Results:
 * -Succes: The head of the list.
 * -Failure: NULL. */
pcb *pcb_remove(pcb *item) {
	pcb *new_head, *next;

	if (!item)
		return NULL;

	if (!item->next) {
		new_head = item->prev;
		item->prev = NULL;
		new_head->next = NULL;

		return new_head;
	}

	next = item->next;
	next->prev = item->prev;

	if (item->prev)
		item->prev->next = next;

	item->next = item->prev = NULL;

	return pcb_find_head(next);
}
