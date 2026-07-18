#include "idt.h"
#include "pic.h"
#include "../../drivers/io.h"

static idt_entry_t idt[IDT_ENTRY_COUNT];
static idt_ptr_t idt_pointer;

extern void idt_load(uint32_t idt_ptr_address);

extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);

extern void irq0(void);
extern void irq1(void);

static void memory_set(void* destination, uint8_t value, uint32_t size) {
    uint8_t* bytes = (uint8_t*)destination;

    for (uint32_t i = 0; i < size; i++) {
        bytes[i] = value;
    }
}

void idt_set_gate(
    uint8_t vector,
    uint32_t handler_address,
    uint16_t selector,
    uint8_t type_attributes
) {
    idt[vector].offset_low = handler_address & 0xFFFF;
    idt[vector].selector = selector;
    idt[vector].zero = 0;
    idt[vector].type_attributes = type_attributes;
    idt[vector].offset_high = (handler_address >> 16) & 0xFFFF;
}

void idt_init(void) {
    __asm__ volatile("cli");
    memory_set(idt, 0, sizeof(idt));

    idt_pointer.limit = sizeof(idt) - 1;
    idt_pointer.base = (uint32_t)&idt;

    uint32_t handlers[32] = {
        (uint32_t)isr0,
        (uint32_t)isr1,
        (uint32_t)isr2,
        (uint32_t)isr3,
        (uint32_t)isr4,
        (uint32_t)isr5,
        (uint32_t)isr6,
        (uint32_t)isr7,
        (uint32_t)isr8,
        (uint32_t)isr9,
        (uint32_t)isr10,
        (uint32_t)isr11,
        (uint32_t)isr12,
        (uint32_t)isr13,
        (uint32_t)isr14,
        (uint32_t)isr15,
        (uint32_t)isr16,
        (uint32_t)isr17,
        (uint32_t)isr18,
        (uint32_t)isr19,
        (uint32_t)isr20,
        (uint32_t)isr21,
        (uint32_t)isr22,
        (uint32_t)isr23,
        (uint32_t)isr24,
        (uint32_t)isr25,
        (uint32_t)isr26,
        (uint32_t)isr27,
        (uint32_t)isr28,
        (uint32_t)isr29,
        (uint32_t)isr30,
        (uint32_t)isr31,
        (uint32_t)irq0,
        (uint32_t)irq1
    };

    for (uint8_t i = 0; i < 32; i++) {
        idt_set_gate(
            i,
            handlers[i],
            0x08,
            0x8E
        );
    }

    pic_remap(0x20, 0x28);

    outb(PIC1_DATA, 0xFF);
    outb(PIC2_DATA, 0xFF);

    pic_clear_mask(0);
    pic_clear_mask(1);

    idt_load((uint32_t)&idt_pointer);

    __asm__ volatile("sti");
}