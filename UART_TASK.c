/*
 * UART_TASK.c
 *
 *  Created on: 8 Oct 2019
 *      Author: okasha
 */

#include "UART_TASK.h"



/********************************************************************
 *                          Variables
 ********************************************************************/


/*****************************************************************
 *                       Semaphores
 *****************************************************************/


SemaphoreHandle_t Sem_UARTReceive;


/********************************************************************
 *                        Private Functions
 ********************************************************************/

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




/********************************************************************
 *                        Public Functions
 ********************************************************************/



void vInit_UART()
{

    Sem_UARTReceive=xSemaphoreCreateBinary();


    xTaskCreate( vTask_UART ,
                 "UART_task" ,                       // Name of the task for later debugging
                 UART_STACK_DEPTH ,                  // size of the stack.
                 NULL ,                      // task parameters.
                 configMAX_PRIORITIES-UART_vTASK_PRIO ,  // priority is inverted in freeRTOS.
                 NULL ) ;            // handle of the task.


}




void vTask_UART(void * param)
{
    char UART_Received_Char = 0 ;
    signed long Received_Data = 0;
    float f_Orientation = 0 ;
    static long point_flag  = 0 ;
    static signed long sign_flag = 1 ;
    /* Inspect the high water mark of the calling task when the task starts to
    execute. */

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
                /* if the received char is number update  received data */
                Received_Data =  f_UART_Decoding (UART_Received_Char ,  Received_Data  , point_flag);
            }

        }

    }

}
