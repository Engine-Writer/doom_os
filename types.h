#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
typedef uint16_t uint8_Vector2;

static inline uint8_Vector2 make_uint8_vector2(uint8_t x, uint8_t y) {
    return (uint8_t)x | (uint8_t)y << 8;
}
#endif