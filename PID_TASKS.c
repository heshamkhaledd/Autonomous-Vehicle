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

/*
    TODO::    -Make sure of the discontinuity point
              -change ORIENT_TO_STEERING_PARAM
*/

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
 *              This function assumes that the input in both the feedback queue and
 *              the desired orientation queue ranges between -180 and 180.
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
    float pidOrientationOutput = 0;
    long motorSteering = 0;
    WRAP_AROUND_FLAG wrapAroundFlag=-1;

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
        
        /*Calculate desired orientation relative to current orientation and check if a wrap around occurs*/
        desiredOrientation = currentOrientation + orientationDifference;
        wrapAroundFlag=int8_getOrientationWrapAroundFlag(currentOrientation,desiredOrientation);

        /*Apply PID control to recieved system inputs*/
        while(1)
        {
            /*check if a new set point is received*/
            float newOrientationDifference;
            if(xQueuePeek(Queue_Desired_Orientation,&newOrientationDifference,portMAX_DELAY))  
                if(abs(orientationDifference-newOrientationDifference)>ERROR_FACTOR)
                {
                    s_controller.lastError=0;
                    s_controller.accumlativeError=0;
                    break;
                }

            /*else keep applying PID control on old set point*/
            xQueueReceive(Queue_Current_Orientation,
                             &currentOrientation,
                             portMAX_DELAY);

            /*adjust desired orientation if wrap around happens*/
            if(wrapAroundFlag != -1)
                v_adjustDesiredOrientaion(wrapAroundFlag,currentOrientation,&desiredOrientation);
            
            pidOrientationOutput=f_PID_control(&s_controller,desiredOrientation ,currentOrientation);
            
            /*Change orientation degrees to motor steering steps and checks for direction*/
            motorSteering=f_DecodeOrientationIntoSteering(pidOrientationOutput);
            xQueueOverwrite(Queue_steering,&motorSteering);

            /*Delay task to allow receiving new inputs to system and to allow motor task to be scheduled*/
             vTaskDelay(20);
        }

    }
}
