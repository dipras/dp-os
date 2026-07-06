#include "vga.h"
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

volatile char *video = (volatile char*)0xB8000;

void vga_clear(unsigned char color) {
  for(int i = 0; i < VGA_WIDTH * VGA_HEIGHT * 2; i += 2) {
    video[i] = ' ';
    video[i + 1] = color;
  }
}

void vga_put_at(char c, int x, int y, unsigned char color) {
  int pos = (y * VGA_WIDTH + x) * 2;
  video[pos] = c;
  video[pos + 1] = color;
}
