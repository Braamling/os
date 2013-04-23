/* Authors:
 * -Bas van den Heuvel
 * -Bram van den Akker */

#include <stdio.h>
#include <stdlib.h>

#include "schedule.h"
#include "pcb_control.h"
#include "mem_alloc.h"

/* This variable will simulate the allocatable memory. */
static long memory[MEM_SIZE];

/* The actual CPU scheduler is implemented here. */
static void CPU_scheduler() {

    /* Insert the code for a MLFbQ scheduler here. */
}

/* The high-level memory allocation scheduler is implemented here. */
static void GiveMemory() {
    
}

/* Here we reclaim the memory of a process after it has finished. */
static void ReclaimMemory() {
    
}

/* You may want to have the last word... */
static void my_finale() {
    
}

/* The main scheduling routine. */
void schedule(event_type event) {
    static int first = 1;

    if (first) {
        mem_init(memory);
        finale = my_finale;
        first = 0;

        /* Add your own initialisation code here. */
    }

    switch (event) {

        /* You may want to do this differently. */
        case NewProcess_event:
            GiveMemory();
            break;
        case Time_event:
            break;
        case IO_event:
            CPU_scheduler();
            break;
        case Ready_event:
            break;
        case Finish_event:
            ReclaimMemory();
            GiveMemory();
            CPU_scheduler();
            break;
        default:
            printf("I cannot handle event nr. %d\n", event);
            break;
    }
}
