

#ifndef EXAMPLES_UARTS_C_
#define EXAMPLES_UARTS_C_
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

#include "UARTs.h"




void UART0_Config()
{
    /*Initialize the Clock System */
      SysCtlClockSet(SYSCTL_USE_PLL|SYSCTL_SYSDIV_2_5|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

      /*Enable the Clock for PortF */
      SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

      /*Make Pin 1,2 as Output Pin */
      GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_2 | GPIO_PIN_1|GPIO_PIN_1);

      /* UART CONFIG */
      /*Enable clock for UART0*/
      SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
      /*Enable clock for port A*/
      SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

      /*UART initialization */
      /* Disable the UART until Configuration finish */
      //   UARTDisable(UART0_BASE);
      /* Set the BOUDRATE and transmitting FRAM*/
      UARTConfigSetExpClk(UART0_BASE,SysCtlClockGet(),9600,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE));
       /*Set the UART clock Source */
      UARTClockSourceSet(UART0_BASE,UART_CLOCK_SYSTEM);
      /*Enable UART */
      UARTEnable(UART0_BASE);


      /* Configure GPIO Port A pins 0 and 1 to be used as UART0. */
      GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

      /* Enable UART0 functionality on GPIO Port A pins 0 and 1. */

      GPIOPinConfigure(GPIO_PA0_U0RX);
      GPIOPinConfigure(GPIO_PA1_U0TX);

}
void UART0_SendString(const char * str)
{
    uint32_t i = 0 ;
     while(str[i] != 0)
        UART0_SendChr(str[i++]);
}

/* Function Name: UART_sendNumber
*  Fucntion description: Sends a number by converting its digits into a string
*
*/
void UART_sendNumber(uint32_t out)
{

        uint16_t c[10] = {0} ;
        uint16_t m;
        int_least32_t i = 0 ;

        if(out == 0)
        {
            c[0] = 48;
            i = 0 ;
        }
        while(out)
        {
            m = out%10;
            c[i++] = m + 48;
            out = out/10;
        }
        while(i != -1)
        {
            UART0_SendChr(c[i--]);
        }

}


void UART0_SendChr(char chr)
{
      while((UART0_FR_R&UART_FR_TXFF) != 0);
      UART0_DR_R = chr;
}

char UART0_ReadChr(void)
{
      while((UART0_FR_R&UART_FR_RXFE) != 0);
      return((char)(UART0_DR_R&0xFF));
}


#endif /* EXAMPLES_UARTS_C_ */
