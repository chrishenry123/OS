#include "context.h"
#include "pcb.h"
#include <stddef.h>

#define IDLE 1
#define EXIT 0

// global pointer representing the currently executing process
struct pcb *current_pcb = NULL;
// static context pointer representing the context from the first time syscall() is entered
// changing stuff to stack pointer
static struct context *initial_context = NULL;

// Function prototypes
static void save_context(struct context *ctx);
static struct pcb* select_next_process(void);
static struct context* return_context(struct pcb *pcb);
static int sys_req_idle(void);

// system call implementation
// takes one param - the context of the current process (pointer)
struct context *sys_call(struct context *ctx) {
    // checks gen purpose register of the context containing operation code
    switch(ctx->eax) {
        // case for idle
        case IDLE:
            // if the initial context is null set it to point to the context provided
            if (initial_context == NULL){
                initial_context = ctx;
            }
            // checks if there are any ready, non-suspended PCBs in the queue
            if (sys_req_idle() == 0) {
                // removes the 1st ready non suspended pcb from the q and stores it in temp variable
                struct pcb *next_pcb = select_next_process();

                // updates the stack pointer
                save_context(ctx);

                if (next_pcb) {
                    current_pcb = next_pcb;
                    return return_context(next_pcb);
                }
                else {
                    return ctx;
                }
            }
            break;

        case EXIT:
            if (current_pcb) {
                pcb_remove(current_pcb);
                pcb_free(current_pcb);
            }

            // checks if there are any ready, non-suspended PCBs in the queue
            // returns the selected pcb from select-next-process
            struct pcb *next_pcb = select_next_process();
            if (next_pcb) {
                current_pcb = next_pcb;

                // returns the context of the selected pcb
                return return_context(next_pcb);
            }
                // loads the original context if pcb q is empty
            else {
                return initial_context;
            }
            break;

            //if the op code is not idle or exit set return value to -1
        default:
            ctx->eax = -1;
            return ctx;
    }

    ctx->eax = 0;
    return ctx;
}

static int sys_req_idle(void) {
    struct pcb *current = ReadyQueue;
    // search fot pcb that is ready and not suspended
    while (current) {
        if (current->exec_state == READY && current->disp_state == NOT_SUSPENDED) {
            return 0;
        }
        current = current->next;
    }

    return -1;
}

static struct pcb* select_next_process(void) {
    // variable to store selected process
    struct pcb *selected_pcb = NULL;
    // makes sure ReadyQueue is not empty, execution state is ready and the dispatch state is not suspended
    if (ReadyQueue && ReadyQueue->exec_state == READY && ReadyQueue->disp_state == NOT_SUSPENDED) {
        selected_pcb = ReadyQueue;
        pcb_remove(ReadyQueue);
    }
    return selected_pcb;
}
// returns the context
static struct context* return_context(struct pcb *pcb) {
    if (!pcb) {
        return NULL;
    }
    return pcb->stack_pointer;
}

// updates stack pointer
static void save_context(struct context *ctx) {
    if (current_pcb) {
        current_pcb->stack_pointer = ctx;
    }
}
