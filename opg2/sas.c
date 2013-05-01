/* Authors:
 * -Bas van den Heuvel
 * -Bram van den Akker */

#include <stdio.h>
#include <stdlib.h>

#include "schedule.h"
#include "pcb_control.h"
#include "sas.h"
#include "mem_alloc.h"
#include "multilevel.h"

int give_mem = 0, new_event = 0, finished_event = 0;
double slice;
scheduler_type scheduler;

/* This variable will simulate the allocatable memory. */
static long memory[MEM_SIZE];

static void CPU_scheduler(){
    if(scheduler == ROUND_ROBIN)
        round_robin();
    else
        multi_level_scheduler();

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

    set_slice(slice);

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

        if (proc->your_admin)
            pcb_admin_destroy(proc->your_admin);

        rm_process(&proc);

        defunct_proc = pcb_remove(proc);
        proc = defunct_proc;
    }
}

/* Every proces that uses the cpu for longer then time slice will be place to 
 * in a higher level queue. Only when all the lower queues are empty the higher
 * queue will get cpu time. 
 */
static void multi_level_scheduler() {
    pcb *proc;
    int level;

    proc = ready_proc;

    if (proc) {
        ready_proc = pcb_remove(proc);

        level = pcb_get_queue_level(proc) + 1;
        pcb_move_to_level(proc, level);

        set_slice(slice);
    }
}

/* Every proces that uses the cpu for longer then time slice will be place to 
 * in the back of the queue. 
 */
static void round_robin() {
    pcb *proc, *ready_tail;

    proc = ready_proc;

    if (proc) {
        ready_proc = pcb_remove(proc);

        if (!ready_proc)
            ready_proc = proc;
        else {
            ready_tail = pcb_find_tail(ready_proc);
            ready_proc = pcb_insert_after(proc, ready_tail);
        }

        printf("timeout: %ld", proc->proc_num);

        if (ready_proc)
            printf(", new: %ld", ready_proc->proc_num);

        printf(".\n");

        set_slice(slice);
    }
}

static void my_finale() {
    /* Your very own code goes here */
}

/* The main scheduling routine. */
void schedule(event_type event) {
    static int first = 1;
    int type;

    if (first) {
        mem_init(memory);
        finale = my_finale;
        first = 0;

        printf("Chose a scheduler type [0-round robin/1-multi/default]:");
        scanf("\n %d", &type);
        if (type == 0){
            scheduler = ROUND_ROBIN;
        }else{
            scheduler = MULTI_LEVEL;
        }

        printf("Chose time slide:");
        scanf("\n %lf", &slice);
    }

    switch (event) {

        /* You may want to do this differently. */
        case NewProcess_event:
            new_event ++;
            GiveMemory();
            break;
        case Time_event:
            CPU_scheduler();
            break;
        case IO_event:
            break;
        case Ready_event:
            break;
        case Finish_event:
            finished_event ++;
            ReclaimMemory();
            GiveMemory();
            break;
        default:
            printf("I cannot handle event nr. %d\n", event);
            break;
    }
}
