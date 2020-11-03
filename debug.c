#include <stdint.h>
#include <stdbool.h>
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/rom_map.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_uart.h"
/******************************************************************************
 *
 * Function Name: UART0_init
 *
 * Description: A function to initialize UART0 module for debug purposes
 *
 * Note: To add this function to the code, define "DEBUG" in this file. 
 * Arguments:   void 
 * Return:      void
 * 
 *****************************************************************************/

    void UART0_init()
     {
               /* Clock Enable for the peripherals (UART1, GPIOB) */
        ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
        ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);


        ROM_IntMasterEnable();                /* Enable Processor Interrupts. */

        ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

        /* Configure the UART for 115,200, 8-N-1 operation. */
        ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), 115200 ,
                                (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                                    UART_CONFIG_PAR_NONE));

     }

/******************************************************************************
 *
 * Function Name: UART_sendNumber
 *
 * Description: A function to convert a decimal value to the ASCII value 
 *              and sends the string using UART0
 *
 * Note: To add this function to the code, define "DEBUG" in this file. 
 * Arguments:   sint32_t decimalValue 
 * Return:      void
 * 
 *****************************************************************************/
    void UART_sendNumber(int32_t decimalValue)
    {

        uint8_t c[10] = {0} ;
        uint8_t m;
        int8_t i = 0 ;
        uint8_t negativeFlag=0;

        if(decimalValue == 0)
        {
            c[0] = 48;
            i = 0 ;
        }
        if(decimalValue <0)
        {
            decimalValue *= -1;
            negativeFlag=1;
        }

        while(decimalValue)
        {
            m = decimalValue%10;

            c[i++] = m + 48;

            decimalValue = decimalValue/10;
        }
        if(negativeFlag == 1)
        {
            c[i++] = '-';
        }
        while(i != -1)
        {
            UARTCharPut (UART0_BASE, c[i--]);
        }
    }


