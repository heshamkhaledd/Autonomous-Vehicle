 /******************************************************************************
 *
 * File Name:   UART.c
 *
 * Description: UART source file to initialize UART module
 *
 * Date:        10/2/2020
 *
 ******************************************************************************/
#include <AutonomousControlSystem/inc/UART.h>

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
    if(UARTIntStatus(UART1_BASE,true) && UART_INT_OE !=0)
    {
        UARTRxErrorClear(UART1_BASE);
    }
    UARTIntClear(UART0_BASE, UART_INT_RX |UART_INT_OE );
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


    ROM_IntMasterEnable(); /* Enable Processor Interrupts. */

    GPIOPinConfigure(GPIO_PB0_U1RX);
    GPIOPinConfigure(GPIO_PB1_U1TX);
    ROM_GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

      /* Configure the UART for 115,200, 8-N-1 operation. */
    ROM_UARTConfigSetExpClk(UART1_BASE, ROM_SysCtlClockGet(), baud_rate,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                            UART_CONFIG_PAR_NONE));

    /* Enable the UART interrupt. */
    ROM_IntEnable(INT_UART1);
    ROM_UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_OE /*| UART_INT_RT*/);
    ROM_UARTFIFOLevelSet(UART1_BASE, UART_FIFO_TX2_8, UART_FIFO_RX2_8); //trying to generate 1 interrupt/4 bytes of data
}
