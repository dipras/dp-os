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

void main() {
    clear_screen();

    char *msg = "Hello World \n";
    print_string(msg);
    print_string("Disini adalah test");
}
