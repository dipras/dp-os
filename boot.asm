[org 0x7c00]

KERNEL_OFFSET equ 0x8000

mov [BOOT_DRIVE], dl

; setup segment
xor ax, ax
mov ds, ax
mov es, ax
mov ss, ax
mov sp, 0x7C00

; load kernel to 0x8000
mov bx, KERNEL_OFFSET
mov ah, 0x02
mov al, 1
mov ch, 0
mov cl, 2
mov dh, 0
mov dl, [BOOT_DRIVE]
int 0x13

jc disk_error

jmp 0x000:KERNEL_OFFSET

disk_error:
  mov si, error_msg
  call print_string
  jmp $

print_string:
  lodsb
  cmp al, 0
  je .done
  mov ah, 0x0E
  int 0x10
  jmp print_string

.done:
  ret

BOOT_DRIVE db 0
error_msg db 'Disk Read Error', 0

times 510-($-$$) db 0
dw 0xAA55
