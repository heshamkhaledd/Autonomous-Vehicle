
#ifndef EXAMPLES_UARTS_H_
#define EXAMPLES_UARTS_H_

#define UART0_DR_R              (*((volatile unsigned long *)0x4000C000))
#define UART0_FR_R              (*((volatile unsigned long *)0x4000C018))
#define UART_FR_TXFF            0x00000020  // UART Transmit FIFO Full
#define UART_FR_RXFE            0x00000010  // UART Receive FIFO Empty

#include <stdint.h>
/*Tivaware includes */
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"

/*This include for calling GPIO_Pin_Config()*/
#include "driverlib/pin_map.h"


/*      Macro to short the expression needed to print to debug port     */
#define DEBUG_WRITE(x) UART0_SendString(x)


/*      Macro for printing float numbers    */
#define _FTOI_I(X) (int32_t)(trunc(X)) /* to get the number before floating point */
#define _FTOI_F(X) (int32_t)fabs(trunc((X-trunc(X))*100)) /* to get the number after floating point */

/* public functions */
void UART0_Config();
void UART0_SendString(const char *);
void UART_sendNumber(uint32_t out);



#endif /* EXAMPLES_UARTS_H_ */
