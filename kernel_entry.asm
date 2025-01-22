; Multiboot header in Assembly (place this in kernel_entry.asm)
section .multiboot
align 4
dd 0x1BADB002               ; Magic number
dd 0                       ; Flags (set to 0 for basic Multiboot compliance)
dd -(0x1BADB002 + 0)       ; Checksum (magic + flags + checksum must equal 0)

section .text
global _start
_start:
    [bits 32]
    [extern kernel_main]
    call kernel_main
    jmp $