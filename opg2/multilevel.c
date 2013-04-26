#include <stdlib.h>

#include "schedule.h"
#include "pcb_control.h"
#include "multilevel.h"

/* Find the tail of a level in the ready Queue.
 * 
 * Arguments:
 * -pcb *queue: The readyqeue to search in.
 * -int level: Queue level to search in.
 * 
 * Results:
 * -Succes: A pointer to the last pcb in de queue.
 * -Fail: NULL. */
pcb *pcb_find_level_tail(int level){
	pcb *item;

	if (!ready_proc)
		return NULL;

	if(pcb_get_queue_level(ready_proc) > level)
		return NULL;

	item = ready_proc;
	while (item->next && (pcb_get_queue_level(item) <= level))
		item = pcb_get_next(item);

	return item;

}

/* Put an item at the end of a certain queue level.
 * 
 * Arguments:
 * -pcb *queue: The readyqueue to put the item in.
 * -pcb *item: The item to put in the readyqueue.
 * -int level: Queue level to put the item in in.
 * 
 * Results:
 * -Succes: A pointer to the last pcb in de queue.
 * -Fail: NULL. */
int pcb_move_to_level(pcb *item, int level){
	pcb* level_tail;

	if (!item)
		return -1;

	if(level < 0)
		return -1;

	((pcb_admin *)item->your_admin)->queue_level = level;

	level_tail = pcb_find_level_tail(level);

	if(!level_tail){
		ready_proc = pcb_insert_before(ready_proc, level_tail);
		if(!ready_proc)
			ready_proc = item;
	}
	else
		ready_proc = pcb_insert_after(item, level_tail);

	return 0;
}

/* Increase the queue level of an item in the ready queue after an timeout.
 * 
 * Arguments:
 * -pcb *item: The item to increase in queue level.
 * 
 * Results:
 * -Succes: A pointer to the item in the queue.
 * -Fail: NULL. */
int pcb_increase_level(pcb *item){
	if (!item)
		return -1;

	((pcb_admin *)item->your_admin)->queue_level ++;

	return 0;
}

/* Increase the queue level of an item in the ready queue after an timeout.
 * 
 * Arguments:
 * -pcb *item: The item to increase in queue level.
 * 
 * Results:
 * -Succes: A pointer to the item in the queue.
 * -Fail: NULL. */
int pcb_place_in_ready_queue(pcb *item){
	pcb_admin *admin;

	if (!item)
		return -1;

	admin = malloc(sizeof(pcb_admin));
	item->your_admin = (void *)admin;

	pcb_move_to_level(item, 1);

	return 0;
}

/* Set a specific level for an item.
 * 
 * Arguments:
 * -pcb *item: The item to increase in queue level.
 * 
 * Results:
 * -Succes: A pointer to the item in the queue.
 * -Fail: NULL. */
int pcb_set_queue_level(pcb *item, int level){
	if (!item)
		return -1;

	((pcb_admin *)item->your_admin)->queue_level = level;

	return 0;
}

/* Set a specific level for an item.
 * 
 * Arguments:
 * -pcb *item: The item to increase in queue level.
 * 
 * Results:
 * -Succes: A pointer to the item in the queue.
 * -Fail: NULL. */
int pcb_get_queue_level(pcb *item){
	if (!item)
		return -1;

	return ((pcb_admin *)item->your_admin)->queue_level;
}

