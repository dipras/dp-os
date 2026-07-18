#include "mmap.h"
#include "../../terminal/terminal.h"

// harus sama persis dengan MMAP_COUNT / MMAP_ENTRIES di boot.asm
#define MMAP_COUNT_ADDR   0x1000
#define MMAP_ENTRIES_ADDR 0x1004

static void print_hex64(uint64_t value) {
    static const char hex[] = "0123456789ABCDEF";
    char buf[19]; // "0x" + 16 digit hex + '\0'

    buf[0] = '0';
    buf[1] = 'x';
    for (int i = 0; i < 16; i++) {
        uint8_t nibble = (value >> ((15 - i) * 4)) & 0xF;
        buf[2 + i] = hex[nibble];
    }
    buf[18] = '\0';

    terminal_write(buf);
}

static const char* type_to_string(uint32_t type) {
    switch (type) {
        case 1: return "Usable";
        case 2: return "Reserved";
        case 3: return "ACPI Reclaimable";
        case 4: return "ACPI NVS";
        case 5: return "Bad Memory";
        default: return "Unknown";
    }
}

uint32_t mmap_get_count(void) {
    return *(volatile uint32_t*)MMAP_COUNT_ADDR;
}

mmap_entry_t* mmap_get_entries(void) {
    return (mmap_entry_t*)MMAP_ENTRIES_ADDR;
}

void mmap_print(void) {
    uint32_t count = mmap_get_count();
    mmap_entry_t* entries = mmap_get_entries();

    terminal_write("Memory map: ");
    print_hex64(count);
    terminal_write(" entries\n");

    for (uint32_t i = 0; i < count; i++) {
        terminal_write("  base=");
        print_hex64(entries[i].base);
        terminal_write(" len=");
        print_hex64(entries[i].length);
        terminal_write(" type=");
        terminal_write(type_to_string(entries[i].type));
        terminal_write("\n");
    }
}
