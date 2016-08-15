#ifndef COLOR_H__
#define COLOR_H__

#include <stdint.h>

#define COLOR_BITS 8

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t color_bit;
} led_color_t;

#endif /* COLOR_H__ */
