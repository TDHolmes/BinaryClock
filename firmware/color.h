#ifndef COLOR_H__
#define COLOR_H__

#include <stdint.h>
#include "global_defines.h"

/*! \file color.h
    \brief Color related definitions and structures.
*/

#define COLOR_BITS 8 //!< Defines the amount of color resolution
#define COLOR_ADDR 0 //!< starting address in EEPROM where color info is saved

//! structure to keep track of color information
typedef struct {
    uint8_t red;    /*! amount of red in the color.*/
    uint8_t green;  /*! amount of green in the color.*/
    uint8_t blue;   /*! amount of blue in the color.*/
    uint8_t color_bit;  /*! keeps track of what bit we're displaying.*/
} LED_color_t;

void save_colors(LED_color_t *color_ptr);
retval_t load_colors(LED_color_t *color_ptr);

#endif /* COLOR_H__ */
