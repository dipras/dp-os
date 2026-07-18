#include "terminal/terminal.h" 
#include "arch/x86/idt.h"
#define BUF_LEN 8

void main(void) {
    terminal_init();
    idt_init();

    terminal_write("IDT initialized\n");

    while (1) {
        __asm__ volatile ("hlt");
    }
}
