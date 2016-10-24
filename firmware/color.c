#include <stdint.h>
#include "global_defines.h"
#include "color.h"
#include "hardware.h"

// Saves the colors to memory
void save_colors(LED_color_t *color_ptr)
{
    EEPROM_write((uint8_t)COLOR_ADDR, color_ptr->red);
    EEPROM_write((uint8_t)(COLOR_ADDR+1), color_ptr->green);
    EEPROM_write((uint8_t)(COLOR_ADDR+2), color_ptr->blue);
}


// Loads the colors from memory. Returns error if colors 
// don't exist in memory
retval_t load_colors(LED_color_t *color_ptr)
{
    color_ptr->red = EEPROM_read((uint8_t)(COLOR_ADDR));
    color_ptr->green = EEPROM_read((uint8_t)(COLOR_ADDR+1));
    color_ptr->blue = EEPROM_read((uint8_t)(COLOR_ADDR+2));
    return GEN_PASS;
}
