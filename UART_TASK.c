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



/* Declare Semaphore Handle for UART task */
SemaphoreHandle_t Sem_UARTReceive;


/******************************************************************************
 *
 * Function Name: f_UART_Decoding
 *
 * Description: Responsible for converting UART Feedback data from ASCII to Decimal
 *
 * Arguments:   char UART_Received_Char , float Decoded_Data  , long point_flag
 * Return:      float Decoded_Data
 *
 *****************************************************************************/
static float f_UART_Decoding (char UART_Received_Char , float Decoded_Data  , long point_flag)
{
    static long counter = 0 ;
    if ( point_flag == 0)
    {
        counter = 0 ;
        Decoded_Data = Decoded_Data * 10 ;
        Decoded_Data = UART_Received_Char - Numbers_Ascii_Base ; /* convert number from ASCII to Decimal */
        Decoded_Data = Decoded_Data + (float)UART_Received_Char ;
        return Decoded_Data ;
    }
    else
    {
        counter ++ ;
        Decoded_Data = Decoded_Data - Numbers_Ascii_Base ; /* convert number from ASCII to Decimal */
        Decoded_Data = Decoded_Data + (float)(Decoded_Data/ ( 10 * counter )) ;
        return Decoded_Data ;
    }

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
 * Description: UART Task that's responsible for receiving the Orientation data
 *              decode it, and push it to the appropriate queue.
 *
 * Arguments:   void *pvParameters
 * Return:      void
 *
 *****************************************************************************/
void vTask_UART(void *pvParameters)
{
    char UART_Received_Char = 0 ;
    signed long Received_Data = 0;
    float f_Orientation = 0 ;
    static long point_flag  = 0 ;
    static signed long sign_flag = 1 ;

    while(1){
        xSemaphoreTake(Sem_UARTReceive,portMAX_DELAY);
        while (UARTCharsAvail(UART1_BASE)){ /* Check if there received char in the buffer or not */

            UART_Received_Char = UARTCharGet(UART1_BASE); /* get Received char and decode it */

            if(((UART_Received_Char) > (Numbers_Ascii_Base + Numbers_Ascii_Offset)) || ((UART_Received_Char) < (Numbers_Ascii_Base)))
            {
                /*  which motor we will modify its current angle or if a negative angle */
                switch (UART_Received_Char){
                /* O for Orientation */
                case 'o' :
                case 'O' :
                    f_Orientation = (float) Received_Data * sign_flag ;
                    //Queue send Desired Orientation
                    xQueueOverwrite(Queue_Current_Orientation, &f_Orientation);
                    /* back to default to receive new data */
                    Received_Data = 0;
                    sign_flag = 1 ;
                    point_flag = 0 ;
                    break;

                case 'k' :
                case 'K' :
                    /* back to default to receive new data */
                                       Received_Data = 0;
                                       sign_flag = 1 ;
                                       point_flag = 0 ;
                    break;
                case 'l' :
                case 'L' :
                    /* back to default to receive new data */
                                       Received_Data = 0;
                                       sign_flag = 1 ;
                                       point_flag = 0 ;
                    break;
                case 'm' :
                case 'M' :
                    /* back to default to receive new data */
                                       Received_Data = 0;
                                       sign_flag = 1 ;
                                       point_flag = 0 ;
                    break;
                case 'n' :
                case 'N' :
                    /* back to default to receive new data */
                                       Received_Data = 0;
                                       sign_flag = 1 ;
                                       point_flag = 0 ;
                    break;
                case 'p' :
                case 'P' :
                    /* back to default to receive new data */
                                       Received_Data = 0;
                                       sign_flag = 1 ;
                                       point_flag = 0 ;
                    break;

                case 'q' :
                case 'Q' :
                    /* back to default to receive new data */
                                       Received_Data = 0;
                                       sign_flag = 1 ;
                                       point_flag = 0 ;
                    break;

                case '.' : /* handling floating numbers */
                    point_flag = 1 ;
                    break ;

                case '-' : /* handling negative numbers */
                    sign_flag = -1 ;
                    /* unexpected Received data Case */
                default :
                   /* handling unexpected data */
                    break;
                }
            }
            else
            {
                /* if the received char is number, update received data */
                Received_Data =  f_UART_Decoding (UART_Received_Char ,  Received_Data  , point_flag);
            }
        }
    }
}
