/*
 * STEPPER.C
 *
 *  Created on: Apr 7, 2021
 *      Author: Yousef_Khaled
 */

#include "STEPPER.h"
/******************************************************************************
 *
 * Function Name: vInit_Stepper_Driver
 *
 * Description: For enabling and initialising the port/pins configurations and clock
 *
 * Arguments:
 * Return:      void
 *
 *****************************************************************************/
void vInit_Stepper_Driver(StepperConfig *steeringPtr)
{
    /* Enable the clock to GPIO port that is used for the steering stepper driver. */
    MAP_SysCtlPeripheralEnable(steeringPtr->Port_Clock);

    /* Check if the peripheral access is enabled. */
    while( !MAP_SysCtlPeripheralReady(steeringPtr->Port_Clock));

    /* Enable the GPIO pin for steering stepper driver pulse.
     * --> Set the direction as output, and enable the GPIO pin for digital function. */
    MAP_GPIOPinTypeGPIOOutput(steeringPtr->Port_Base, steeringPtr->Pulse_Pin);

    /* Enable the GPIO pin for steering stepper driver direction.
     * --> Set the direction as output, and enable the GPIO pin for digital function. */
    MAP_GPIOPinTypeGPIOOutput(steeringPtr->Port_Base, steeringPtr->Direction_Pin);

    /* Enable the GPIO pin for steering stepper driver enable.
     * --> Set the direction as output, and enable the GPIO pin for digital function. */
    MAP_GPIOPinTypeGPIOOutput(steeringPtr->Port_Base, steeringPtr->Enable_Pin) ;

    /* Initialise Enable pin by HIGH to enable rotating steering manually. */
    MAP_GPIOPinWrite(steeringPtr->Port_Base, steeringPtr->Enable_Pin, steeringPtr->Enable_Pin);
}


int32_t uMove_Stepper(QueueHandle_t Queue_Desired_Angle, int32_t movedSteps, long  stepsDesired , StepperConfig *steeringPtr){
    float desiredOrientation;
    while(movedSteps != stepsDesired)
         {
             if(xQueuePeek(Queue_Desired_Angle,&desiredOrientation,0)){
                 return movedSteps;
             }

             if(stepsDesired>movedSteps)
             {
                 movedSteps++;
                 /* Set direction to left*/
                 MAP_GPIOPinWrite(steeringPtr->Port_Base, steeringPtr->Direction_Pin, ~(steeringPtr->Direction_Pin));
             }
             else
             {
                 movedSteps--;
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
    return movedSteps;
}

