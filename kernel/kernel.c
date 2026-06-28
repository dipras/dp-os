#include "drivers/vga.h" 

void main() {
   vga_clear(0x0F);
   vga_put_at('H', 0, 0, 0X0F);
}
