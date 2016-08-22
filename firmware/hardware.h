#ifndef HARDWARE_H__
#define HARDWARE_H__

#include <stdint.h>
#include "global_defines.h"

#define ON  1
#define OFF 0

#define F_OSC 8000000UL

retval_t hardware_init(void);

#endif /* HARDWARE_H__ */
