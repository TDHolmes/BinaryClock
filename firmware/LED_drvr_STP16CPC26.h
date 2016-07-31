#ifndef LED_DRVR_STP16CPC26_H__
#define LED_DRVR_STP16CPC26_H__

#include "ports.h"
#include "LED.h"
#include <stdint.h>

#define HOUR_TENS_MASK (1 << HOUR_TEN_ANO)
#define HOUR_ONES_MASK (1 << HOUR_ONE_ANO)
#define MIN_TENS_MASK  (1 << MIN_TEN_ANO)
#define MIN_ONES_MASK  (1 << MIN_ONE_ANO)
#define SEC_TENS_MASK  (1 << SEC_TEN_ANO)
#define SEC_ONES_MASK  (1 << SEC_ONE_ANO)

/* row masks for RGB LED (three LED per bit) */
#define ROW_MASK_NINE   0b111000000111  /* bit 8     1 */
#define ROW_MASK_EIGHT  0b000000000111  /* bit 8       */
#define ROW_MASK_SEVEN  0b111111111000  /* bit   4 2 1 */
#define ROW_MASK_SIX    0b000111111000  /* bit   4 2   */
#define ROW_MASK_FIVE   0b111000111000  /* bit   4   1 */
#define ROW_MASK_FOUR   0b000000111000  /* bit   4     */
#define ROW_MASK_THREE  0b111111000000  /* bit     2 1 */
#define ROW_MASK_TWO    0b000111000000  /* bit     2   */
#define ROW_MASK_ONE    0b111000000000  /* bit       1 */


typedef struct {
    uint8_t LED_array[row][col][color];  // 4 rows, 6 columns, 3 colors
    LED_errors_t LED_errors;             // defined in LED.h
    uint8_t active_column;               // keeps track of column being displayed for multiplexing
    uint8_t active_color_bit;            // keeps track of the current color bit being displayed
} LED_drvr_t

#endif /* LED_DRVR_STP16CPC26_H__ */
