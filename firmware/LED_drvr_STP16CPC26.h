#ifndef LED_DRVR_STP16CPC26_H__
#define LED_DRVR_STP16CPC26_H__

#include "ports.h"
#include "LED.h"
#include "color.h"
#include <stdint.h>

/*! \file LED_drvr_STP16CPC26.h
    \brief Low level LED driver definitions to be used with the STP16CPC26 LED
    constant current driver.
*/


#define ROW_SIZE   4  //!< Number of rows on the binary clock
#define COL_SIZE   6  //!< Number of clumns on the binary clock
#define COLOR_SIZE 3  //!< Number of colors on the binary clock

#define RED_IND   0  //!< Offset of red LED on the PCB
#define BLUE_IND  1  //!< Offset of blue LED on the PCB
#define GREEN_IND 2  //!< Offset of green LED on the PCB


//! LED driver structure to keep track of pertinant LED variables.
typedef struct {
    uint8_t LED_array[ROW_SIZE][COL_SIZE][COLOR_SIZE];  //!< Array of the individual LEDs colors
    led_color_t colors;        //!< structure to keep track of the color to display
    uint8_t active_column;     //!< keeps track of column being displayed for multiplexing
    uint8_t active_color_bit;  //!< keeps track of the current color bit being displayed
} LED_drvr_t;


void LED_drvr_init(LED_drvr_t * LED_ptr);
void LED_drvr_run(LED_drvr_t *LED_ptr);

#endif /* LED_DRVR_STP16CPC26_H__ */
