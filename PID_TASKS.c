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


/******************************************************************************
 *
 * Function Name: getDesiredOrientation
 *
 * Description: Responsible for referencing the orientation angle to
 *              the vehicle's absolute angle. 
 *              Also, normalizes the orientation angle to be 0< angle< 360 
 *              so that PID calculations are all positive
 *
 * Arguments:   float current , float relative
 * Return:      float desired
 *
 *****************************************************************************/
static float getDesiredOrientation(float current , float relative)
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
 * Function Name: Steering_Saturation
 *
 * Description: Checks if the Steering output is adjusted to the max/min motor steering
 *              to be within the acceptable steering range.
 *
 * Arguments:   float a_SteeringOutput
 * Return:      float Max_Steering || float Steering_Output
 *
 *****************************************************************************/
static float Steering_Saturation (float a_SteeringOutput)
{
    a_SteeringOutput=a_SteeringOutput>MAX_STEERING ? MAX_STEERING:a_SteeringOutput;
    a_SteeringOutput=a_SteeringOutput<MIN_STEERING ? MIN_STEERING:a_SteeringOutput;

    return a_SteeringOutput ;
}

/***********************************************************************************
 *
 * Name : f_DecodingOrientIntoSteering
 *
 * Description: This function aims to decode or get the steering angle to be excuted from
 *              the desired orientation.
 *              It also checks 
 *              There are many approximations according to field experiments as maximum steering
 *              we can get from stepper is 360 degree which is almost equvilant to 45 degree
 *              orientation wise.
 *              The decoding process depends on factor (ORIENT_TO_STEERING_PARAM) to map from
 *              orientation into steering and this factor also hold ratio of losses in mechanical
 *              system during excuting steering.
 *
 * Arguments:   float f_Desired_Orientation
 *
 * Return:      float Steering_Degrees
 *
 ************************************************************************************/
/*TODO:: change ORIENT_TO_STEERING_PARAM*/
float f_DecodeOrientationIntoSteering (float f_Desired_Orientation )
{
    float steeringDegrees ;

    if ( (long) f_Desired_Orientation > 180)
    {
        steeringDegrees = (((float)ORIENT_TO_STEERING_PARAM ) * (f_Desired_Orientation - OVERLAP_CORRECTION_FACTOR )) ;
    }
    else if ( (long) f_Desired_Orientation < -180)
    {
        steeringDegrees = (((float)ORIENT_TO_STEERING_PARAM ) * (f_Desired_Orientation + OVERLAP_CORRECTION_FACTOR )) ;
    }
    else
    {
        steeringDegrees = (((float)ORIENT_TO_STEERING_PARAM ) * (f_Desired_Orientation )) ;
    }

    steeringDegrees=Steering_Saturation(steeringDegrees);
    return steeringDegrees ;
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
    float pidOrientationOutput = 0;
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
        
        /*Calculate desired orientation from current orientation and orientation difference*/
        desiredOrientation = getDesiredOrientation (currentOrientation,orientationDifference);

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

            pidOrientationOutput=f_PID_control(&s_controller,desiredOrientation ,currentOrientation);
            
            /*Change orientation degrees to motor steering steps and checks for direction*/
            motorSteps=f_DecodeOrientationIntoSteering(pidOrientationOutput);
            motorSteps = (long)((motorSteps) / STEERING_STEP);
            
            xQueueOverwrite(Queue_steering,&motorSteps);

            /*Delay task to allow receiving new inputs to system and to allow motor task to be scheduled*/
             vTaskDelay(20);
        }

    }
}
