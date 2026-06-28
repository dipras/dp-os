#include "vga.h"
#define VGA_ROW 80
#define VGA_COL 25

volatile char *video = (volatile char*)0xB8000;

void vga_clear(unsigned char color) {
  for(int i = 0; i < VGA_ROW * VGA_COL * 2; i += 2) {
    video[i] = ' ';
    video[i + 1] = color;
  }
}

void vga_put_at(char c, int x, int y, unsigned char color) {
  int pos = x * y;
  video[pos] = c;
  video[pos + 1] = color;
}
