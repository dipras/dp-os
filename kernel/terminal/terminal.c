#include "terminal.h"
#include "../drivers/vga.h"

int cursor_y = 0;
int cursor_x = 0;

void terminal_clear() {
  vga_clear(0x0F);
}

void terminal_init() {
  terminal_clear();
}

void terminal_putchar(char c) {
  int newLine = 0;
  if(c == '\n') {
    newLine = 1;
  } else {
    vga_put_at(c, cursor_x, cursor_y, 0x0F);
  }
  if(cursor_x < 79 && !newLine) {
    cursor_x++;
  } else {
    cursor_y++;
    cursor_x = 0;
  }

  if(cursor_y >= VGA_HEIGHT) {
    vga_scroll();
    cursor_y--;
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
