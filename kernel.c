#include "terminal.h"
#include "types.h"

void kernel_main() {
    terminal_initialize();
    terminal_clear(make_color(VGA_COLOR_BLUE, VGA_COLOR_CYAN));
    terminal_writestring("Hello World!");
    terminal_set_cursor_position(make_uint8_vector2(0, 0));
}
