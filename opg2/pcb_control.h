/* Authors:
 * -Bas van den Heuvel
 * -Bram van den Akker */

/* Get the next item in a list of PCB's.
 *
 * Arguments:
 * -pcb *item: The item to get the next item from.
 *
 * Results:
 * -Success: The next item relative to the item.
 * -Failure: NULL. */
pcb *pcb_get_next(pcb *item);

/* Get the previous item in a list of PCB's.
 *
 * Arguments:
 * -pcb *item: The item to get the previous item from.
 *
 * Results:
 * -Success: The previous item relative to the item.
 * -Failure: NULL. */
pcb *pcb_get_prev(pcb *item);

/* Get the amount of MEM the PCB needs.
 *
 * Arguments:
 * -pcb *item: The item to get the memory needed from.
 *
 * Results:
 * -Success: The memory the item needs.
 * -Failure: -1. */
long pcb_get_mem_need(pcb *item);

/* Set the base of MEM for the PCB.
 *
 * Arguments:
 * -pcb *item: The item to set the memory base to.
 *
 * Results:
 * -Success: 0.
 * -Failure: -1. */
int pcb_set_mem_base(pcb *item, long MEM_base);

/* Find the head of a list.
 *
 * Arguments:
 * -pcb *item: The item in the list to look through.
 *
 * Results:
 * -Success: The head of the list.
 * -Failure: NULL. */
pcb *pcb_find_head(pcb *item);

/* Find the tail of a list.
 *
 * Arguments:
 * -pcb *item: The item in the list to look through.
 *
 * Results:
 * -Success: The tail of the list.
 * -Failure: NULL. */
pcb *pcb_find_tail(pcb *item);

/* Insert an item into a list of PCB's before another one.
 *
 * Arguments:
 * -pcb *item: The item to insert into the list.
 * -pcb *before: The before which the item will be inserted.
 *
 * Results:
 * -Success: The head of the list.
 * -Failure: NULL. */
pcb *pcb_insert_before(pcb *item, pcb *before);

/* Insert an item into a list of PCB's after another one.
 *
 * Arguments:
 * -pcb *item: The item to insert into the list.
 * -pcb *after: The after which the item will be inserted.
 *
 * Results:
 * -Success: The head of the list.
 * -Failure: NULL. */
pcb *pcb_insert_after(pcb *item, pcb *after);

/* Remove an item from a list of PCB's.
 *
 * Arguments:
 * -pcb *item: The item in the list.
 *
 * Results:
 * -Succes: The head of the list.
 * -Failure: NULL. */
pcb *pcb_remove(pcb *item);
