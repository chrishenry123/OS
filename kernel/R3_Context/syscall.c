#include "context.h"
#include "pcb.h"
#include <stddef.h>

#define IDLE 1
#define EXIT 0

// Global variables
struct pcb *current_pcb = NULL;
static struct context *initial_context = NULL;

// Function prototypes
static void save_context(struct context *ctx);
static struct pcb* select_next_process(void);
static int sys_req_idle(void);
static void terminate_and_free_all_pcbs(struct pcb **queue);
struct context *sys_call(struct context *ctx);

// System call implementation
struct context *sys_call(struct context *ctx) {
    switch (ctx->eax) {
        case IDLE:
            if (initial_context == NULL) {
                initial_context = ctx;
            }
            if (current_pcb != NULL) {
                save_context(ctx);
                current_pcb->exec_state = READY;
                pcb_insert(current_pcb);
            }
            break;

        case EXIT:
            terminate_and_free_all_pcbs(&ReadyQueue);
            terminate_and_free_all_pcbs(&BlockedQueue);

            if (current_pcb) {
                pcb_free(current_pcb);
                current_pcb = NULL;
            }
            break;

        default:
            ctx->eax = -1;
            return ctx;
    }

    if (sys_req_idle() == 0) {
        struct pcb *next_pcb = select_next_process();
        current_pcb = next_pcb;
        current_pcb->exec_state = READY;
        return (struct context *) current_pcb->stack_pointer;
    } else {
        ctx = initial_context;
        initial_context = NULL;
        return ctx;
    }
}

// Saves the context by updating the stack pointer
static void save_context(struct context *ctx) {
    if (current_pcb) {
        current_pcb->stack_pointer = ctx;
    }
}

// Checks if there are any ready, non-suspended PCBs in the queue
static int sys_req_idle(void) {
    struct pcb *current = ReadyQueue;
    while (current != NULL) {
        if (current->exec_state == READY && current->disp_state == NOT_SUSPENDED) {
            return 0;
        }
        current = current->next;
    }
    return -1;
}

// Selects the next process if there are any ready, non-suspended PCBs in the queue
static struct pcb* select_next_process(void) {
    if (ReadyQueue && ReadyQueue->exec_state == READY && ReadyQueue->disp_state == NOT_SUSPENDED) {
        struct pcb *selected_pcb = ReadyQueue;
        pcb_remove(ReadyQueue);
        return selected_pcb;
    }
    return NULL;
}

// Terminates and frees all PCBs in a given queue
static void terminate_and_free_all_pcbs(struct pcb **queue) {
    struct pcb *current;
    while ((current = *queue) != NULL) {
        *queue = current->next;
        pcb_free(current);
    }
}
