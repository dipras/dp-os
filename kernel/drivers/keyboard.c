#include "keyboard.h"
#include "io.h"
#include "../terminal/terminal.h"


// pool method, not efficient
int keyboard_has_data() {
    return inb(0x64) & 1;
}

unsigned char keyboard_read_scancode() {
    while(!keyboard_has_data()) {}
    return inb(0x60);
}

char scancode_to_ascii(unsigned char scancode) {
    char map[128] = {
        0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
        '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
        0, 'a','s','d','f','g','h','j','k','l',';','\'','`',
        0, '\\','z','x','c','v','b','n','m',',','.','/',
        0, '*', 0, ' '
    };

    if(scancode > 127) return 0;
    return map[scancode];
}

#define KB_BUF_SIZE 256

static volatile char kb_buffer[KB_BUF_SIZE];
static volatile int kb_head = 0; // dibaca oleh consumer (mis. terminal_readline)
static volatile int kb_tail = 0; // ditulis oleh producer (irq_handler)

// dipanggil dari irq_handler saat IRQ1 terjadi
void keyboard_on_irq() {
    unsigned char scancode = inb(0x60);

    // bit 0x80 set berarti key release, bukan key press
    if (scancode & 0x80) return;

    char c = scancode_to_ascii(scancode);
    if (c == 0) return;

    keyboard_buffer_push(c);
}

void keyboard_buffer_push(char c) {
    int next = (kb_tail + 1) % KB_BUF_SIZE;

    // buffer penuh, drop karakter baru
    if (next == kb_head) return;

    kb_buffer[kb_tail] = c;
    kb_tail = next;
}

char keyboard_buffer_pop() {
    while (keyboard_buffer_empty()) {
        __asm__ volatile ("hlt");
    }

    char c = kb_buffer[kb_head];
    kb_head = (kb_head + 1) % KB_BUF_SIZE;
    return c;
}

int keyboard_buffer_empty() {
    return kb_head == kb_tail;
}

void keyboard_flush() {
    kb_head = kb_tail;
}