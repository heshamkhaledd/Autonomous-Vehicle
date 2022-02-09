 /******************************************************************************
 *
 * File Name:   steering_tasks.c
 *
 * Description: Steering Motor source file, includes the initializing task, orientation
 *              to steering decoding functions and the steering task.
 *
 * Date:        10/2/2020
 *
 ******************************************************************************/
#include <AutonomousControlSystem/inc/steering_tasks.h>

/*Defining the structure carrying the steering stepper motor configurations*/
static StepperConfig Steering_Args = {STEERING_DRIVER_PORT_CLOCK,
                                               STEERING_DRIVER_PORT_BASE,
                                               STEERING_PULSE_PIN,
                                               STEERING_DIRECTION_PIN,
                                               STEERING_ENABLE_PIN,
                                               STEERING_STEP_DELAY
                                              };

static StepperConfig *steeringPtr = &Steering_Args;

/******************************************************************************
 *
 * Function Name:   init_steering_tasks
 *
 * Description:     For initialize the stepper driver tasks, and queue creating.
 *
 * Arguments:       void
 * Return:          void
 *
 *****************************************************************************/
void init_steering_tasks(void)
{
    /* Initialize Stepper Driver HW */
    init_stepper_driver(steeringPtr);

    /* Task creation */
    xTaskCreate( vTask_Steering,                             /* Task Address       */
                 "steering_task",                           /* Task name          */
                 STEPPER_vTASK_STACK_DEPTH,                 /* Size of the stack. */
                 NULL,                                      /* Task Parameters.   */
                 configMAX_PRIORITIES-STEERING_vTASK_PRIO,  /* Task Priority .    */
                 NULL);                                     /* Task handle        */
}

/******************************************************************************
 *
 * Function Name: vTask_Steering
 *
 * Description: Runs the steering stepper Motor. It waits for receiving steps,
 *              then proceed in stepping in the desired direction.
 *
 * Arguments:   void *pvParameters
 * Return:      void
 *
 *****************************************************************************/
void vTask_Steering(void *pvParameters)
{
    /* initial condition of the motor*/
    float desiredOrientation = 0;
    int32_t movedSteps = 0;
    int32_t desiredSteps = 0;

    while(1)
    {
        /* Wait until a new input is received in Queue_Desired_Orientation */
        xQueueReceive(Queue_Desired_Orientation,
                      &desiredOrientation,
                      portMAX_DELAY);

        /* Check if received steering angle/orientation exceeds the physical limits of our car,
         * if received orientation, exceeds in one direction, we limit it to our maximum     */
        desiredOrientation = desiredOrientation > MAX_ORIENTATION ? MAX_ORIENTATION : desiredOrientation;
        desiredOrientation = desiredOrientation < MIN_ORIENTATION ? MIN_ORIENTATION : desiredOrientation;

        /* divide the received orientation with the STEERING_DRV_ANGLES_PER_STEP, in order to
         * get number of driver steps needed for the received orientation                    */
        desiredSteps = (long) (desiredOrientation * (ORIENT_TO_STEERING_PARAM / STEERING_DRV_ANGLES_PER_STEP));

        /* move motor by desired steps */
        movedSteps = int32_move_stepper(Queue_Desired_Orientation, movedSteps, desiredSteps, steeringPtr);
    }
}
