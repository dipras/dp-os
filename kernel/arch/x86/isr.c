#include "isr.h"
#include "../../terminal/terminal.h"

static const char* exception_messages[32] = {
    "Division By Zero",
    "Debug",
    "Non-Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 Floating Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating Point Exception",
    "Virtualization Exception",
    "Control Protection Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Hypervisor Injection Exception",
    "VMM Communication Exception",
    "Security Exception",
    "Reserved"
};

void isr_handler(registers_t* registers) {
    terminal_write("\nCPU exception: ");

    if (registers->interrupt_number < 32) {
        terminal_write(
            exception_messages[registers->interrupt_number]
        );
    } else {
        terminal_write("Unknown");
    }

    terminal_write("\nSystem halted.");

    while (1) {
        __asm__ volatile ("cli; hlt");
    }
}