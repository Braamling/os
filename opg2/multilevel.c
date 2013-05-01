/* Authors:
 * -Bas van den Heuvel
 * -Bram van den Akker */

#include <stdlib.h>
#include <stdio.h>

#include "schedule.h"
#include "pcb_control.h"
#include "multilevel.h"

/* Initialize a pcb admin.
 *
 * Arguments:
 * -pcb_admin **admin: Pointer to the pointer to the struct.
 *
 * Results:
 * -Succes: 0.
 * -Failure: -1. */
int pcb_admin_init(pcb_admin **admin) {
	*admin = malloc(sizeof(pcb_admin));

	if (*admin) {
		(*admin)->queue_level = 0;
		return 0;
	}

	return -1;
}

/* Destroy a pcb admin from memory.
 *
 * Arguments:
 * -pcb_admin *admin: Pointer to the struct.
 *
 * Results:
 * -Succes: 0.
 * -Failure: -1. */
int pcb_admin_destroy(pcb_admin *admin) {
	if (!admin)
		return -1;

	free(admin);
	return 0;
}

/* Find the tail of a level in the ready queue.
 * 
 * Arguments:
 * -int level: Queue level to search in.
 * 
 * Results:
 * -Succes: A pointer to the last pcb in the queue.
 * -Failure: NULL. */
pcb *pcb_find_level_tail(int level) {
	pcb *item;

	if (!ready_proc)
		return NULL;

	if (pcb_get_queue_level(ready_proc) > level)
		return NULL;

	item = ready_proc;
	while ((pcb_get_queue_level(item) <= level) && item->next)
		item = pcb_get_next(item);

	return item;
}

/* Put an item at the end of a certain level in the ready queue.
 * 
 * Arguments:
 * -pcb *item: The item to put in the ready queue.
 * -int level: Queue level to put the item in in.
 * 
 * Results:
 * -Succes: 0.
 * -Failure: -1. */
int pcb_move_to_level(pcb *item, int level) {
	pcb* level_tail;

	if (!item)
		return -1;

	if (level < 0)
		return -1;

	pcb_set_queue_level(item, level);

	level_tail = pcb_find_level_tail(level);

	if (!level_tail) {

		/* If there are no entries in the ready proc queue the item replaces
		 * the ready_proc. */
		if (!ready_proc){
			ready_proc = item;
			return 0;
		}

		/* if inserted in the first queue with no entries the entry can be 
		 * placed before the first entry in the ready proc. */
		ready_proc = pcb_insert_before(item, ready_proc);
		return 0;
	}

	ready_proc = pcb_insert_after(item, level_tail);
	return 0;
}

/* Increase the queue level of an item in the ready queue after an timeout.
 * 
 * Arguments:
 * -pcb *item: The item to increase in queue level.
 * 
 * Results:
 * -Succes: 0.
 * -Failure: -1. */
int pcb_increase_level(pcb *item) {
	if (!item)
		return -1;

	((pcb_admin *)item->your_admin)->queue_level ++;

	return 0;
}

/* Places an item in de ready queue on level 1.
 * 
 * Arguments:
 * -pcb *item: The item to increase in queue level.
 * 
 * Results:
 * -Succes: 0.
 * -Failure: -1. */
int pcb_place_in_ready_queue(pcb *item) {
	pcb_admin *admin;

	if (!item)
		return -1;

	pcb_admin_init(&admin);
	item->your_admin = admin;

	return pcb_move_to_level(item, 1);
}

/* Set a specific level for an item.
 * 
 * Arguments:
 * -pcb *item: The item to increase in queue level.
 * 
 * Results:
 * -Succes: 0.
 * -Failure: -1. */
int pcb_set_queue_level(pcb *item, int level) {
	if (!item)
		return -1;

	((pcb_admin *)item->your_admin)->queue_level = level;

	return 0;
}

/* Get the level of a specific pcb item.
 * 
 * Arguments:
 * -pcb *item: The item to increase in queue level.
 * 
 * Results:
 * -Succes: The queue level of the item.
 * -Failure: -1. */
int pcb_get_queue_level(pcb *item) {
	if (!item)
		return -1;

	return ((pcb_admin *)item->your_admin)->queue_level;
}
