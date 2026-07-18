bits 32

extern irq_handler

global irq0
global irq1

irq0:
    cli

    push byte 32
    push byte 0

    jmp irq_common_stub

irq1:
    cli

    push byte 33
    push byte 1

    jmp irq_common_stub

irq_common_stub:
    pusha

    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax

    push esp
    call irq_handler
    add esp, 4

    pop gs
    pop fs
    pop es
    pop ds

    popa

    add esp, 8

    iret