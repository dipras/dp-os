#include "terminal/terminal.h" 

void main() {
    terminal_init();
    terminal_write("Hello Kawan ");
    char answer[12];
    terminal_readline(answer, 12);
}
