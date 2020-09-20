/*
 * PID_TASKS.c
 *
 *  Created on: 7 Oct 2019
 *      Author: okasha
 */


#include "PID_TASKS.h"
#include "UART_TASK.h"
#include "Orientation.h"



/***********************************************
 *                 Private Functions
 ***********************************************/


/***********************************************************************************
 *
 * Name : getDesired
 *
 * Purpose :  to make sure the car will not revolve around itself
 *
 * Parameters : current orientation , relative orientation .
 *
 * return :  desired orientation.
 ************************************************************************************/

static float getDesired (float current , float relative )
{
    float desired ;

    desired = current + relative ;
    /* Orientation Overflow */
    if ( ((long)desired) > 360)
    {
        desired = desired - 360 ;
    }
    else if ( ((long)desired) < 0)
    {
        desired = desired + 360 ;
    }
    return desired ;
}

/***********************************************
 *                 Global Functions
 ***********************************************/

void vInit_PID(){

    xTaskCreate( vTask_PID ,
                 "PID_task" ,                       // Name of the task for later debugging
                 PID_STACK_DEPTH ,                  // size of the stack.
                 NULL ,                      // task parameters.
                 configMAX_PRIORITIES-PID_vTASK_PRIO ,  // priority is inverted in freeRTOS.
                 NULL ) ;            // handle of the task.

}

void vTask_PID(void * para){

    float f_Desired_Orientation =0;
    float f_Current_Orientation = 0;
    float f_Refernece_Orientation =0;
    float f_Steering = 0;
    long l_Steps = 0;

    float Accumlative_Error =0;
    float Last_Error =0;

    long usb_flag = 0 ;
    /*     Inspect the high water mark of the calling task when the task starts to
    execute.
    PIDTaskHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
     */

    while(1){

           /* avoid handshacking */
           if (usb_flag == 0)
           {
               usb_flag = 1 ;
               xQueueReceive(Queue_Desired_Orientation,
                             &f_Refernece_Orientation,
                             portMAX_DELAY);
           }

           if (usb_flag == 1)
           {
               usb_flag = 2 ;

               xQueueReceive(Queue_Desired_Orientation,
                             &f_Refernece_Orientation,
                             portMAX_DELAY);

               f_Desired_Orientation = getDesired (f_Current_Orientation,f_Refernece_Orientation);
           }

           xQueueReceive(Queue_Current_Orientation,
                         &f_Current_Orientation,
                         0);

           f_Steering = f_DecodingOrientIntoSteering(f_Refernece_Orientation) ;

           l_Steps = (long)((f_Steering) / STEERING_STEP);

           xQueueOverwrite(Queue_steering,&l_Steps);

           do{
               if(xQueueReceive(Queue_Desired_Orientation,&f_Refernece_Orientation,0) == pdPASS){
                   break;
               }
               vTaskDelay(20);

               xQueueReceive(Queue_Current_Orientation,
                             &f_Current_Orientation,
                             portMAX_DELAY);

               f_Steering = f_PID_Steering(f_Desired_Orientation ,
                                           f_Current_Orientation ,
                                           &Accumlative_Error,
                                           &Last_Error);

               l_Steps = (long)((f_Steering) / STEERING_STEP);

               xQueueOverwrite(Queue_steering,&l_Steps);

           }while(1);

           f_Desired_Orientation = getDesired (f_Current_Orientation,f_Refernece_Orientation);

           Accumlative_Error = 0;
           Last_Error = 0;

       }

}






