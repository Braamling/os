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
pcb *pcb_find_level_tail(pcb *queue, int level){
	pcb *item;

	if (!queue)
		return NULL;

	item = queue;

	if(pcb_get_queue_level(item) > level)
		return NULL;

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
pcb *pcb_move_to_level(pcb *item, int level){
	pcb* tail;

	if (!item)
		return NULL;

	if(level < 0)
		return NULL;
	

	tail = pcb_find_level_tail(ready_proc, level);

	if(!tail){
		tail = pcb_insert_before(ready_proc, tail);
		if(!tail)
			ready_proc = tail;
	}

	if(tail)
		pcb_insert_after(item, tail);

	return item;
}

/* Increase the queue level of an item in the ready queue after an timeout.
 * 
 * Arguments:
 * -pcb *item: The item to increase in queue level.
 * 
 * Results:
 * -Succes: A pointer to the item in the queue.
 * -Fail: NULL. */
pcb *pcb_increase_level(pcb *item){
	if (!item)
		return NULL;

	((struct your_admin *)item->your_admin)->queue_level ++;

	return item;
}

/* Increase the queue level of an item in the ready queue after an timeout.
 * 
 * Arguments:
 * -pcb *item: The item to increase in queue level.
 * 
 * Results:
 * -Succes: A pointer to the item in the queue.
 * -Fail: NULL. */
pcb *pcb_place_in_ready_queue(pcb *item){
	if (!item)
		return NULL;

	pcb_move_to_level(item, 1);

	return item;
}

/* Set a specific level for an item.
 * 
 * Arguments:
 * -pcb *item: The item to increase in queue level.
 * 
 * Results:
 * -Succes: A pointer to the item in the queue.
 * -Fail: NULL. */
pcb *pcb_set_queue_level(pcb *item, int level){
	if (!item)
		return NULL;

	((struct your_admin *)item->your_admin)->queue_level = level;

	return item;
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
		return 0;

	return ((struct your_admin *)item->your_admin)->queue_level;
}

