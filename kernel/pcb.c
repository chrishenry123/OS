#include "pcb.h"
#include "memory.h"  // Assume this header has sys_alloc_mem and sys_free_mem
#include <string.h>


struct pcb *ReadyQueue = NULL;
struct pcb *BlockedQueue = NULL;

// Allocates memory for a new PCB
struct pcb* pcb_allocate(void) {
    struct pcb *new_pcb = (struct pcb*) sys_alloc_mem(sizeof(struct pcb));
    if (new_pcb == NULL) return NULL;

    new_pcb->stack = sys_alloc_mem(1024);
    if (new_pcb->stack == NULL) {
        sys_free_mem(new_pcb);
        return NULL;
    }

    memset(new_pcb->stack, 0, 1024);
    new_pcb->stack_pointer = (void*)((char*)new_pcb->stack + 1020); // Enough to hold a void *
    return new_pcb;
}

// Frees memory associated with a PCB
int pcb_free(struct pcb *pcb_to_free) {
    if (pcb_to_free == NULL) return -1;
    sys_free_mem(pcb_to_free->stack);
    sys_free_mem(pcb_to_free);
    return 0;
}

// Sets up a new PCB with initial values
struct pcb* pcb_setup(const char *name, int class, int priority) {
    struct pcb *new_pcb = pcb_allocate();
    if (new_pcb == NULL || name == NULL || strlen(name) < 1 || strlen(name) > 15 || priority < 0 || priority > 9) {
        return NULL;
    }
    size_t i;
    for (i = 0; i < 15 && name[i] != '\0'; ++i) {
        new_pcb->name[i] = name[i];
    }
    new_pcb->name[i] = '\0';  // Null-terminate the copied string

    new_pcb->class = class;
    new_pcb->priority = priority;
    new_pcb->exec_state = READY;
    new_pcb->disp_state = NOT_SUSPENDED;

    return new_pcb;
}

// Find PCB by name
struct pcb* pcb_find(const char *name) {
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
    return NULL;
}

void pcb_insert(struct pcb* inserted) {
    if (!inserted) return;  // Guard against null input

    struct pcb **queue;
    struct pcb *current;
    struct pcb *prev = NULL;

    if (inserted->exec_state == BLOCKED) {
        queue = &BlockedQueue;
        // Simply push to the front of the BlockedQueue.
        inserted->next = *queue;
        *queue = inserted;
    } else if (inserted->exec_state == READY) {
        queue = &ReadyQueue;
        current = *queue;
        // Find the correct position based on priority.
        while (current && current->priority <= inserted->priority) {
            prev = current;
            current = current->next;
        }
        // Insert the new PCB.
        if (prev) {
            prev->next = inserted;
        } else {
            // If prev is NULL, this means the inserted PCB has higher
            // priority than all other PCBs, so it should be at the front.
            *queue = inserted;
        }
        inserted->next = current;
    }
}


// Removes a PCB from its current queue
int pcb_remove(struct pcb *target) {
    if (target == NULL) return -1;

    struct pcb **queue;

    if (target->exec_state == READY) {
        queue = &ReadyQueue;
    } else if (target->exec_state == BLOCKED) {
        queue = &BlockedQueue;
    } else {
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

    return -1;  // Target not found
}
