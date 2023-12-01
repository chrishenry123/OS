#include <mpx/gdt.h>
#include <mpx/interrupts.h>
#include <mpx/serial.h>
#include <mpx/vm.h>
#include <sys_req.h>
#include <string.h>
#include <memory.h>
#include <pcb.h>
#include <pcbuser.h>
#include <context.h>
#include <processes.h>
#include <stdint.h>
#include <cmdHandler.h>
#include <processes.h>

void init_comhand_process(void);
void init_system_idle_process(void);


static void klogv(device dev, const char *msg)
{
    char prefix[] = "klogv: ";
    serial_out(dev, prefix, strlen(prefix));
    serial_out(dev, msg, strlen(msg));
    serial_out(dev, "\r\n", 2);
}

void init_comhand_process(void) {
    struct pcb* comHand = pcb_setup("comhand", 1, 0);
    if (!comHand) {
        // Handle error
        klogv(COM1, "Failed to setup comhand process...");
        return;
    }
    struct context* ctx = comHand->stack_pointer;
    ctx->ds = 0x10;
    ctx->es = 0x10;
    ctx->fs = 0x10;
    ctx->gs = 0x10;
    ctx->ss = 0x10;
    ctx->eax = 0x00;
    ctx->ebx = 0x00;
    ctx->ecx = 0x00;
    ctx->edx = 0x00;
    ctx->esi = 0x00;
    ctx->edi = 0x00;
    ctx->ebp = (int)comHand->stack;
    //ctx->esp = (int)comHand->stack_pointer;
    ctx->eip = (unsigned int)comhand;
    ctx->cs = 0x08;
    ctx->eflags = 0x0202;
    pcb_insert(comHand);
    klogv(COM1, "Successfully initialized comhand...");
}

void init_system_idle_process(void) {
    struct pcb* systemIdle = pcb_setup("Sys IDLE Proc", 1, 9);
    if (!systemIdle) {
        // Handle error
        klogv(COM1, "Failed to setup System IDLE Process...");
        return;
    }
    struct context* ctx = (struct context *)systemIdle->stack_pointer;
    ctx->ds = 0x10;
    ctx->es = 0x10;
    ctx->fs = 0x10;
    ctx->gs = 0x10;
    ctx->ss = 0x10;
    ctx->eax = 0x00;
    ctx->ebx = 0x00;
    ctx->ecx = 0x00;
    ctx->edx = 0x00;
    ctx->esi = 0x00;
    ctx->edi = 0x00;
    ctx->ebp = (int)systemIdle->stack;
    //ctx->esp = (int)systemIdle->stack_pointer;
    ctx->eip = (unsigned int)sys_idle_process;
    ctx->cs = 0x08;
    ctx->eflags = 0x0202;
    pcb_insert(systemIdle);
    klogv(COM1, "Successfully initialized system idle process...");
}

void kmain(void)
{
    // 0) Serial I/O -- <mpx/serial.h>
    // If we don't initialize the serial port, we have no way of
    // performing I/O. So we need to do that before anything else so we
    // can at least get some output on the screen.
    // Note that here, you should call the function *before* the output
    // via klogv(), or the message won't print. In all other cases, the
    // output should come first as it describes what is about to happen.


    serial_init(COM1);
    //serial_out(COM1, buffer, len);
    klogv(COM1, "Initialized serial I/O on COM1 device...");

    // 1) Global Descriptor Table (GDT) -- <mpx/gdt.h>
    // Keeps track of the various memory segments (Code, Data, Stack, etc.)
    // required by the x86 architecture. This needs to be initialized before
    // interrupts can be configured.
    klogv(COM1, "Initializing Global Descriptor Table...");
    gdt_init();

    // 2) Interrupt Descriptor Table (IDT) -- <mpx/interrupts.h>
    // Keeps track of where the various Interrupt Vectors are stored. It
    // needs to be initialized before Interrupt Service Routines (ISRs) can
    // be installed.
    klogv(COM1, "Initializing Interrupt Descriptor Table...");
    idt_init();

    // 3) Disable Interrupts -- <mpx/interrupts.h>
    // You'll be modifying how interrupts work, so disable them to avoid
    // crashing.
    klogv(COM1, "Disabling interrupts...");
    cli();

    // 4) Interrupt Request (IRQ) -- <mpx/interrupts.h>
    // The x86 architecture requires ISRs for at least the first 32
    // Interrupt Request (IRQ) lines.
    klogv(COM1, "Initializing Interrupt Request routines...");
    irq_init();


    // 5) Programmable Interrupt Controller (PIC) -- <mpx/interrupts.h>
    // The x86 architecture uses a Programmable Interrupt Controller (PIC)
    // to map hardware interrupts to software interrupts that the CPU can
    // then handle via the IDT and its list of ISRs.
    klogv(COM1, "Initializing Programmable Interrupt Controller...");
    pic_init();

    // 6) Reenable interrupts -- <mpx/interrupts.h>
    // Now that interrupt routines are set up, allow interrupts to happen
    // again.
    klogv(COM1, "Enabling Interrupts...");
    sti();

    // 7) Virtual Memory (VM) -- <mpx/vm.h>
    // Virtual Memory (VM) allows the CPU to map logical addresses used by
    // programs to physical address in RAM. This allows each process to
    // behave as though it has exclusive access to memory. It also allows,
    // in more advanced systems, the kernel to swap programs between RAM and
    // storage (such as a hard drive or SSD), and to set permissions such as
    // Read, Write, or Execute for pages of memory. VM is managed through
    // Page Tables, data structures that describe the logical-to-physical
    // mapping as well as manage permissions and other metadata.
    klogv(COM1, "Initializing Virtual Memory...");
    vm_init();

    // 8) MPX Modules -- *headers vary*
    // Module specific initialization -- not all modules require this.
    klogv(COM1, "Initializing MPX modules...");
    // R5: sys_set_heap_functions(...);
    // R4: create commhand and idle processes

    // 9) YOUR command handler -- *create and #include an appropriate .h file*
    // Pass execution to your command handler so the user can interact with
    // the system.
    klogv(COM1, "Transferring control to commhand...");
    // i uncommented this for r4

    //__asm__ volatile ("int $0x60" :: "a"(IDLE));
    //__asm__ volatile ("int $0x60" :: "a"(IDLE));
    // Setup Command Handler process

    comhand();
    init_comhand_process();
    init_system_idle_process();
    __asm__ volatile ("int $0x60" :: "a"(IDLE));

    // 10) System Shutdown -- *headers to be determined by your design*
    // After your command handler returns, take care of any clean up that
    // is necessary.
    klogv(COM1, "Starting system shutdown procedure...");


    // 11) Halt CPU -- *no headers necessary, no changes necessary*
    // Execution of kmain() will complete and return to where it was called
    // in boot.s, which will then attempt to power off Qemu or halt the CPU.
    klogv(COM1, "Halting CPU...");
}
