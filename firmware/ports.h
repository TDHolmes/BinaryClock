#ifndef PORTS_H__
#define PORTS_H__

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
#define LEDDRV_SDI  PD3
#define LEDDRV_CLK  PD4
#define LEDDRV_OE   PD5
#define LEDDRV_LE   PD6
// No 7th port

#endif /* PORTS_H__ */
