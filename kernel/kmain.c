#include <mpx/gdt.h>
#include <mpx/interrupts.h>
#include <mpx/serial.h>
#include <mpx/vm.h>
#include <sys_req.h>
#include <string.h>
#include <memory.h>
#include "cmdHandler.h"

static void klogv(device dev, const char *msg)
{
    char prefix[] = "klogv: ";
    serial_out(dev, prefix, strlen(prefix));
    serial_out(dev, msg, strlen(msg));
    serial_out(dev, "\r\n", 2);
}

void kmain(void)
{
    // Initialize serial I/O first so we have a means of communication
    klogv(COM1, "Initialized serial I/O on COM1 device...");

    // Initialize Global Descriptor Table
    klogv(COM1, "Initializing Global Descriptor Table...");

    // Initialize Interrupt Descriptor Table
    klogv(COM1, "Initializing Interrupt Descriptor Table...");

    // Disable Interrupts while setting up
    klogv(COM1, "Disabling interrupts...");

    // Initialize Interrupt Request routines
    klogv(COM1, "Initializing Interrupt Request routines...");

    // Initialize Programmable Interrupt Controller
    klogv(COM1, "Initializing Programmable Interrupt Controller...");

    // Re-enable interrupts now that they're set up
    klogv(COM1, "Enabling Interrupts...");

    // Initialize Virtual Memory
    klogv(COM1, "Initializing Virtual Memory...");

    // Module specific initialization
    klogv(COM1, "Initializing MPX modules...");
    // R5: sys_set_heap_functions(...);
    // R4: create commhand and idle processes

    // Transfer control to the command handler after all initialization
    klogv(COM1, "Transferring control to commhand...");
    comhand();  // At this point, the user can provide input

    // Begin system shutdown procedure (this will only be reached if comhand() returns)
    klogv(COM1, "Starting system shutdown procedure...");

    // Halt the CPU or perform any final steps
    klogv(COM1, "Halting CPU...");
}
