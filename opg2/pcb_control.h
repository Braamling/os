/* Authors:
 * -Bas van den Heuvel
 * -Bram van den Akker */

/* Insert a PCB before another one into a list of PCB's. */
int pcb_insert_before(pcb *item, pcb *next_item);

/* Insert a PCB after another one into a list of PCB's. */
int pcb_insert_after(pcb *item, pcb *prev_item);

/* Remove a PCB from a list of PCB's. */
int pcb_remove(pcb *item);

/* Move a PCB to a new position before another one in a list of PCB's. */
int pcb_move_before(pcb *item, pcb *next_item);

/* Move a PCB to a new position after another one in a list of PCB's. */
int pcb_move_after(pcb *item, pcb *prev_item);

/* Get the next item in a list of PCB's. */
pcb *pcb_get_next(pcb *item);

/* Get the previous item in a list of PCB's. */
pcb *pcb_get_prev(pcb *item);

/* Get the amount of MEM the PCB needs. */
long pcb_get_mem_need(pcb *item);

/* Set the base of the PCB's MEM. */
int pcb_set_mem_base(pcb *item, long MEM_base);
