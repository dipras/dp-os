#include "terminal.h"
#include "../drivers/vga.h"

int cursor_y = 0;
int cursor_x = 0;

void terminal_init() {
  vga_clear(0x0F);
}

void terminal_putchar(char c) {
  vga_put_at(c, cursor_x, cursor_y, 0x0F);
  if(cursor_x < 80) {
    cursor_x++;
  } else {
    cursor_y++;
    cursor_x = 0;
  }
}

void terminal_write(const char* str) {
  while(*str != '\0') {
    terminal_putchar(*str++);
  }
}

void terminal_backspace() {
  if(cursor_x > 0) {
    cursor_x--;
  } else {
    if(cursor_y <= 0) return;
    cursor_y--;
    cursor_x = 79;
  }

  vga_put_at(' ', cursor_x, cursor_y, 0x0F);
}
