 /******************************************************************************
 *
 * File Name:   UART_tasks.c
 *
 * Description: UART source file to initialize UART module tasks
 *
 * Date:        10/2/2020
 *
 ******************************************************************************/
#include <AutonomousControlSystem/inc/UART_tasks.h>



/* Declare Semaphore Handle for UART task */
SemaphoreHandle_t Sem_UARTReceive;
extern QueueHandle_t Queue_Feedback; /*New queue to pass the feedback value to usb to transmit*/
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

    uint8_t UART_Received_Frame[PACKET_SIZE]; // only receiving rotation feedback values

    static uint8_t index = 0;

    while(1)
    {
        xSemaphoreTake(Sem_UARTReceive,portMAX_DELAY);

        /* Check if there received char in the buffer or not */
        for(index=0;index<=3;index++)
        {
            UART_Received_Frame[index] = UARTCharGetNonBlocking(UART1_BASE); /* Get the received char */
        }
        UART_Received_Frame[PACKET_SIZE-1]='R';
        xQueueOverwrite(Queue_Feedback,(void *)UART_Received_Frame); //Queue Feedback passes the feedback data to transmit task in usb
        //State_Decoding (UART_Received_Frame, UART_MODULE);

    }
}
