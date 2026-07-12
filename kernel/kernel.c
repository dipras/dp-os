#include "terminal/terminal.h" 
#include "arch/x86/idt.h"
#define BUF_LEN 8

void main(void) {
    terminal_init();
    idt_init();

    terminal_write("IDT initialized\n");

    volatile int zero = 0;
    volatile int result = 10 / zero;

    (void)result;

    while (1) {
        __asm__ volatile ("hlt");
    }
}
