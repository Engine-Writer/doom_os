#!/bin/bash
export PATH=$PATH:/usr/local/i386elfgcc/bin

# Assemble bootloader
nasm "boot.asm" -f bin -o "bin/boot.bin"



# Assemble kernel entry
nasm "kernel_entry.asm" -f elf -o "bin/kernel_entry.o"

# Compile kernel C code
i386-elf-gcc -ffreestanding -m32 -g -c "kernel.c" -o "bin/kernel.o"
i386-elf-gcc -ffreestanding -m32 -g -c "terminal.c" -o "bin/terminal.o"

# Link kernel
i386-elf-ld -o "bin/full_kernel.elf" -Ttext 0x1000 "bin/kernel_entry.o" "bin/kernel.o" "bin/terminal.o"

# Create ISO
cp "bin/full_kernel.elf" iso/boot/
grub-mkrescue -o "bin/OS.iso" iso/

# Run in QEMU
qemu-system-i386 -cdrom "bin/OS.iso" -m 64 -boot d