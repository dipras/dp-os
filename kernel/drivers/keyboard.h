#ifndef KEYBOARD_H
#define KEYBOARD_H

int keyboard_has_data();
unsigned char keyboard_read_scancode();
char scancode_to_ascii(unsigned char scancode);
void keyboard_to_terminal();


#endif