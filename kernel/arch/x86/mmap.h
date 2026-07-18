#ifndef MMAP_H
#define MMAP_H

#include <stdint.h>

typedef struct {
    uint64_t base;
    uint64_t length;
    uint32_t type;
    uint32_t acpi_ext;
} __attribute__((packed)) mmap_entry_t;

#define MMAP_TYPE_USABLE 1

void mmap_print(void);

#endif
