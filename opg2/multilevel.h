typedef struct your_admin {
    int queue_level;
} your_admin;

/* Find the tail of a level in the ready Queue.
 * 
 * Arguments:
 * -pcb *queue: The readyqeue to search in.
 * -int level: Queue level to search in.
 * 
 * Results:
 * -Succes: Return a pointer to the last pcb in de queue.
 * -Fail: NULL. */
pcb *pcb_find_level_tail(pcb *queue, int level);

/* Put an item at the end of a certain queue level.
 * 
 * Arguments:
 * -pcb *queue: The readyqueue to put the item in.
 * -pcb *item: The item to put in the readyqueue.
 * -int level: Queue level to put the item in in.
 * 
 * Results:
 * -Succes: Return a pointer to the last pcb in de queue.
 * -Fail: NULL. */
pcb *pcb_move_to_level(pcb *item, int level);

/* Increase the queue level of an item in the ready queue after an timeout.
 * 
 * Arguments:
 * -pcb *item: The item to increase in queue level.
 * 
 * Results:
 * -Succes: Return a pointer to the item in the queue.
 * -Fail: NULL. */
pcb *pcb_increase_level(pcb *item);

pcb *pcb_place_in_ready_queue(pcb *item);

/* Set a specific level for an item.
 * 
 * Arguments:
 * -pcb *item: The item to increase in queue level.
 * 
 * Results:
 * -Succes: A pointer to the item in the queue.
 * -Fail: NULL. */
pcb *pcb_set_queue_level(pcb *item, int level);

/* Set a specific level for an item.
 * 
 * Arguments:
 * -pcb *item: The item to increase in queue level.
 * 
 * Results:
 * -Succes: A pointer to the item in the queue.
 * -Fail: NULL. */
int pcb_get_queue_level(pcb *item);
