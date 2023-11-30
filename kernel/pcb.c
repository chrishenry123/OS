#include "pcb.h"
#include "memory.h"
#include <context.h>
#include <string.h>
#include <sys_req.h>

#define COM1 0x3F8


struct pcb *ReadyQueue = NULL;
struct pcb *BlockedQueue = NULL;

// Utility function to write detailed errors
void detailed_error(const char *message, const char *variable_name, int value) {
    char buffer[512];
    snprintf(buffer, sizeof(buffer), "%s %s = %d\n", message, variable_name ? variable_name : "", value);
    sys_req(WRITE, COM1, buffer, strlen(buffer));
}

struct pcb* pcb_allocate(void) {
    struct pcb *new_pcb = (struct pcb*) sys_alloc_mem(sizeof(struct pcb));
    if (new_pcb == NULL) {
        detailed_error("Error: Failed to allocate memory for new PCB.", NULL, 0);
        return NULL;
    }

    memset(new_pcb->stack, 0, 1024);
    new_pcb->stack_pointer = (void*)(new_pcb->stack + 1020 - sizeof(struct context));
    return new_pcb;
}

int pcb_free(struct pcb *pcb_to_free) {

    if (pcb_to_free == NULL) {
        detailed_error("Error: Attempted to free a NULL PCB.", NULL, 0);
        return -1;
    }

    sys_free_mem(pcb_to_free->stack);
    sys_free_mem(pcb_to_free);
    return 0;
}

struct pcb* pcb_setup(const char *name, int class, int priority) {
    if (name == NULL || strlen(name) < 1 || strlen(name) > 15 || priority < 0 || priority > 9) {
        detailed_error("Error: Invalid PCB setup parameters.", "Priority", priority);
        return NULL;
    }

    struct pcb *new_pcb = pcb_allocate();
    if (new_pcb == NULL) {
        return NULL;
    }

    strncpy(new_pcb->name, name, 15);
    new_pcb->name[15] = '\0';
    new_pcb->class = class;
    new_pcb->priority = priority;
    new_pcb->exec_state = READY;
    new_pcb->disp_state = NOT_SUSPENDED;
    new_pcb->stack_pointer = &new_pcb->stack[1020] - sizeof(struct context);
    new_pcb->stack_pointer = (struct context *)new_pcb->stack_pointer;
    memset(new_pcb->stack_pointer, 0, sizeof(struct context));

    return new_pcb;
}

struct pcb* pcb_find(const char *name) {
    if (!name) {
        detailed_error("Error: Attempted to find PCB with NULL name.", NULL, 0);
        return NULL;
    }

    struct pcb *current = ReadyQueue;
    while (current) {
        if (strcmp(current->name, name) == 0) return current;
        current = current->next;
    }

    current = BlockedQueue;
    while (current) {
        if (strcmp(current->name, name) == 0) return current;
        current = current->next;
    }

    detailed_error("Error: PCB not found in any queue. Searched for:", "Name", (int) *name);
    return NULL;
}

void pcb_insert(struct pcb* inserted) {
    if (!inserted) {
        detailed_error("Error: Attempted to insert NULL PCB into queue.", NULL, 0);
        return;
    }

    struct pcb **queue;
    struct pcb *current;
    struct pcb *prev = NULL;

    if (inserted->exec_state == BLOCKED) {
        queue = &BlockedQueue;
        inserted->next = *queue;
        *queue = inserted;
    } else if (inserted->exec_state == READY) {
        queue = &ReadyQueue;
        current = *queue;
        while (current && current->priority <= inserted->priority) {
            prev = current;
            current = current->next;
        }
        if (prev) {
            prev->next = inserted;
        } else {
            *queue = inserted;
        }
        inserted->next = current;
    } else {
        detailed_error("Error: PCB has invalid exec_state for insertion.", "Exec_state", inserted->exec_state);
    }
}

int pcb_remove(struct pcb *target) {
    if (!target) {
        detailed_error("Error: Attempted to remove NULL PCB from queue.", NULL, 0);
        return -1;
    }

    struct pcb **queue;

    if (target->exec_state == READY) {
        queue = &ReadyQueue;
    } else if (target->exec_state == BLOCKED) {
        queue = &BlockedQueue;
    } else {
        detailed_error("Error: PCB has invalid exec_state for removal.", "Exec_state", target->exec_state);
        return -1;
    }

    struct pcb *prev = NULL;
    struct pcb *current = *queue;

    while (current) {
        if (current == target) {
            if (prev) {
                prev->next = current->next;
            } else {
                *queue = current->next;
            }
            return 0;
        }
        prev = current;
        current = current->next;
    }

    detailed_error("Error: Target PCB not found in its respective queue.", "Target Name", (int) *target->name);
    return -1;
}
