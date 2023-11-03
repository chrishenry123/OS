#include "context.h"
#include "pcb.h"
#include <stddef.h>

#define IDLE 0
#define EXIT 1

// Global variables
static struct pcb *current_pcb = NULL;
static struct context *initial_context = NULL;

// Function prototypes
static void save_context(struct context *ctx);
static struct pcb* select_next_process(void);
static struct context* restore_context(struct pcb *pcb);
static int sys_req_idle(void);

// System call implementation
struct context *sys_call(struct context *ctx) {
    if (!ctx) {
        // Handle the case where the context passed in is NULL
        return NULL;
    }

    // Save the initial context if this is the first syscall
    if (!initial_context) {
        initial_context = ctx;
    }

    switch (ctx->eax) {
        case IDLE:
            // IDLE syscall handling
            if (sys_req_idle() == 0) {
                save_context(ctx); // Save the current context
                struct pcb *next_pcb = select_next_process();
                if (next_pcb) {
                    current_pcb = next_pcb; // Switch to the next process
                    return restore_context(next_pcb); // Restore the context of the next process
                }
            }
            ctx->eax = 0; // Return value seen by sys_req() is 0
            return ctx; // Continue with the current context if no process is ready

        case EXIT:
            // EXIT syscall handling
            if (current_pcb) {
                // Remove the current process and free its resources
                pcb_remove(current_pcb);
                pcb_free(current_pcb);
                current_pcb = NULL; // Clear the current PCB
            }

            struct pcb *next_pcb = select_next_process();
            if (next_pcb) {
                current_pcb = next_pcb; // Update the current process
                return restore_context(next_pcb); // Restore the context of the next process
            } else {
                // If there are no more processes, return to the initial context
                ctx->eax = 0; // Return value seen by sys_req() is 0
                return initial_context;
            }

        default:
            // Unsupported system call operation
            ctx->eax = -1; // Set an error code
            return ctx; // Return the same context to indicate an error
    }
}

static int sys_req_idle(void) {
    struct pcb *current = ReadyQueue;
    while (current) {
        if (current->exec_state == READY && current->disp_state == NOT_SUSPENDED) {
            return 0; // Found a process that is ready and not suspended
        }
        current = current->next;
    }
    return -1; // No ready and non-suspended processes
}

static struct pcb* select_next_process(void) {
    struct pcb *selected_pcb = NULL;
    if (ReadyQueue && ReadyQueue->exec_state == READY && ReadyQueue->disp_state == NOT_SUSPENDED) {
        selected_pcb = ReadyQueue;
        pcb_remove(ReadyQueue); // Assume pcb_remove() removes the PCB from the queue
    }
    return selected_pcb;
}

static struct context* restore_context(struct pcb *pcb) {
    if (!pcb) {
        return NULL;
    }
    return pcb->stack_pointer; // Assume pcb->stack_pointer stores the context
}

static void save_context(struct context *ctx) {
    if (current_pcb) {
        current_pcb->stack_pointer = ctx; // Save the current context to the PCB
    }
}
