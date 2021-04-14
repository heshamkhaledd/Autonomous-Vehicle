/******************************************************************************
 *
 * File Name:   stepper_motor.C
 *
 * Description: Stepper motor source file includes functions that intializes and
 *              controls a stepper motor.
 *
 * Date:        7/4/2021
 *
 ******************************************************************************/

#include "stepper_motor.h"

/******************************************************************************
 *
 * Function Name: vInit_Stepper_Driver
 *
 * Description: Intializes HW (pins/ports and clock configurations) for a stepper motor and 
 *               enables it.
 *
 * Arguments: StepperConfig *steeringPtr
 * Return:      void
 *
 *****************************************************************************/
void vInit_Stepper_Driver(StepperConfig *steeringPtr)
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

    /* Initialise Enable pin by HIGH to enable rotating steering manually. */
    MAP_GPIOPinWrite(steeringPtr->Port_Base, steeringPtr->Enable_Pin, steeringPtr->Enable_Pin);
}

/******************************************************************************
 *
 * Function Name: int32_Move_Stepper
 *
 * Description: Intializes HW (pins/ports and clock configurations) for a stepper motor and 
 *               enables it.
 *
 * Arguments: QueueHandle_t controllingQueue, int32_t currentSteps, int32_t desiredSteps, StepperConfig *steeringPtr
 * Return:    int32 currentSteps: the noved steps of motor before it receives a new input.
 *
 *****************************************************************************/
int32_t int32_Move_Stepper(QueueHandle_t controllingQueue, int32_t currentSteps, int32_t desiredSteps, StepperConfig *steeringPtr)
{
    float desiredOrientation;
    while (currentSteps != desiredSteps)
    {
        /*Check if a new value is passed to the controlling queue, if yes return from the function*/
        if (controllingQueue != NULL && xQueuePeek(controllingQueue, &desiredOrientation, 0))
        {
            return currentSteps;
        }

        if (desiredSteps > currentSteps)
        {
            currentSteps++;
            /* Set direction to left*/
            MAP_GPIOPinWrite(steeringPtr->Port_Base, steeringPtr->Direction_Pin, ~(steeringPtr->Direction_Pin));
        }
        else
        {
            currentSteps--;
            /* Set direction to right*/
            MAP_GPIOPinWrite(steeringPtr->Port_Base, steeringPtr->Direction_Pin, steeringPtr->Direction_Pin);
        }

        /* making a high pulse on pulse pin to make a step.
             * making pulse start by making a rising edge. */
        MAP_GPIOPinWrite(steeringPtr->Port_Base, steeringPtr->Pulse_Pin, steeringPtr->Pulse_Pin);

        /* delay to recognise the high pulse. */
        vTaskDelay(steeringPtr->Driver_Delay);

        /* end of pulse by making a falling edge */
        MAP_GPIOPinWrite(steeringPtr->Port_Base, steeringPtr->Pulse_Pin, ~(steeringPtr->Pulse_Pin));

        /* delay before another pulse. */
        vTaskDelay(steeringPtr->Driver_Delay);
    }
    return currentSteps;
}
