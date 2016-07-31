#ifndef LEDS_DRVR_STP16CPC26_H__
#define LEDS_DRVR_STP16CPC26_H__

#include "ports.h"

#define HOUR_TENS_MASK (1 << HOUR_TEN_ANO)
#define HOUR_ONES_MASK (1 << HOUR_ONE_ANO)
#define MIN_TENS_MASK  (1 << MIN_TEN_ANO)
#define MIN_ONES_MASK  (1 << MIN_ONE_ANO)
#define SEC_TENS_MASK  (1 << SEC_TEN_ANO)
#define SEC_ONES_MASK  (1 << SEC_ONE_ANO)

/* row masks for RGB LEDs (three LEDs per bit) */
#define ROW_MASK_NINE   0b111000000111  /* bit 8     1 */
#define ROW_MASK_EIGHT  0b000000000111  /* bit 8       */
#define ROW_MASK_SEVEN  0b111111111000  /* bit   4 2 1 */
#define ROW_MASK_SIX    0b000111111000  /* bit   4 2   */
#define ROW_MASK_FIVE   0b111000111000  /* bit   4   1 */
#define ROW_MASK_FOUR   0b000000111000  /* bit   4     */
#define ROW_MASK_THREE  0b111111000000  /* bit     2 1 */
#define ROW_MASK_TWO    0b000111000000  /* bit     2   */
#define ROW_MASK_ONE    0b111000000000  /* bit       1 */

#endif /* LEDS_DRVR_STP16CPC26_H__ */
