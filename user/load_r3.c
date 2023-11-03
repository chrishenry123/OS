//
// Created by Dylan Caldwell on 10/13/23.
//
#include "load_r3.h"
#include "processes.h"
#include "pcb.h"
#include "context.h"
#include <string.h>
#include <stdint.h>
#include <sys_req.h>

#define COM1 0x3F8

const char* process_names[] = {"Process 1", "Process 2", "Process 3", "Process 4", "Process 5"};
const int priorities[] = {1, 2, 3, 4, 5};

void (*process_funcs[])(void) = {proc1, proc2, proc3, proc4, proc5};

void load_r3(void) {
    for (int i = 0; i < 5; i++) {
        struct pcb *new_pcb = pcb_allocate();
        if (new_pcb == NULL) {
            char error_msg[] = "\033[0;31mError: Failed to allocate PCB for R3 process.\n";
            sys_req(WRITE, COM1, error_msg, strlen(error_msg));
            return;
        }

        // Set PCB fields
        strncpy(new_pcb->name, process_names[i], sizeof(new_pcb->name));  // Corrected the assignment of name
        new_pcb->priority = priorities[i];
        new_pcb->exec_state = READY;
        new_pcb->disp_state = NOT_SUSPENDED;
        //
        struct context* new_context = (struct context*)new_pcb->stack_pointer;
        // Initialize the context
        new_context->cs = 0x08;
        new_context->ds = 0x10;
        new_context->es = 0x10;
        new_context->fs = 0x10;
        new_context->gs = 0x10;
        new_context->ss = 0x10;
        // Adjusted these lines to use the correct fields
        new_context->esp = (uint32_t)(new_pcb->stack_pointer);
        new_context->ebp = (uint32_t)(new_pcb->stack_pointer);
        new_context->eip = (uint32_t)process_funcs[i];
        new_context->eflags = 0x0202;
        new_context->eax = 0;
        new_context->ebx = 0;
        new_context->ecx = 0;
        new_context->edx = 0;
        new_context->esi = 0;
        new_context->edi = 0;

        // Add the new PCB to the queue
        pcb_insert(new_pcb);
    }
    char success_msg[] = "\nAll R3 processes have been successfully loaded.\n";
    sys_req(WRITE, COM1, success_msg, strlen(success_msg));
}
