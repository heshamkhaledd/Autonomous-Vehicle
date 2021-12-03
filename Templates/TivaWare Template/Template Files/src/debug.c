#include <Template Files/inc/debug.h>

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

        /* Enable Processor Interrupts. */
        ROM_IntMasterEnable();

        ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

        /* Configure the UART for 115,200, 8-N-1 operation. */
        ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), 115200 ,
                               (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                                UART_CONFIG_PAR_NONE));
     }

/*****************************************************************************/
/*                                 UART Functions                            */
/*****************************************************************************/

/*
 * The reason in the next two UART functions, we added the ui32Base argument
 * is because it would allow those functions to be generic to all UART modules
 * in our MC.
 */

    void UART_receiveString(uint32_t ui32Base, uint8_t *Str)
    {
        /* Counter Declaration to be used to loop until string is sent. */
        uint8_t i = 0;

        /* Receive the first character to use it as initial sentinel for the loop. */
        Str[i] = UARTCharGet (ui32Base);

        /* Receive characters until the ENTER key is pressed. */
        /* 13 is the ASCII equivalent for the ENTER button. */
        while(Str[i] != 13)
        {
            /* Receive the next character that follows the initial sentinel we used. */
            i++;
            Str[i] = UARTCharGet (ui32Base);
        }
        /* Enter the NULL at the end, so the string would be valid. */
        Str[i] = '\0';
    }

    void UART_sendString(uint32_t ui32Base, const uint8_t *Str)
    {
        /* Counter Declaration to be used to loop until string is sent. */
        uint8_t i = 0;

        while(Str[i] != '\0')
        {
            /* Send the characters one by one. */
            UARTCharPut(ui32Base, Str[i]);
            i++;
        }
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
