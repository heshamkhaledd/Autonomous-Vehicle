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

    /*Task variables*/
    float desiredOrientation =0;
    float currentOrientation = 0;
    float orientationDifference =0;
    float motorSteering = 0;
    long motorSteps = 0;

    PIDcontroller s_controller={0.8,0,0,0,0};

    while(1)
    {
        /*Receive orientation difference relative to current position*/ 
        xQueueReceive(Queue_Desired_Orientation,
                             &orientationDifference,
                             portMAX_DELAY);  

        /*Receive current orientation*/
        xQueueReceive(Queue_Current_Orientation,
                             &currentOrientation,
                             portMAX_DELAY);
        
        /*Calculate desired orientation*/
        desiredOrientation = getDesired (currentOrientation,orientationDifference);

        /*Apply PID control to recieved system inputs*/
        while(1)
        {
            /*check if a new set point is received*/
            if(xQueuePeek(Queue_Desired_Orientation,&orientationDifference,portMAX_DELAY))  
            {
                s_controller.lastError=0;
                s_controller.accumlativeError=0;
                break;
            }

            /*else keep applying PID control on old set point*/
            xQueueReceive(Queue_Current_Orientation,
                             &currentOrientation,
                             portMAX_DELAY);

            motorSteering = PID_control(&s_controller,desiredOrientation ,currentOrientation);
            motorSteps = (long)((motorSteering) / STEERING_STEP);
            xQueueOverwrite(Queue_steering,&motorSteps);

            /*Delay task to allow receiving new inputs to system and to allow motor task to be scheduled*/
             vTaskDelay(20);
        }

    }
}
