
#ifndef EXAMPLES_UARTS_H_
#define EXAMPLES_UARTS_H_

#define UART0_DR_R              (*((volatile unsigned long *)0x4000C000))
#define UART0_FR_R              (*((volatile unsigned long *)0x4000C018))
#define UART_FR_TXFF            0x00000020  // UART Transmit FIFO Full
#define UART_FR_RXFE            0x00000010  // UART Receive FIFO Empty

#include <stdint.h>

/* public functions */
void UART0_Config();
void UART0_SendString(const char *);
void UART0_SendInt(uint8_t);
void UART0_SendChr(char);
char UART0_ReadChr(void);


#endif /* EXAMPLES_UARTS_H_ */
