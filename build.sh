#!/bin/bash

nasm boot.asm -f bin -o boot.bin
nasm kernel_entry.asm -f bin -o kernel.bin

cat boot.bin kernel.bin > os-image.bin

qemu-system-x86_64 -drive format=raw,file=os-image.bin
