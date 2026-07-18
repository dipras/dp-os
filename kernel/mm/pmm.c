#include "pmm.h"
#include "../arch/x86/mmap.h"
#include "../terminal/terminal.h"

#define FRAME_SIZE 4096

// disediakan oleh linker.ld, menandai alamat tepat setelah akhir kernel
extern uint8_t _kernel_end;

// bitmap-nya sendiri diletakkan di RAM tepat setelah kernel, ukurannya
// dihitung saat runtime dari total RAM yang terdeteksi e820 (bukan
// static array di compile time, biar kernel.bin gak ikut bengkak)
static uint32_t* bitmap;
static uint32_t bitmap_size_dwords;
static uint32_t total_frames;

static inline uint32_t align_up(uint32_t addr, uint32_t align) {
    return (addr + align - 1) & ~(align - 1);
}

static void bitmap_set(uint32_t frame) {
    bitmap[frame / 32] |= (1u << (frame % 32));
}

static void bitmap_clear(uint32_t frame) {
    bitmap[frame / 32] &= ~(1u << (frame % 32));
}

static int bitmap_test(uint32_t frame) {
    return bitmap[frame / 32] & (1u << (frame % 32));
}

static void mark_range_used(uint32_t start_addr, uint32_t end_addr) {
    uint32_t start_frame = start_addr / FRAME_SIZE;
    uint32_t end_frame = align_up(end_addr, FRAME_SIZE) / FRAME_SIZE;

    for (uint32_t f = start_frame; f < end_frame && f < total_frames; f++) {
        bitmap_set(f);
    }
}

static void mark_range_free(uint32_t start_addr, uint32_t end_addr) {
    uint32_t start_frame = align_up(start_addr, FRAME_SIZE) / FRAME_SIZE;
    uint32_t end_frame = end_addr / FRAME_SIZE;

    for (uint32_t f = start_frame; f < end_frame && f < total_frames; f++) {
        bitmap_clear(f);
    }
}

void pmm_init(void) {
    uint32_t count = mmap_get_count();
    mmap_entry_t* entries = mmap_get_entries();

    // cari alamat tertinggi yang disebut e820 supaya bitmap bisa cover semuanya
    uint64_t highest = 0;
    for (uint32_t i = 0; i < count; i++) {
        uint64_t end = entries[i].base + entries[i].length;
        if (end > highest) highest = end;
    }

    // batasi ke 4GB, karena kernel ini masih 32-bit
    if (highest > 0xFFFFFFFFULL) highest = 0xFFFFFFFFULL;

    total_frames = (uint32_t)(highest / FRAME_SIZE);
    bitmap_size_dwords = (total_frames + 31) / 32;

    uint32_t kernel_end = align_up((uint32_t)&_kernel_end, FRAME_SIZE);
    bitmap = (uint32_t*)kernel_end;

    // 1. default: anggap semua frame "dipakai"
    for (uint32_t i = 0; i < bitmap_size_dwords; i++) {
        bitmap[i] = 0xFFFFFFFF;
    }

    // 2. bebaskan frame yang menurut e820 usable
    for (uint32_t i = 0; i < count; i++) {
        if (entries[i].type != MMAP_TYPE_USABLE) continue;
        if (entries[i].base > 0xFFFFFFFFULL) continue;

        uint64_t end64 = entries[i].base + entries[i].length;
        uint32_t start = (uint32_t)entries[i].base;
        uint32_t end = (end64 > 0xFFFFFFFFULL) ? 0xFFFFFFFFu : (uint32_t)end64;

        mark_range_free(start, end);
    }

    // 3. kunci lagi low memory + boot sector + kernel + bitmap itu sendiri,
    //    supaya PMM tidak pernah meminjamkan alamat yang sudah dipakai
    //    (langkah 2 di atas sempat membebaskannya karena masuk range usable)
    uint32_t bitmap_end = kernel_end + bitmap_size_dwords * sizeof(uint32_t);
    mark_range_used(0x0, bitmap_end);
}

uint32_t pmm_alloc_frame(void) {
    for (uint32_t i = 0; i < bitmap_size_dwords; i++) {
        if (bitmap[i] == 0xFFFFFFFF) continue; // dword ini penuh

        for (uint32_t bit = 0; bit < 32; bit++) {
            uint32_t frame = i * 32 + bit;
            if (frame >= total_frames) break;

            if (!bitmap_test(frame)) {
                bitmap_set(frame);
                return frame * FRAME_SIZE;
            }
        }
    }

    return 0; // out of memory
}

void pmm_free_frame(uint32_t addr) {
    uint32_t frame = addr / FRAME_SIZE;
    if (frame < total_frames) {
        bitmap_clear(frame);
    }
}

uint32_t pmm_total_frames(void) {
    return total_frames;
}

uint32_t pmm_free_frames(void) {
    uint32_t free_count = 0;
    for (uint32_t f = 0; f < total_frames; f++) {
        if (!bitmap_test(f)) free_count++;
    }
    return free_count;
}

static void print_hex32(uint32_t value) {
    static const char hex[] = "0123456789ABCDEF";
    char buf[11]; // "0x" + 8 digit hex + '\0'

    buf[0] = '0';
    buf[1] = 'x';
    for (int i = 0; i < 8; i++) {
        uint8_t nibble = (value >> ((7 - i) * 4)) & 0xF;
        buf[2 + i] = hex[nibble];
    }
    buf[10] = '\0';

    terminal_write(buf);
}

void pmm_print_stats(void) {
    terminal_write("PMM: total=");
    print_hex32(total_frames);
    terminal_write(" frames, free=");
    print_hex32(pmm_free_frames());
    terminal_write(" frames\n");
}
