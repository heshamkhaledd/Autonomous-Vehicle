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

#include "inc/hw_ints.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "inc/hw_uart.h"
#include "driverlib/uart.h"

#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <semphr.h>
#include <queue.h>
#include "FreeRTOSConfig.h"
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
