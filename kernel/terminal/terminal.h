#ifndef TERMINAL_H
#define TERMINAL_H
void terminal_init();

void terminal_putchar(char c);

void terminal_write(const char* str);

void terminal_backspace();

void terminal_clear();

void terminal_render();
#endif
