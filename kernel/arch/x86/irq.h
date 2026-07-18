#ifndef IRQ_H
#define IRQ_H

#include <stdint.h>
typedef struct interrupt_frame {
    uint32_t gs;
    uint32_t fs;
    uint32_t es;
    uint32_t ds;

    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    uint32_t irq;
    uint32_t vector;
} interrupt_frame_t;

void irq_handler(interrupt_frame_t* frame);


#endif