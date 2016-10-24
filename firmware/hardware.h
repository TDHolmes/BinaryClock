#ifndef HARDWARE_H__
#define HARDWARE_H__

#include <stdint.h>
#include "global_defines.h"

/*! \file hardware.h
    \brief Hardware related definitions (e.g. oscillator speed).
*/

#define ON  1 //!< defines the on state
#define OFF 0 //!< defines the off state

#define F_OSC 8000000UL //!< oscillator speec

retval_t hardware_init(void);

// EEPROM functions
void EEPROM_write(uint8_t address, uint8_t data_to_write);
uint8_t EEPROM_read(uint8_t address_to_read);

#endif /* HARDWARE_H__ */
