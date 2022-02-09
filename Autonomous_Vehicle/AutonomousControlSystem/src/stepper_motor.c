/******************************************************************************
 *
 * File Name:   stepper_motor.c
 *
 * Description: Stepper motor source file includes functions that initializes
 *              and controls a stepper motor,
 *              It will be used in steering, throttle and braking stepper motors.
 *
 * Date:        7/4/2021
 *
 ******************************************************************************/

#include <AutonomousControlSystem/inc/stepper_motor.h>

/******************************************************************************
 *
 * Function Name:   init_stepper_driver
 *
 * Description:     Initializes HW (pins/ports and clock configurations) for a
 *                  stepper motor and enables it.
 *
 * Arguments:       StepperConfig *steeringPtr
 * Return:          void
 *
 *****************************************************************************/
void init_stepper_driver(StepperConfig *steeringPtr)
{
    /* Enable the clock to GPIO port that is used for the steering stepper driver. */
    MAP_SysCtlPeripheralEnable(steeringPtr->Port_Clock);

    /* Check if the peripheral access is enabled. */
    while (!MAP_SysCtlPeripheralReady(steeringPtr->Port_Clock));

    /* Enable the GPIO pin for steering stepper driver pulse.
     * --> Set the direction as output, and enable the GPIO pin for digital function. */
    MAP_GPIOPinTypeGPIOOutput(steeringPtr->Port_Base, steeringPtr->Pulse_Pin);

    /* Enable the GPIO pin for steering stepper driver direction.
     * --> Set the direction as output, and enable the GPIO pin for digital function. */
    MAP_GPIOPinTypeGPIOOutput(steeringPtr->Port_Base, steeringPtr->Direction_Pin);

    /* Enable the GPIO pin for steering stepper driver enable.
     * --> Set the direction as output, and enable the GPIO pin for digital function. */
    MAP_GPIOPinTypeGPIOOutput(steeringPtr->Port_Base, steeringPtr->Enable_Pin);

    /* Initialize Enable pin by HIGH to enable rotating steering. */
    MAP_GPIOPinWrite(steeringPtr->Port_Base, steeringPtr->Enable_Pin, steeringPtr->Enable_Pin);
}

/******************************************************************************
 *
 * Function Name:   int32_move_stepper
 *
 * Description:     Initializes HW (pins/ports and clock configurations) for a
 *                  stepper motor and enables it.
 *
 * Arguments:       QueueHandle_t controllingQueue, int32_t currentSteps,
 *                  int32_t desiredSteps, StepperConfig *steeringPtr
 *
 * Return:          int32 currentSteps: the moved steps of motor before it
 *                  receives a new input.
 *
 *****************************************************************************/
int32_t int32_move_stepper(QueueHandle_t controllingQueue, int32_t currentSteps, int32_t desiredSteps, StepperConfig *steeringPtr)
{
    float desiredOrientation;
    while (currentSteps != desiredSteps)
    {
        /* Check if a new value is passed to the controlling queue, if yes return from the function */
        if (controllingQueue != NULL && xQueuePeek(controllingQueue, &desiredOrientation, 0))
        {
            return currentSteps;
        }

        if (desiredSteps > currentSteps)
        {
            currentSteps++;

            /* Set direction to left */
            MAP_GPIOPinWrite(steeringPtr->Port_Base, steeringPtr->Direction_Pin, ~(steeringPtr->Direction_Pin));
        }
        else
        {
            currentSteps--;

            /* Set direction to right */
            MAP_GPIOPinWrite(steeringPtr->Port_Base, steeringPtr->Direction_Pin, steeringPtr->Direction_Pin);
        }

        /* Making a high pulse on the pulse pin to initiate a step on the pulse rising edge */
        MAP_GPIOPinWrite(steeringPtr->Port_Base, steeringPtr->Pulse_Pin, steeringPtr->Pulse_Pin);

        /* Delay to recognize the high pulse */
        vTaskDelay(steeringPtr->Driver_Delay);

        /* End of pulse by making a falling edge */
        MAP_GPIOPinWrite(steeringPtr->Port_Base, steeringPtr->Pulse_Pin, ~(steeringPtr->Pulse_Pin));

        /* Delay before another pulse */
        vTaskDelay(steeringPtr->Driver_Delay);
    }

    return currentSteps;
}
