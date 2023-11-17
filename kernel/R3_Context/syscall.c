#include "context.h"
#include "pcb.h"
#include <mpx/serial.h>
#include <stddef.h>
#include <sys_req.h>
#include <string.h>


#define IDLE 1
#define EXIT 0
#define LOG_BUFFER_SIZE 256
#define DEBUG_MODE 1  // Set to 0 to disable debugging logs

// Global variables
static struct pcb *current_pcb = NULL;
static struct context *initial_context = NULL;
static int debugMode = DEBUG_MODE;

// Logging utility functions
    static void klogv(device dev, const char *msg)
    {
        char prefix[] = "klogv: ";
        serial_out(dev, prefix, strlen(prefix));
        serial_out(dev, msg, strlen(msg));
        serial_out(dev, "\r\n", 2);
    }

void log_context(const char *prefix, struct context *ctx) {
    if (!debugMode || !ctx) {
        return;
    }
    char buffer[LOG_BUFFER_SIZE];
    snprintf(buffer, LOG_BUFFER_SIZE, "%s EAX: %u, EBX: %u, ECX: %u, EDX: %u, CS: %u, DS: %u\n",
             prefix, ctx->eax, ctx->ebx, ctx->ecx, ctx->edx, ctx->cs, ctx->ds);
    klogv(COM1, buffer);
}


// Function prototypes
static void save_context(struct context *ctx);
static struct pcb* select_next_process(void);
static struct context* restore_context(struct pcb *pcb);
static int sys_req_idle(void);

// System call implementation
struct context *sys_call(struct context *ctx) {
    if (!ctx) {
        klogv(COM1, "Context passed is NULL. Exiting sys_call");
        return NULL;
    }

    if (!initial_context) {
        initial_context = ctx;
        klogv(COM1, "Initial context set.");
    }

    char buffer[LOG_BUFFER_SIZE];
    snprintf(buffer, LOG_BUFFER_SIZE, "Operation Code (EAX): %d", ctx->eax);
    klogv(COM1, buffer);

    switch (ctx->eax) {
        case IDLE:
            if (sys_req_idle() == 0) {
                save_context(ctx);
                struct pcb *next_pcb = select_next_process();
                if (next_pcb) {
                    current_pcb = next_pcb;
                    klogv(COM1, "Context switch to next process.");
                    return restore_context(next_pcb);
                }
            }
            ctx->eax = 0;
            klogv(COM1, "No process ready. Continuing with current context.");
            return ctx;

        case EXIT:
            if (current_pcb) {
                pcb_remove(current_pcb);
                pcb_free(current_pcb);
                current_pcb = NULL;
                klogv(COM1, "Current PCB removed and freed.");
            }

            struct pcb *next_pcb_exit = select_next_process();
            if (next_pcb_exit) {
                current_pcb = next_pcb_exit;
                klogv(COM1, "Context switch to next process after EXIT.");
                return restore_context(next_pcb_exit);
            } else {
                klogv(COM1, "No more processes. Returning initial context.");
                ctx->eax = 0;
                return initial_context;
            }

        default:
            snprintf(buffer, LOG_BUFFER_SIZE, "Unsupported operation code: %d", ctx->eax);
            klogv(COM1, buffer);
            ctx->eax = -1;
            return ctx;
    }
}

// Save context function
static void save_context(struct context *ctx) {
    klogv(COM1, "Saving context\n");
    if (current_pcb) {
        current_pcb->stack_pointer = ctx;
        klogv(COM1, "Context saved to current PCB.\n");
    }
}

// Select next process function
static struct pcb* select_next_process(void) {
    klogv(COM1, "Selecting next process.\n");
    struct pcb *selected_pcb = NULL;
    if (ReadyQueue && ReadyQueue->exec_state == READY && ReadyQueue->disp_state == NOT_SUSPENDED) {
        selected_pcb = ReadyQueue;
        pcb_remove(ReadyQueue);
        klogv(COM1, "Process selected from ReadyQueue.\n");
    }
    return selected_pcb;
}

// Restore context function
static struct context* restore_context(struct pcb *pcb) {
    klogv(COM1, "Restoring context.\n");
    if (!pcb) {
        klogv(COM1, "PCB is NULL. Cannot restore context.\n");
        return NULL;
    }
    return pcb->stack_pointer;
}

// Check if system request is idle
static int sys_req_idle(void) {
    struct pcb *current = ReadyQueue;
    while (current) {
        if (current->exec_state == READY && current->disp_state == NOT_SUSPENDED) {
            klogv(COM1, "Found a ready and not suspended process.\n");
            return 0;
        }
        current = current->next;
    }
    klogv(COM1, "No ready and non-suspended processes found.\n");
    return -1;
}

