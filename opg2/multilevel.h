/* Authors:
 * -Bas van den Heuvel
 * -Bram van den Akker */

typedef struct pcb_admin {
    int queue_level;
} pcb_admin;

/* Find the tail of a level in the ready Queue.
 * 
 * Arguments:
 * -pcb *queue: The readyqeue to search in.
 * -int level: Queue level to search in.
 * 
 * Results:
 * -Succes: A pointer to the last pcb in de queue.
 * -Fail: NULL. */
pcb *pcb_find_level_tail(int level);

/* Put an item at the end of a certain queue level.
 * 
 * Arguments:
 * -pcb *queue: The readyqueue to put the item in.
 * -pcb *item: The item to put in the readyqueue.
 * -int level: Queue level to put the item in in.
 * 
 * Results:
 * -Succes: 0.
 * -Fail: -1. */
int pcb_move_to_level(pcb *item, int level);

/* Increase the queue level of an item in the ready queue after an timeout.
 * 
 * Arguments:
 * -pcb *item: The item to increase in queue level.
 * 
 * Results:
 * -Succes: 0.
 * -Fail: -1. */
int pcb_increase_level(pcb *item);

/* Places an item in de ready queue on level 1.
 * 
 * Arguments:
 * -pcb *item: The item to increase in queue level.
 * 
 * Results:
 * -Succes: 0.
 * -Fail: -1. */
int pcb_place_in_ready_queue(pcb *item);

/* Set a specific level for an item.
 * 
 * Arguments:
 * -pcb *item: The item to increase in queue level.
 * 
 * Results:
 * -Succes: 0.
 * -Fail: -1. */
int pcb_set_queue_level(pcb *item, int level);

/* Get the level of a specific pcb item.
 * 
 * Arguments:
 * -pcb *item: The item to increase in queue level.
 * 
 * Results:
 * -Succes: The queue level of the item.
 * -Fail: -1. */
int pcb_get_queue_level(pcb *item);