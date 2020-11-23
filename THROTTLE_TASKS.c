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

static volatile ThrottleConfig Throttle_Args = {THROTTLE_DRIVER_PORT_CLOCK,
                                                THROTTLE_DRIVER_PORT_BASE,
                                                THROTTLE_PULSE_PIN,
                                                THROTTLE_DIRECTION_PIN,
                                                THROTTLE_ENABLE_PIN,
                                                THROTTLE_STEP_DELAY};

static volatile ThrottleConfig *throttlePtr = &Throttle_Args;
/******************************************************************************
 *
 * Function Name: vInit_Throttle_Drive
 *
 * Description: For enabling and initialising the port/pins configurations and clock
 *
 * Arguments:
 * Return:      void
 *
 *****************************************************************************/
static void vInit_Throttle_Driver()
{

    MAP_SysCtlPeripheralEnable(throttlePtr->Port_Clock); /* Enable the clock to GPIO port that is used for the throttle stepper driver. */

    while (!MAP_SysCtlPeripheralReady(throttlePtr->Port_Clock)); /* Check if the peripheral access is enabled. */

    /* Enable the GPIO pin for throttle stepper driver pulse.
        * --> Set the direction as output, and enable the GPIO pin for digital function. */
    MAP_GPIOPinTypeGPIOOutput(throttlePtr->Port_Base, throttlePtr->Pulse_Pin);

    /* Enable the GPIO pin for throttle stepper driver direction.
        * --> Set the direction as output, and enable the GPIO pin for digital function. */
    MAP_GPIOPinTypeGPIOOutput(throttlePtr->Port_Base, throttlePtr->Direction_Pin);

    /* Enable the GPIO pin for throttle stepper driver enable.
        * --> Set the direction as output, and enable the GPIO pin for digital function. */
    MAP_GPIOPinTypeGPIOOutput(throttlePtr->Port_Base, throttlePtr->Enable_Pin);

    /*Initialize Enable pin to be 0*/
    MAP_GPIOPinWrite(throttlePtr->Port_Base, throttlePtr->Enable_Pin, 0);
}
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
    /*
         * Queue handler
         * driver port clock
         * driver port base
         * driver pulse pin
         * driver direction pin
         * driver enable pin
         * driver delay
         */

    /* Initialize Task Parameters */
    QueueHandle_t Queue_angles_desired = Queue_Throttle_Orientation;

    /* variables to convert desired angle to pulses on stepper*/
    uint16_t end;
    uint16_t pulse_counter;
    float counter = 0;
    float current_angle;
    float relate_angle;
    float desired_angle;

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
        current_angle = counter * CONVERT_TO_ANGLES;

        /*determine the angle needed to be converted to pulses on stepper*/
        relate_angle = desired_angle - current_angle;

        /*controlling the direction */
        if (relate_angle >= 0) //if the angle desired is larger than the current
        {

            /*opening the throttle*/
            /*convert angles to pulses*/
            end = ((STEP) * (relate_angle));
            /* counterclockwise direction*/
            GPIOPinWrite(throttlePtr->Port_Base, throttlePtr->Direction_Pin, LOW);
            for (pulse_counter = 0; pulse_counter < end; pulse_counter++)
            {

                /* generating the pulse */
                GPIOPinWrite(throttlePtr->Port_Base, throttlePtr->Pulse_Pin, throttlePtr->Pulse_Pin);
                vTaskDelay(throttlePtr->Driver_Delay);
                GPIOPinWrite(throttlePtr->Port_Base, throttlePtr->Pulse_Pin, LOW);
                vTaskDelay(throttlePtr->Driver_Delay);
                /*detecting the steps moved in this pulse*/
                counter++;
            }
        }
        else if (relate_angle < 0)
        {
            /*closing the throttle*/
            /*convert angles to pulses*/
            end = (STEP) * (-relate_angle);
            /* clockwise direction*/
            GPIOPinWrite(throttlePtr->Port_Base, throttlePtr->Direction_Pin, throttlePtr->Direction_Pin); //setting the direction to clockwise

            for (pulse_counter = 0; pulse_counter < end; pulse_counter++)
            {
                /* generating the pulse */
                GPIOPinWrite(throttlePtr->Port_Base, throttlePtr->Pulse_Pin, throttlePtr->Pulse_Pin);

                vTaskDelay(throttlePtr->Driver_Delay); //the delay here is
                GPIOPinWrite(throttlePtr->Port_Base, throttlePtr->Pulse_Pin, LOW);
                vTaskDelay(throttlePtr->Driver_Delay);

                /*detecting the steps moved in this pulse*/
                counter--;
            }
        }
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

    vInit_Throttle_Driver();

                
    /* Task creation */
    xTaskCreate(vTask_throttle,                             /* Task Address       */
                "throttle_task",                            /* Task name          */
                THROTTLE_vTASK_STACK_DEPTH,                 /* Size of the stack. */
                NULL,                        /* Task Parameters.   */
                configMAX_PRIORITIES - THROTTLE_vTASK_PRIO, /* Task Priority .    */
                NULL);                                      /* Task handle        */
}
