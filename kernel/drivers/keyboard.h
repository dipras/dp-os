#ifndef KEYBOARD_H
#define KEYBOARD_H

int keyboard_has_data();
unsigned char keyboard_read_scancode();
char scancode_to_ascii(unsigned char scancode);

void keyboard_on_irq();

void keyboard_buffer_push(char c);
char keyboard_buffer_pop();
int keyboard_buffer_empty();
void keyboard_flush();


#endif