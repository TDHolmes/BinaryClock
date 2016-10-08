#ifndef COLOR_H__
#define COLOR_H__

#include <stdint.h>

/*! \file color.h
    \brief Color related definitions and structures.
*/

#define COLOR_BITS 8 //!< Defines the amount of color resolution

//! structure to keep track of color information
typedef struct {
    uint8_t red;    /*! amount of red in the color.*/
    uint8_t green;  /*! amount of green in the color.*/
    uint8_t blue;   /*! amount of blue in the color.*/
    uint8_t color_bit;  /*! keeps track of what bit we're displaying.*/
} LED_color_t;

#endif /* COLOR_H__ */
