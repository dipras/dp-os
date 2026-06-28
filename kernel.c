volatile char* video = (volatile char*)0xB8000;

void clear_screen() {

    for (int i = 0; i < 80 * 25 * 2; i += 2) {
        video[i] = ' ';
        video[i + 1] = 0x0F;
    }
}

int cursor_x = 0;
int cursor_y = 0;

void putchar(char c) {
    if(c == '\n') {
        cursor_x = 0;
        cursor_y++;
        return;
    }
    
    int offset = (cursor_y * 80 + cursor_x) * 2;

    video[offset] = c;
    video[offset + 1] = 0x0F;

    cursor_x++;

    if(cursor_x >= 80) {
        cursor_x = 0;
        cursor_y++;
    }
}
void print_string(const char* str) {
    while (*str != '\0') {
        putchar(*str++);        
    }
}

void backspace() {
    if (cursor_x == 0 && cursor_y == 0)
        return;

    if (cursor_x > 0) {
        cursor_x--;
    } else {
        cursor_y--;
        cursor_x = 79;
    }

    int offset = (cursor_y * 80 + cursor_x) * 2;

    video[offset] = ' ';
    video[offset + 1] = 0x0F;
}

static inline unsigned char inb(unsigned short port) {
    unsigned char result;
    __asm__ volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

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

void readline(char *buffer, int max_len) {
    int i = 0;

    while (1) {
        unsigned char scancode = keyboard_read_scancode();

        // skip key release
        if (scancode & 0x80)
            continue;

        char c = scancode_to_ascii(scancode);

        if (c == 0)
            continue;

        if (c == '\n') {
            putchar('\n');
            buffer[i] = '\0';
            return;
        }

        if (c == '\b') {
            if (i > 0) {
                i--;
                backspace();
            }
            continue;
        }

        if (i < max_len - 1) {
            buffer[i] = c;
            i++;
            putchar(c);
        }
    }
}

void main() {
    clear_screen();

    char *msg = "Hello World \n";
    print_string(msg);
    char *retrive;
    int maxLen = 100;
    readline(retrive, maxLen); 
}
