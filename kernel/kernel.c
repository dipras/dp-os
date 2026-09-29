#include "terminal/terminal.h"
#include "arch/x86/idt.h"
#include "arch/x86/mmap.h"
#include "arch/x86/paging.h"
#include "mm/pmm.h"
#define BUF_LEN 8

void main(void) {
    terminal_init();
    idt_init();

    terminal_write("IDT initialized\n");

    mmap_print();

    pmm_init();
    pmm_print_stats();

    paging_init();
    pmm_print_stats(); // frame yang dipakai page directory/table ikut kepotong

    // demo cepat: alokasi 2 frame, lihat free count turun, lalu bebaskan 1
    uint32_t f1 = pmm_alloc_frame();
    uint32_t f2 = pmm_alloc_frame();
    pmm_print_stats();

    pmm_free_frame(f1);
    pmm_print_stats();
    (void)f2;

    char buf[BUF_LEN];
    while (1) {
        terminal_write("> ");
        terminal_readline(buf, BUF_LEN);
        terminal_write("You typed: ");
        terminal_write(buf);
        terminal_write("\n");
    }
}
