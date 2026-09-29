#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

#define PAGE_PRESENT  0x1
#define PAGE_WRITABLE 0x2
#define PAGE_USER     0x4

void paging_init(void);
void paging_map_page(uint32_t virtual_addr, uint32_t physical_addr, uint32_t flags);

#endif
