#!/bin/bash

# 1. Compile bootloader menjadi raw binary (tetap tidak berubah)
nasm boot.asm -f bin -o boot.bin

# 2. Compile kernel_entry menjadi format ELF 32-bit (bukan bin!)
nasm kernel_entry.asm -f elf32 -o kernel_entry.o
nasm kernel/arch/x86/idt_load.asm -f elf32 -o idt_load.o
nasm kernel/arch/x86/isr.asm -f elf32 -o isr_asm.o
nasm kernel/arch/x86/irq.asm -f elf32 -o irq_asm.o

# 3. Compile kernel.c menjadi format object 32-bit (tanpa standard library)
gcc -m32 -ffreestanding -fno-pie -fno-pic -fno-stack-protector -c kernel/kernel.c -o kernel.o
gcc -m32 -ffreestanding -fno-pie -fno-pic -fno-stack-protector -c kernel/drivers/vga.c -o vga.o
gcc -m32 -ffreestanding -fno-pie -fno-pic -fno-stack-protector -c kernel/lib/string.c -o string.o
gcc -m32 -ffreestanding -fno-pie -fno-pic -fno-stack-protector -c kernel/drivers/keyboard.c -o keyboard.o
gcc -m32 -ffreestanding -fno-pie -fno-pic -fno-stack-protector -c kernel/terminal/terminal.c -o terminal.o
gcc -m32 -ffreestanding -fno-pie -fno-pic -fno-stack-protector -c kernel/arch/x86/idt.c -o idt.o
gcc -m32 -ffreestanding -fno-pie -fno-pic -fno-stack-protector -c kernel/arch/x86/isr.c -o isr.o
gcc -m32 -ffreestanding -fno-pie -fno-pic -fno-stack-protector -c kernel/arch/x86/irq.c -o irq.o
gcc -m32 -ffreestanding -fno-pie -fno-pic -fno-stack-protector -c kernel/arch/x86/pic.c -o pic.o
gcc -m32 -ffreestanding -fno-pie -fno-pic -fno-stack-protector -c kernel/arch/x86/mmap.c -o mmap.o


# 4. Jahit (Link) kernel_entry.o dan kernel.o menjadi raw binary di offset 0x8000
ld -m elf_i386 -T linker.ld -o kernel.bin kernel_entry.o kernel.o vga.o terminal.o keyboard.o string.o idt_load.o idt.o isr_asm.o isr.o irq_asm.o irq.o pic.o mmap.o --oformat binary

# 5. Gabungkan bootloader dan kernel menjadi OS image
cat boot.bin kernel.bin > os-image.bin
truncate -s 20k os-image.bin

# 6. Bersihkan file object sementara (Opsional agar direktori tetap rapi)
rm *.o

# 7. Jalankan di QEMU
qemu-system-x86_64 -drive format=raw,file=os-image.bin
