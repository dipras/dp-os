#ifndef VGA_H
#define VGA_H
void vga_put_at(char c, int x, int y, unsigned char color);
void vga_clear(unsigned char color);
void vga_scroll();
#endif
