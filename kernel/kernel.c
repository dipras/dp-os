#include "terminal/terminal.h" 
#include "lib/string.h"
#define BUF_LEN 8

void main() {
    terminal_clear();
    terminal_write("Welcome to DP Shell\n");
    char buffer[BUF_LEN];
    while(1) {
        terminal_write("> ");
        terminal_readline(buffer, BUF_LEN);
        if(streq(buffer, "help")) {
            terminal_write("DP terminal:\n");
            terminal_write("  help  - Show what you can do\n");
            terminal_write("  clear - Clearn terminal\n");
        } else if(streq(buffer, "clear")) {
            terminal_clear();
        } else {
            terminal_write("Unknown terminal\n");
        }
    }
}
