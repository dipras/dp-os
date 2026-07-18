[org 0x7c00]

KERNEL_OFFSET equ 0x8000

; buffer hasil deteksi memori (e820), di low memory yang aman
; (tidak bentrok dengan boot sector di 0x7c00 atau kernel di 0x8000)
MMAP_COUNT   equ 0x1000 ; uint32_t: jumlah entry yang ditemukan
MMAP_ENTRIES equ 0x1004 ; array mmap_entry_t, masing-masing 24 byte

mov [BOOT_DRIVE], dl

; setup segment
xor ax, ax
mov ds, ax
mov es, ax
mov ss, ax
mov sp, 0x7C00

call detect_memory

; load kernel to 0x8000
mov bx, KERNEL_OFFSET
mov ah, 0x02
mov al, 38
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

; ------------------
; Deteksi memori via BIOS int 0x15, eax=0xE820
; ------------------
detect_memory:
  pusha

  mov di, MMAP_ENTRIES
  xor ebx, ebx    ; continuation value, 0 = mulai dari awal
  xor esi, esi    ; esi = jumlah entry yang ditemukan (32-bit)

.mmap_loop:
  mov eax, 0xE820
  mov edx, 0x534D4150 ; 'SMAP'
  mov ecx, 24
  int 0x15

  jc .mmap_done         ; carry set -> BIOS tidak dukung / error
  cmp eax, 0x534D4150
  jne .mmap_done         ; signature tidak cocok -> hasil tidak valid

  add di, 24
  inc esi

  test ebx, ebx
  jz .mmap_done          ; ebx == 0 -> ini entry terakhir

  cmp esi, 200            ; batas aman biar buffer tidak overflow
  jae .mmap_done

  jmp .mmap_loop

.mmap_done:
  mov [MMAP_COUNT], esi

  popa
  ret

BOOT_DRIVE db 0
error_msg db 'Disk Read Error', 0

times 510-($-$$) db 0
dw 0xAA55
