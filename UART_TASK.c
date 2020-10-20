 /******************************************************************************
 *
 * File Name:   UART_TASK.c
 *
 * Description: UART source file to initialize UART module tasks
 *
 * Date:        10/2/2020
 *
 ******************************************************************************/

#include "UART_TASK.h"
#include "State_Decode.h"


/* Declare Semaphore Handle for UART task */
SemaphoreHandle_t Sem_UARTReceive;

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
    xSemaphoreGiveFromISR(Sem_UARTReceive,NULL);        /* Please use "FromISR" for freeRTOS functions used in any ISR */
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


/******************************************************************************
 *
 * Function Name: vInit_UART
 *
 * Description: Function to create the UART task semaphore and UART Feedback Task
 *
 * Arguments:   void
 * Return:      void
 *
 *****************************************************************************/
void vInit_UART(void)
{
    
    UART1_Init (BAUD_RATE); /* Initialize UART1 Hardware */

    /* Create UART Task Semaphore and store its address in Sem_UARTReceive */
    Sem_UARTReceive= xSemaphoreCreateBinary();

    xTaskCreate( vTask_UART,                            /* Task Address       */
                 "UART_task",                           /* Task name          */
                 UART_STACK_DEPTH,                      /* Size of the stack. */
                 NULL,                                  /* Task Parameters.   */
                 configMAX_PRIORITIES-UART_vTASK_PRIO,  /* Task Priority .    */
                 NULL);                                 /* Task handle        */
}

/******************************************************************************
 *
 * Function Name: vInit_UART
 *
 * Description: UART Task that's responsible for receiving the Orientation Data
 *              and store each character in a software buffer (UART_Received_Frame)
 *
 * Arguments:   void *pvParameters
 * Return:      void
 *
 *****************************************************************************/
void vTask_UART(void *pvParameters)
{

    uint8_t UART_Received_Frame[arr_size];

    static uint8_t index = 0;

    while(1)
    {
        xSemaphoreTake(Sem_UARTReceive,portMAX_DELAY);

        /* Check if there received char in the buffer or not */
        while (UARTCharsAvail(UART1_BASE))
        {
            UART_Received_Frame[index] = UARTCharGet(UART1_BASE); /* Get the received char */

            /* If the received character is a letter. The frame is completed, we must start decoding it */
            if (UART_Received_Frame[index] == 'o' || UART_Received_Frame[index] == 'O')
            {
                UART_Received_Frame[++index] = '\0';
                break;
            }

            index++;
        }

        State_Decoding (UART_Received_Frame, UART_MODULE);
        index = 0;
    }
}
