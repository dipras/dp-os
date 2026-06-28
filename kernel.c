int text_pos = 0;

void clear_screen() {
    char* video_memory = (char*)0xB8000;

    for (int i = 0; i < 80 * 25 * 2; i += 2) {
        video_memory[i] = ' ';
        video_memory[i + 1] = 0x0F;
    }
}

void print_string(const char* str) {
    char* vid_mem = (char*)0xB8000;

    while (*str != '\0') {
        *vid_mem++ = *str++;
        *vid_mem++ = 0x0F;
    }
}

void main() {
    clear_screen();

    char *msg = "Hello World";
    print_string(msg);
}
