#ifndef PORTS_H__
#define PORTS_H__

#include <avr/io.h>

/****    PORT A    ****/
#define IND_LED0     PA0
#define RTC_SQW      PA1
// Pin PA2 is reset input

/****    PORT B    ****/
#define HOUR_TEN_ANO PB0
#define HOUR_ONE_ANO PB1
#define MIN_TEN_ANO  PB2
#define MIN_ONE_ANO  PB3
#define SEC_TEN_ANO  PB4
#define RTC_SDA      PB5
#define SEC_ONE_ANO  PB6
#define RTC_SCL      PB7

/****    PORT D    ***/
#define UART_RXD    PD0
#define UART_TXD    PD1
#define IND_LED1    PD2
// #define LEDDRV_SDI  PD3
// #define LEDDRV_CLK  PD4
// #define LEDDRV_OE   PD5
// #define LEDDRV_LE   PD6
// No 7th port

#define LEDDRV_SDI PD3
#define LEDDRV_CLK PD4
#define LEDDRV_OE PD5
#define LEDDRV_LE PD6

#define ANO_PORT PORTB

#define LEDDRV_OE_SET(val)  (val ? ANO_PORT |= (1 << LEDDRV_OE) : (ANO_PORT &= ~(1 << LEDDRV_OE)))
#define LEDDRV_LE_SET(val)  (val ? ANO_PORT |= (1 << LEDDRV_LE) : (ANO_PORT &= ~(1 << LEDDRV_LE)))
#define LEDDRV_SDI_SET(val) (val ? ANO_PORT |= (1 << LEDDRV_SDI) : (ANO_PORT &= ~(1 << LEDDRV_SDI)))
#define LEDDRV_CLK_SET(val) (val ? ANO_PORT |= (1 << LEDDRV_CLK) : (ANO_PORT &= ~(1 << LEDDRV_CLK)))

#define LED0_set(val) (val ? (PORTA |= 1 << IND_LED0) : (PORTA &= ~(1 << IND_LED0)))
#define LED1_set(val) (val ? (PORTD |= 1 << IND_LED1) : (PORTD &= ~(1 << IND_LED1)))

#endif /* PORTS_H__ */
