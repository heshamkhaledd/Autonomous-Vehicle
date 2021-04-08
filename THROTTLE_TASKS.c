 /******************************************************************************
 *
 * File Name:   THROTTLE_TASKS.c
 *
 * Description: Throttle control source file
 *
 * Date:        10/2/2020
 *
 ******************************************************************************/

#include "THROTTLE_TASKS.h"

static StepperConfig Throttle_Args = {THROTTLE_DRIVER_PORT_CLOCK,
                                                THROTTLE_DRIVER_PORT_BASE,
                                                THROTTLE_PULSE_PIN,
                                                THROTTLE_DIRECTION_PIN,
                                                THROTTLE_ENABLE_PIN,
                                                THROTTLE_STEP_DELAY};

static StepperConfig *throttlePtr = &Throttle_Args;

/******************************************************************************
 *
 * Function Name: vTask_throttle
 *
 * Description: Runs the Stepper Motor. It waits for receiving angle , then,
 *              proceed in stepping in the desired position.
 *
 * Arguments:   void *pvParameters
 * Return:      void
 *
 *****************************************************************************/

void vTask_throttle(void *pvParameters)
{

    /* Initialize Task Parameters */
    QueueHandle_t Queue_angles_desired = Queue_Throttle_Orientation;
    /*stores the current angle if needed in feedback*/
    float current_angle;
    /* stores the desired angle read from queue */
    float desired_angle;
    /*stores the number of pulses generated on the stepper*/
    int32_t movedSteps = 0;
    long stepsDesired;

    while (1)
    {

        /* QUEUE BLOCKING */
        /*receiving the desired angle from USB*/
        xQueueReceive(Queue_angles_desired, &desired_angle, portMAX_DELAY);

        /*mapping the range of angles from -5 to 15*/
        if (desired_angle >= MAX_ANGLE)
        {
            desired_angle = MAX_ANGLE;
        }
        else if (desired_angle < MIN_ANGLE)
        {
            desired_angle = MIN_ANGLE;
        }
        /*determine the current angle of the throttle depending on the position of the motor*/
        current_angle = movedSteps * CONVERT_TO_ANGLES;
        /*convert desried angle into steps or pulses*/
        stepsDesired = ((STEP) * (desired_angle));

         movedSteps = uMove_Stepper(Queue_angles_desired, movedSteps, stepsDesired, throttlePtr);
    }
}
/******************************************************************************
 *
 * Function Name: vInit_throttle_Tasks
 *
 * Description: For initialise the stepper driver tasks, and queue creating.
 *
 * Arguments:   void
 * Return:      void
 *
 *****************************************************************************/

void vInit_throttle_Tasks()
{

    vInit_Stepper_Driver(throttlePtr);

                
    /* Task creation */
    xTaskCreate(vTask_throttle,                             /* Task Address       */
                "throttle_task",                            /* Task name          */
                THROTTLE_vTASK_STACK_DEPTH,                 /* Size of the stack. */
                NULL,                                       /* Task Parameters.   */
                configMAX_PRIORITIES - THROTTLE_vTASK_PRIO, /* Task Priority .    */
                NULL);                                      /* Task handle        */
}
