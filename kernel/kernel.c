#include "terminal/terminal.h" 
#include "drivers/vga.h"

void main() {
    terminal_init();
    terminal_write("Hello\nworld\ndisini\ndisana\nlesggooo\nmalik\nweheheh\nlesgooo\nmakan bang\ntuju dua\nwowowowowoo\ntesttesttes\nsebanyak banyaknya\ntututu\ndedede\nwewewewe\ndudududu\nyuyuyuu\nlelele\ngigigi\nlelelel\nyuuufjlkfwj\nheheheh\nduduud\nljlj\n");
    vga_scroll();
}
