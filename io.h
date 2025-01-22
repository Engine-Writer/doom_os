#ifndef IO_H
#define IO_H

#include <stdint.h>

// Inline static function to read a byte from a port
static inline uint8_t inb(uint16_t port) {
    uint8_t result;
    __asm__ __volatile__ (
        "inb %1, %0"
        : "=a"(result)
        : "d"(port)
    );
    return result;
}

// Inline static function to write a byte to a port
static inline void outb(uint16_t port, uint8_t value) {
    __asm__ __volatile__ (
        "outb %0, %1"
        : 
        : "a"(value), "d"(port)
    );
}

#endif // IO_H
