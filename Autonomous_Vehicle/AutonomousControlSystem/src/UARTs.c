

#ifndef EXAMPLES_UARTS_C_
#define EXAMPLES_UARTS_C_



#include "../inc/UARTs.h"




void UART0_Config()
{
    /*Initialize the Clock System */
  //  MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |SYSCTL_CFG_VCO_480), 120000000);


    SysCtlClockFreqSet((SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN),80000000);
    //SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
      /*Enable the Clock for PortF */
      SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

      /*Make Pin 1,2 as Output Pin */
      GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_3| GPIO_PIN_1|GPIO_PIN_1);

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
         UARTCharPut(UART0_BASE,str[i++]);
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
            UARTCharPut(UART0_BASE,c[i--]);
        }

}




#endif /* EXAMPLES_UARTS_C_ */
