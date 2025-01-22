#include <stddef.h>
#include <stdint.h>
#include "terminal.h"
#include "types.h"
#include "io.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY (uint16_t*)0xB8000

#define VGA_PORT_INDEX 0x3D4
#define VGA_PORT_DATA  0x3D5

static size_t terminal_row;
static size_t terminal_column;
static VGA_Color2 terminal_color;
static uint16_t* terminal_buffer;

void terminal_initialize() {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = make_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK); // White on Black
    terminal_buffer = VGA_MEMORY;

    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = make_vga_entry(' ', terminal_color);
        }
    }
}

void terminal_setcolor(VGA_Color2 color) {
    terminal_color = color;
}

void terminal_putentryat(char c, VGA_Color2 color, size_t x, size_t y) {
    if (x >= VGA_WIDTH || y >= VGA_HEIGHT) {
        return; // Ignore out-of-bounds writes
    }
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = make_vga_entry(c, color);
}

void terminal_scroll() {
    // Shift all rows up by one
    for (size_t y = 1; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            terminal_buffer[(y - 1) * VGA_WIDTH + x] = terminal_buffer[y * VGA_WIDTH + x];
        }
    }

    // Clear the last row
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = make_vga_entry(' ', terminal_color);
    }
}

void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            terminal_row--;
            terminal_scroll();
        }
        return;
    }

    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);

    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            terminal_row--;
            terminal_scroll();
        }
    }
}

void terminal_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        terminal_putchar(data[i]);
    }
}

void terminal_writestring(const char* data) {
    while (*data) {
        terminal_putchar(*data++);
    }
}

void terminal_clear(VGA_Color2 color) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = color;
    for (uint16_t p = 0; p < (VGA_WIDTH * VGA_HEIGHT); p++) {
        terminal_buffer[p] = make_vga_entry(' ', color);
    }
}

void terminal_set_cursor_position(uint8_Vector2 position) {
    // Send the high byte (upper 8 bits) to port 0x3D5
    outb(VGA_PORT_INDEX, 0x0E); // Cursor high byte index
    outb(VGA_PORT_DATA, (position >> 8) & 0xFF);

    // Send the low byte (lower 8 bits) to port 0x3D5
    outb(VGA_PORT_INDEX, 0x0F); // Cursor low byte index
    outb(VGA_PORT_DATA, position & 0xFF);
}

uint8_Vector2 terminal_get_cursor_position() {
    uint16_t position = 0;

    // Read the high byte
    outb(VGA_PORT_INDEX, 0x0E); // Cursor high byte index
    position = inb(VGA_PORT_DATA) << 8;

    // Read the low byte
    outb(VGA_PORT_INDEX, 0x0F); // Cursor low byte index
    position |= inb(VGA_PORT_DATA);

    return (uint8_Vector2)position;
}
