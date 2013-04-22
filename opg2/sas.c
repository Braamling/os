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
    long MEM_base;
    pcb *proc, *ready_proc_last;

    proc = pcb_get_first(new_proc);
    while (proc != NULL) {
        MEM_base = mem_get(pcb_get_mem_need(proc));

        if (MEM_base >= 0) {
            pcb_set_mem_base(proc, MEM_base);

            ready_proc_last = pcb_get_last(ready_proc);
            if (ready_proc_last != NULL)
                pcb_move_before(proc, ready_proc_last);
            else {
                pcb_remove(proc);
                ready_proc = proc;
            }

            proc = pcb_get_next(proc);
        }
    }

    // new_proc = NULL;
    // ready_proc = pcb_get_first(proc);
}

/* Here we reclaim the memory of a process after it has finished. */
static void ReclaimMemory() {
    pcb *proc;

    proc = defunct_proc;
    while (proc) {

        /* Free your own administrative structure if it exists. */
        if (proc->your_admin) {
            free(proc->your_admin);
        }

        /* Free the simulated allocated memory. */
        mem_free(proc->MEM_base);
        proc->MEM_base = -1;

        /* Call the function that cleans up the simulated process. */
        rm_process(&proc);

        /* See if there are more processes to be removed. */
        proc = defunct_proc;
    }
}

/* You may want to have the last word... */
static void my_finale() {
    
    /* Your very own code goes here. */
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
