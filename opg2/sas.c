/* Authors:
 * -Bas van den Heuvel
 * -Bram van den Akker */

#include <stdio.h>
#include <stdlib.h>

#include "schedule.h"
#include "pcb_control.h"
#include "mem_alloc.h"
#include "sas.h"

/* This variable will simulate the allocatable memory. */
static long memory[MEM_SIZE];

/* The actual CPU scheduler is implemented here. */
static void CPU_scheduler() {

    /* Insert the code for a MLFbQ scheduler here. */
}

static void move_to_mem_queue(pcb *proc) {
    pcb *mem_wait_tail;

    new_proc = pcb_remove(proc);

    if (!mem_wait_proc)
        mem_wait_proc = proc;
    else {
        mem_wait_tail = pcb_find_tail(mem_wait_proc);
        mem_wait_proc = pcb_insert_after(proc, mem_wait_tail);
    }
}

static void insert_mem_in_new_queue() {
    pcb *new_head, *mem_wait_tail;

    new_head = pcb_find_head(new_proc);

    new_proc = mem_wait_proc;
    mem_wait_proc = NULL;

    if (!new_proc)
        new_proc = new_head;
    else {
        mem_wait_tail = pcb_find_tail(new_proc);
        new_proc = pcb_insert_after(new_head, mem_wait_tail);
    }
}

/* The high-level memory allocation scheduler is implemented here. */
static void GiveMemory() {
    long mem_need, assigned_mem_base;
    pcb *proc, *ready_tail;

    proc = new_proc;

    while (proc) {
        mem_need = pcb_get_mem_need(proc);

        if (mem_need == -1) {
            move_to_mem_queue(proc);
            proc = new_proc;
            continue;
        }

        assigned_mem_base = mem_get(mem_need);
        if (assigned_mem_base == -1) {
            printf("new_proc before move: %ld.\n", new_proc->proc_num);
            move_to_mem_queue(proc);
            if (new_proc)
                printf("new_proc after move: %ld.\n", new_proc->proc_num);
            proc = new_proc;
            continue;
        }

        pcb_set_mem_base(proc, assigned_mem_base);

        /* Move the pcb to the ready queue. */
        new_proc = pcb_remove(proc);
        ready_tail = pcb_find_tail(ready_proc);

        if (!ready_tail)
            ready_proc = proc;
        else
            ready_proc = pcb_insert_after(proc, ready_tail);

        proc = new_proc;
    }

    insert_mem_in_new_queue();
}

/* Here we reclaim the memory of a process after it has finished. */
static void ReclaimMemory() {
    pcb *proc;
    long mem_base;

    proc = defunct_proc;

    while (proc) {
        printf("proc %ld finished.\n", proc->proc_num);

        mem_base = pcb_get_mem_base(proc);
        if (mem_base >= 0) {
            mem_free(mem_base);
            pcb_set_mem_base(proc, -1);
        }

        if (proc->your_admin) {
            /* Release admin. */
        }

        rm_process(&proc);

        defunct_proc = pcb_remove(proc);
        proc = defunct_proc;
    }
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
