 /******************************************************************************
 *
 * File Name:   UART.c
 *
 * Description: UART source file to initialize UART module
 *
 * Date:        10/2/2020
 *
 ******************************************************************************/
#include "UART.h"


#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"


long pUART_Received_Char;


/******************************************************************************
 *
 * Function Name: UART1IntHandler
 *
 * Description: UART1 ISR Handler. Triggered when UART receives
 *              any data on the receive buffer.
 *
 * Arguments:   void
 * Return:      void
 *
 *****************************************************************************/
void UART1IntHandler(void){

    UARTIntClear(UART1_BASE, UART_INT_RX | UART_INT_RT);
    xSemaphoreGiveFromISR(Sem_UARTReceive,NULL);/* Please use "FromISR" for freeRTOS functions used in any ISR */

}


/******************************************************************************
 *
 * Function Name: UART1_Init
 *
 * Description: Responsible for initializing UART1 Hardware.
 *
 *
 * Arguments:   void
 * Return:      void
 *
 *****************************************************************************/
void UART1_Init(int baud_rate)
{

      /* Clock Enable for the peripherals (UART1, GPIOB) */
      ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
      ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);


      ROM_IntMasterEnable();                /* Enable Processor Interrupts. */

      GPIOPinConfigure(GPIO_PB0_U1RX);
      GPIOPinConfigure(GPIO_PB1_U1TX);
      ROM_GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

      /* Configure the UART for 115,200, 8-N-1 operation. */
       ROM_UARTConfigSetExpClk(UART1_BASE, ROM_SysCtlClockGet(), baud_rate ,
                               (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                                UART_CONFIG_PAR_NONE));

       /* Enable the UART interrupt. */
       ROM_IntEnable(INT_UART1);
       ROM_UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);
}
