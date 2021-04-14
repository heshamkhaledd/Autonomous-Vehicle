/******************************************************************************
 *
 * File Name:   STEPPER.H
 *
 * Description: Steering Motor header file, includes the intializing task, orientation
 *              to sterring decoding functions and the steering task.
 *
 * Date:        7/4/2021
 *
 ******************************************************************************/

#ifndef STEPPER_H_
#define STEPPER_H_

#include "common_includes.h"
#include "portable.h"

/****************************************************************
 *                       Structures                             *
*****************************************************************/

/* Structure storing the pins and base port for a stepper motor*/
typedef struct
{
  uint32_t Port_Clock;
  uint32_t Port_Base;
  uint8_t Pulse_Pin;
  uint8_t Direction_Pin;
  uint8_t Enable_Pin;
  uint8_t Driver_Delay;
} StepperConfig;


/*******************************************************************************
*                          Functions Prototypes                                *
*******************************************************************************/
void vInit_Stepper_Driver(StepperConfig *steeringPtr);

int32_t int32_Move_Stepper(QueueHandle_t controllingQueue, int32_t currentSteps, int32_t desiredSteps, StepperConfig *steeringPtr);

#endif /* STEPPER_H_ */
