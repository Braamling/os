/* Authors:
 * -Bas van den Heuvel
 * -Bram van den Akker */

#include <stdio.h>
#include <stdlib.h>

#include "schedule.h"
#include "pcb_control.h"
#include "mem_alloc.h"
#include "sas.h"
#include "multilevel.h"

int give_mem = 0, new_event = 0, finished_event = 0;

/* This variable will simulate the allocatable memory. */
static long memory[MEM_SIZE];

/* The actual CPU scheduler is implemented here. */
static void CPU_scheduler() {

    /* Insert the code for a MLFbQ scheduler here. */
}

/* The high-level memory allocation scheduler is implemented here. */
static void GiveMemory() {
    long mem_need, assigned_mem_base;
    pcb *proc;

    proc = new_proc;

    if (!proc)
        return;

    mem_need = pcb_get_mem_need(proc);

    if (mem_need == -1)
        return;

    assigned_mem_base = mem_get(mem_need);
    if (assigned_mem_base == -1)
        return;

    pcb_set_mem_base(proc, assigned_mem_base);

    /* Move the pcb to the ready queue. */
    new_proc = pcb_remove(proc);

    pcb_place_in_ready_queue(proc);
    // ready_tail = pcb_find_tail(ready_proc);

    // if (!ready_tail)
    //     ready_proc = proc;
    // else
    //     ready_proc = pcb_insert_after(proc, ready_tail);

    set_slice(SLICE);

    give_mem ++;
}

/* Here we reclaim the memory of a process after it has finished. */
static void ReclaimMemory() {
    pcb *proc;
    long mem_base;

    proc = defunct_proc;

    while (proc) {
        mem_base = pcb_get_mem_base(proc);
        if (mem_base >= 0) {
            mem_free(mem_base);
            pcb_set_mem_base(proc, -1);
        }

        if (proc->your_admin) {
            free(proc->your_admin);
        }

        rm_process(&proc);

        defunct_proc = pcb_remove(proc);
        proc = defunct_proc;
    }
}

static void schedule_to_back() {
    pcb *proc;
    int level;

    // printf("yay!\n");

    proc = ready_proc;

    if (proc) {
        printf("proc: %ld\n", proc->proc_num);

        ready_proc = pcb_remove(proc);

        level = pcb_get_queue_level(proc) + 1;
        pcb_move_to_level(proc, level);

        // if (!ready_proc)
        //     ready_proc = proc;
        // else {
        //     ready_tail = pcb_find_tail(ready_proc);
        //     ready_proc = pcb_insert_after(proc, ready_tail);
        // }

        printf("timeout: %ld", proc->proc_num);

        if (ready_proc)
            printf(", new: %ld", ready_proc->proc_num);

        printf(".\n");

        set_slice(SLICE);
    }
}

/* OLD */
// static void schedule_to_back() {
//     pcb *proc, *ready_tail;

//     // printf("yay!\n");

//     proc = ready_proc;

//     if (proc) {
//         ready_proc = pcb_remove(proc);

//         if (!ready_proc)
//             ready_proc = proc;
//         else {
//             ready_tail = pcb_find_tail(ready_proc);
//             ready_proc = pcb_insert_after(proc, ready_tail);
//         }

//         printf("timeout: %ld", proc->proc_num);

//         if (ready_proc)
//             printf(", new: %ld", ready_proc->proc_num);

//         printf(".\n");

//         set_slice(SLICE);
//     }
// }

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
            new_event ++;
            GiveMemory();
            break;
        case Time_event:
            schedule_to_back();
            break;
        case IO_event:
            CPU_scheduler();
            break;
        case Ready_event:
            break;
        case Finish_event:
            finished_event ++;
            ReclaimMemory();
            GiveMemory();
            CPU_scheduler();
            break;
        default:
            printf("I cannot handle event nr. %d\n", event);
            break;
    }
}
