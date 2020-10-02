 /******************************************************************************
 *
 * File Name:   PID_TASKS.c
 *
 * Description: PID source file to initialize and start the PID Control tasks.
 *
 * Date:        10/2/2020
 *
 ******************************************************************************/
#include "PID_TASKS.h"
#include "UART_TASK.h"
#include "Orientation.h"


/******************************************************************************
 *
 * Function Name: getDesired
 *
 * Description: Responsible for referencing the orientation angle to
 *              the vehicle's absolute angle. Also, normalizing the
 *              Orientation angle to be < 360 degrees.
 *
 * Arguments:   float current , float relative
 * Return:      float desired
 *
 *****************************************************************************/
static float getDesired (float current , float relative)
{
    float desired ;

    desired = current + relative;

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

/******************************************************************************
 *
 * Function Name: vInit_PID
 *
 * Description: Responsible for creating the PID Control Task in the FreeRTOS
 *              MicroKernel.
 *
 * Arguments:   void
 * Return:      void
 *
 *****************************************************************************/
void vInit_PID(void){

    xTaskCreate( vTask_PID,                            /* Task Address       */
                 "PID_task",                           /* Task name          */
                 PID_STACK_DEPTH,                      /* Size of the stack. */
                 NULL,                                 /* Task Parameters.   */
                 configMAX_PRIORITIES-PID_vTASK_PRIO,  /* Task Priority .    */
                 NULL);                                /* Task handle        */
}


/******************************************************************************
 *
 * Function Name: vTask_PID
 *
 * Description: PID Control Task. Responsible for getting the current vehicle's
 *              orientation and adjust the stepper steps according to the
 *              feedback.
 *
 * Arguments:   void *pvParameters
 * Return:      void
 *
 *****************************************************************************/
void vTask_PID(void * pvParameters){

    float f_Desired_Orientation =0;
    float f_Current_Orientation = 0;
    float f_Refernece_Orientation =0;
    float f_Steering = 0;
    long l_Steps = 0;

    float Accumlative_Error =0;
    float Last_Error =0;

    long usb_flag = 0 ;

    while(1){

           /* Avoid Handshacking */
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
