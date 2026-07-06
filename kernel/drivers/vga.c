#include "vga.h"

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

void vga_scroll() {
  int VGA_MEM_WIDTH = (VGA_WIDTH * 2);
  for(int i = 1; i < VGA_HEIGHT; i++) {
    for(int j = 0; j < VGA_MEM_WIDTH; j += 2) {
      video[VGA_MEM_WIDTH * (i - 1) + j] = video[VGA_MEM_WIDTH * i + j];
      video[VGA_MEM_WIDTH * (i - 1) + j + 1] = video[VGA_MEM_WIDTH * i + j + 1];
    }
  }

  // clean last row of screen avoid duplicate
  for(int j = 0; j < VGA_WIDTH; j++) {
    vga_put_at(' ', j, VGA_HEIGHT - 1, 0x0F);
  }

}
