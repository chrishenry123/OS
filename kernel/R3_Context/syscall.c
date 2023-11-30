#include "context.h"
#include "pcb.h"
#include <stddef.h>

#define IDLE 1
#define EXIT 0

//global PCB pointer representing the currently executing process
struct pcb *current_pcb = NULL;
// static context pointer representing the context from the first time syscall() is entered
static struct context *initial_context = NULL;

// Function prototypes
static void save_context(struct context *ctx);
static struct pcb* select_next_process(void);
//static struct context* return_context(struct pcb *pcb);
static int sys_req_idle(void);

// system call implementation
// takes one param - the context of the current process (pointer)
struct context *sys_call(struct context *ctx) {
    // checks gen purpose register of the context containing operation code
    switch (ctx->eax) {
        // case for idle
        case IDLE:
            // checking if initial context is null
            if (initial_context == NULL) {
                //setting it to point to the context provided as a parameter
                initial_context = ctx;
            }
            if (current_pcb != NULL) {
                save_context(ctx);
                current_pcb->exec_state = READY;
                pcb_insert(current_pcb);

            }
            break;

       case EXIT:
        // check for current pcb
        if (current_pcb) {
            // delete current pcb
            // pcb_remove(current_pcb);
            // frees the memory
            pcb_free(current_pcb);
            current_pcb = NULL;
        }
            break;

      default:
            ctx->eax = -1;
            return ctx;
}

    //work
    // checks if there are any ready, non-suspended PCBs in the queue
    if (sys_req_idle() == 0){
        struct pcb *next_pcb = select_next_process();
    //    pcb_remove(next_pcb);
        current_pcb = next_pcb;
        current_pcb->exec_state = READY;


        return (struct context *) current_pcb->stack_pointer;


    }
    else{ // no process
                ctx  = initial_context;
                initial_context = NULL;
                return ctx;

            }



}

static int sys_req_idle(void) {
    struct pcb *current = ReadyQueue;
    // search for pcb that is ready and not suspended
    while (current != NULL) {
        if (current->exec_state == READY && current->disp_state == NOT_SUSPENDED) {
            return 0;
        }
        current = current->next;
    }
    return -1;
}
//if there are any ready, non-suspended PCBs in the queue,
// remove the first from the queue and store it in a temporary variable as the next process
static struct pcb* select_next_process(void) {
    // variable to store selected process
    struct pcb *selected_pcb = NULL;
    // makes sure ReadyQueue is not empty, execution state is ready and the dispatch state is not suspended
    if (ReadyQueue && ReadyQueue->exec_state == READY && ReadyQueue->disp_state == NOT_SUSPENDED) {
        // would this load the first
        selected_pcb = ReadyQueue;
        pcb_remove(ReadyQueue);
    }
    return selected_pcb;
}

// Restore context function
//static struct context* return_context(struct pcb *pcb) {
 //   if (pcb == NULL) {
   //     return NULL;
  //  }
  //  return pcb->stack_pointer;
//}
//}

// saves the context by updating the stack pointer
static void save_context(struct context *ctx) {
    if (current_pcb) {
        current_pcb->stack_pointer = ctx;
    }
}
