[bits 16]
global _start

_start:

cli

lgdt [gdt_descriptor]

mov eax, cr0
or eax, 1
mov cr0, eax

jmp 0x08:protected_mode_start

[bits 32]
[extern main]

protected_mode_start:

mov ax, 0x10
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax

mov esp, 0x90000

call main

jmp $

; ------------------
; GDT
; ------------------

gdt_start:

gdt_null:
    dq 0

gdt_code:
    dq 0x00CF9A000000FFFF

gdt_data:
    dq 0x00CF92000000FFFF

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start
