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
    if (!initial_context) {
        initial_context = ctx;
    }

    switch(ctx->eax) {
        case IDLE:
            if (sys_req_idle() == 0) {
                save_context(ctx);
                struct pcb *next_pcb = select_next_process();
                if (next_pcb) {
                    current_pcb = next_pcb;
                    return restore_context(next_pcb);
                }
            }
            break;

        case EXIT:
            pcb_remove(current_pcb);
            pcb_free(current_pcb);
            struct pcb *next_pcb = select_next_process();
            if (next_pcb) {
                current_pcb = next_pcb;
                return restore_context(next_pcb);
            } else {
                return initial_context;
            }
            break;

        default:
            ctx->eax = -1;
            return ctx;
    }

    ctx->eax = 0;
    return ctx;
}

static int sys_req_idle(void) {
    struct pcb *current = ReadyQueue;
    while (current) {
        if (current->exec_state == READY && current->disp_state == NOT_SUSPENDED) {
            return 0;
        }
        current = current->next;
    }
    return -1;
}

static struct pcb* select_next_process(void) {
    struct pcb *selected_pcb = NULL;
    if (ReadyQueue && ReadyQueue->exec_state == READY && ReadyQueue->disp_state == NOT_SUSPENDED) {
        selected_pcb = ReadyQueue;
        pcb_remove(ReadyQueue);
    }
    return selected_pcb;
}

static struct context* restore_context(struct pcb *pcb) {
    if (!pcb) {
        return NULL;
    }
    return pcb->context;
}

static void save_context(struct context *ctx) {
    if (current_pcb) {
        current_pcb->context = ctx;
    }
}
