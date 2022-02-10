 /******************************************************************************
 *
 * File Name:   throttle_tasks.c
 *
 * Description: Throttle Motor source file, includes the initializing task, orientation
 *              to steering decoding functions and the steering task.
 *
 * Date:        10/2/2020
 *
 ******************************************************************************/

#include <AutonomousControlSystem/inc/throttle_tasks.h>

/* Queue to send the measurement from to the PID*/
extern QueueHandle_t Queue_Measurement;

/*Defining the structure carrying the throttle stepper motor configurations*/
static StepperConfig Throttle_Args = {THROTTLE_DRIVER_PORT_CLOCK,
                                                THROTTLE_DRIVER_PORT_BASE,
                                                THROTTLE_PULSE_PIN,
                                                THROTTLE_DIRECTION_PIN,
                                                THROTTLE_ENABLE_PIN,
                                                THROTTLE_STEP_DELAY};

static StepperConfig *throttlePtr = &Throttle_Args;

/******************************************************************************
 *
 * Function Name:   init_throttle_tasks
 *
 * Description:     For initializing the stepper driver tasks, and queue creating.
 *
 * Arguments:       void
 * Return:          void
 *
 *****************************************************************************/
void init_throttle_tasks()
{
    init_stepper_driver(throttlePtr);

    /* Task creation */
    xTaskCreate(vTask_Throttle,                             /* Task Address       */
                "throttle_task",                            /* Task name          */
                THROTTLE_vTASK_STACK_DEPTH,                 /* Size of the stack. */
                NULL,                                       /* Task Parameters.   */
                configMAX_PRIORITIES - THROTTLE_vTASK_PRIO, /* Task Priority .    */
                NULL);                                      /* Task handle        */
}
/******************************************************************************
 *
 * Function Name:   vTask_Throttle
 *
 * Description:     Runs the Stepper Motor. It waits for receiving angle,
 *                  then proceed in stepping in the desired position.
 *
 * Arguments:       void *pvParameters
 * Return:          void
 *
 *****************************************************************************/
void vTask_Throttle(void *pvParameters)
{
#warning ("Requires some editing for the PID controller")

    /* initialize Task Parameters */
    QueueHandle_t Queue_angles_desired = Queue_Throttle_Orientation;

    /* stores the current angle if needed in feedback */
    float currentAngle;

    /* stores the error from the queue */
    float error;

    /* stores the desired angle from the PID (will be the current angle + error */
    float desiredAngle;

    /* stores the number of pulses generated on the stepper */
    int32_t movedSteps = 0;
    int32_t desiredSteps = 0;

    while (1)
    {
        /* receiving the desired angle from USB */
        xQueueReceive(Queue_angles_desired, &error, portMAX_DELAY);

        /* determine the current angle of the throttle depending on the position of the motor */
        currentAngle = movedSteps * THROTTLE_DRV_ANGLES_PER_STEP;

        desiredAngle=currentAngle+error;


        /* Check if received steering angle/orientation exceeds the physical limits of our car,
         * if received orientation, exceeds in one direction, we limit it to our maximum     */
        desiredAngle = desiredAngle > MAX_ANGLE ? MAX_ANGLE : desiredAngle;
        desiredAngle = desiredAngle < MIN_ANGLE ? MIN_ANGLE : desiredAngle;

        /* multiply the received angle with the ANGLE_TO_THROTTLE_PARAM, in order to
         * get number of driver steps needed for the received throttle motor angle */
        desiredSteps = desiredAngle * ANGLE_TO_THROTTLE_PARAM;



        /* move motor by desired steps */
        movedSteps = int32_move_stepper(Queue_angles_desired, movedSteps, desiredSteps, throttlePtr);

        UART_sendString (UART0_BASE, "\n\r Received in throttle and movedsteps=  ");
        UART0_send_num_in_ASCII (movedSteps);

        xQueueSend(Queue_Measurement, &currentAngle, portMAX_DELAY);

        //xSemaphoreGive(PID_Block_Sem);
    }
}
