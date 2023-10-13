//
// Created by Dylan Caldwell on 10/13/23.
//
#include "load_r3.h"
#include "processes.h"
#include "pcb.h"
#include "context.h"
#include <string.h>
#include <stdint.h>

// You can customize these names and priorities
const char* process_names[] = {"Process 1", "Process 2", "Process 3", "Process 4", "Process 5"};
const int priorities[] = {1, 2, 3, 4, 5};

void (*process_funcs[])(void) = {proc1, proc2, proc3, proc4, proc5};  // Corrected function pointer declaration

void load_r3(void) {
    for (int i = 0; i < 5; i++) {
        struct pcb *new_pcb = pcb_allocate();
        if (new_pcb == NULL) {
            // Handle allocation failure, maybe print an error and return
            return;
        }

        // Set PCB fields
        strncpy(new_pcb->name, process_names[i], sizeof(new_pcb->name));  // Corrected the assignment of name
        new_pcb->priority = priorities[i];
        new_pcb->exec_state = READY;
        new_pcb->disp_state = NOT_SUSPENDED;

        // Initialize the context
        new_pcb->context->cs = 0x08;
        new_pcb->context->ds = 0x10;
        new_pcb->context->es = 0x10;
        new_pcb->context->fs = 0x10;
        new_pcb->context->gs = 0x10;
        // Adjusted these lines to use the correct fields
        new_pcb->context->ebp = (uint32_t)(new_pcb->stack_pointer);
        new_pcb->context->esp = (uint32_t)(new_pcb->stack_pointer);
        new_pcb->context->eip = (uint32_t)process_funcs[i];
        new_pcb->context->eflags = 0x0202;
        new_pcb->context->eax = 0;
        new_pcb->context->ebx = 0;
        new_pcb->context->ecx = 0;
        new_pcb->context->edx = 0;
        new_pcb->context->esi = 0;
        new_pcb->context->edi = 0;

        // Add the new PCB to the queue
        pcb_insert(new_pcb);
    }
}
