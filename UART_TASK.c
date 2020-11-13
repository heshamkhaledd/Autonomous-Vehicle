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
            if (UART_Received_Frame[index] == 'r' || UART_Received_Frame[index] == 'R')
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
