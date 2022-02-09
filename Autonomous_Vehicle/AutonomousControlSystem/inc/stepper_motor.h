/******************************************************************************
 *
 * File Name:   stepper_motor.h
 *
 * Description: Steering Motor header file, includes the initializing task,
 *              orientation to steering decoding functions and the steering task.
 *
 * Date:        7/4/2021
 *
 ******************************************************************************/

#ifndef STEPPER_MOTOR_H_
#define STEPPER_MOTOR_H_

#include <AutonomousControlSystem/inc/common_includes.h>
#include "portable.h"

/*******************************************************************************
*                              Structures                                      *
********************************************************************************/

/* Structure storing the configurations of the stepper motor*/
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
********************************************************************************/
void init_stepper_driver(StepperConfig *steeringPtr);
int32_t int32_move_stepper(QueueHandle_t controllingQueue, int32_t currentSteps, int32_t desiredSteps, StepperConfig *steeringPtr);

#endif /* STEPPER_MOTOR_H_ */
