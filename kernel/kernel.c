#include "terminal/terminal.h" 
#include "arch/x86/idt.h"
#define BUF_LEN 8

void main(void) {
    terminal_init();
    idt_init();

    terminal_write("IDT initialized\n");

    char buf[BUF_LEN];
    while (1) {
        terminal_write("> ");
        terminal_readline(buf, BUF_LEN);
        terminal_write("You typed: ");
        terminal_write(buf);
        terminal_write("\n");
    }
}
