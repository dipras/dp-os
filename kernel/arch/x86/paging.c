#include "paging.h"
#include "../../mm/pmm.h"
#include "../../terminal/terminal.h"

#define PAGE_SIZE 4096
#define ENTRIES_PER_TABLE 1024

// diidentity-map di awal (virtual == fisik), cukup buat menampung kernel,
// struktur PMM, dan sisa ruang buat page table/heap awal
#define IDENTITY_MAP_SIZE (16 * 1024 * 1024) // 16MB

static uint32_t* page_directory; // 1024 dword, tiap entry nunjuk ke satu page table

static inline void invlpg(uint32_t addr) {
    __asm__ volatile("invlpg (%0)" :: "r"(addr) : "memory");
}

// ambil page table buat index tertentu di page directory, bikin baru
// (pinjam 1 frame dari PMM) kalau belum ada
static uint32_t* get_or_create_table(uint32_t pd_index) {
    if (!(page_directory[pd_index] & PAGE_PRESENT)) {
        uint32_t frame = pmm_alloc_frame();
        uint32_t* table = (uint32_t*)frame;

        for (int i = 0; i < ENTRIES_PER_TABLE; i++) {
            table[i] = 0;
        }

        page_directory[pd_index] = frame | PAGE_PRESENT | PAGE_WRITABLE;
    }

    return (uint32_t*)(page_directory[pd_index] & ~0xFFF);
}

void paging_map_page(uint32_t virtual_addr, uint32_t physical_addr, uint32_t flags) {
    uint32_t pd_index = virtual_addr >> 22;
    uint32_t pt_index = (virtual_addr >> 12) & 0x3FF;

    uint32_t* table = get_or_create_table(pd_index);
    table[pt_index] = (physical_addr & ~0xFFF) | flags | PAGE_PRESENT;

    invlpg(virtual_addr);
}

void paging_init(void) {
    uint32_t pd_frame = pmm_alloc_frame();
    page_directory = (uint32_t*)pd_frame;

    for (int i = 0; i < ENTRIES_PER_TABLE; i++) {
        page_directory[i] = 0;
    }

    // identity map: virtual == fisik, supaya eksekusi kernel yang sedang
    // berjalan sekarang tidak langsung page fault begitu paging diaktifkan
    for (uint32_t addr = 0; addr < IDENTITY_MAP_SIZE; addr += PAGE_SIZE) {
        paging_map_page(addr, addr, PAGE_WRITABLE);
    }

    __asm__ volatile(
        "mov %0, %%cr3\n"
        "mov %%cr0, %%eax\n"
        "or $0x80000000, %%eax\n"
        "mov %%eax, %%cr0\n"
        :: "r"(pd_frame)
        : "eax"
    );

    terminal_write("Paging enabled\n");
}
