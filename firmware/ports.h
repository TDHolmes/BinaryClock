#ifndef PORTS_H__
#define PORTS_H__

#include <avr/io.h>

/*! \file ports.h
    \brief Defines what pins are connected to and defines useful macros for setting certain
    pins.
*/

//! PORT A PIN DEFINITIONS
#define IND_LED0     PA0
#define RTC_SQW      PA1
// Pin PA2 is reset input

//! PORT B PIN DEFINITIONS
#define HOUR_TEN_ANO PB0
#define HOUR_ONE_ANO PB1
#define MIN_TEN_ANO  PB2
#define MIN_ONE_ANO  PB3
#define SEC_TEN_ANO  PB4
#define RTC_SDA      PB5
#define SEC_ONE_ANO  PB6
#define RTC_SCL      PB7

//! PORT D PIN DEFINITIONS
#define UART_RXD    PD0
#define UART_TXD    PD1
#define IND_LED1    PD2
#define LEDDRV_SDI  PD3
#define LEDDRV_CLK  PD4
#define LEDDRV_OE   PD5
#define LEDDRV_LE   PD6
// no 7th pin on the port

#define ANO_PORT PORTB  //!< The annode of the LEDs is on port B

//! Macro to set the LED Driver output enable pin to either high or low
#define LEDDRV_OE_SET(val)  (val ? PORTD |= (1 << LEDDRV_OE) : (PORTD &= ~(1 << LEDDRV_OE)))
//! Macro to set the LED Driver latch enable pin to either high or low
#define LEDDRV_LE_SET(val)  (val ? PORTD |= (1 << LEDDRV_LE) : (PORTD &= ~(1 << LEDDRV_LE)))
//! Macro to set the LED Driver serial data in pin to either high or low
#define LEDDRV_SDI_SET(val) (val ? PORTD |= (1 << LEDDRV_SDI) : (PORTD &= ~(1 << LEDDRV_SDI)))
//! Macro to set the LED Driver clock pin to either high or low
#define LEDDRV_CLK_SET(val) (val ? PORTD |= (1 << LEDDRV_CLK) : (PORTD &= ~(1 << LEDDRV_CLK)))

//! Macro to set the debug LED 0 pin to either high or low
#define LED0_set(val) (val ? (PORTA |= 1 << IND_LED0) : (PORTA &= ~(1 << IND_LED0)))
//! Macro to set the debug LED 1 pin to either high or low
#define LED1_set(val) (val ? (PORTD |= 1 << IND_LED1) : (PORTD &= ~(1 << IND_LED1)))
//! Macro to toggle the debug LED 0 pin
#define LED0_toggle() (PORTA ^= (1 << IND_LED0))
//! Macro to toggle the debug LED 1 pin
#define LED1_toggle() (PORTD ^= (1 << IND_LED1))

#endif /* PORTS_H__ */
