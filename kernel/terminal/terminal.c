#include "terminal.h"
#include "../drivers/vga.h"
#include "../drivers/keyboard.h"

int cursor_y = 0;
int cursor_x = 0;

void terminal_clear() {
  cursor_x = 0;
  cursor_y = 0;
  vga_clear(0x0F);
  vga_move_cursor(cursor_x, cursor_y);
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

  vga_move_cursor(cursor_x, cursor_y);
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
  vga_move_cursor(cursor_x, cursor_y);
}

void terminal_readline(char* buffer, int max_len) {
  keyboard_flush(); // abaikan ketikan sebelum readline dipanggil

  int i = 0;
  while(1) {
    char c = keyboard_buffer_pop();

    if(c == '\n') {
      terminal_putchar('\n');
      buffer[i] = '\0';
      return;
    }

    if(c == '\b') {
      if(i > 0) {
        i--;
        terminal_backspace();
      }

      continue;
    }

    if(i < max_len - 1) {
      buffer[i] = c;
      i++;
      terminal_putchar(c);
    }
  }
}
