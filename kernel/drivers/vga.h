#ifndef VGA_H
#define VGA_H
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
void vga_put_at(char c, int x, int y, unsigned char color);
void vga_clear(unsigned char color);
void vga_scroll();
void vga_move_cursor(int x, int y);
#endif
