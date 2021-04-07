/*
 * STEPPER.h
 *
 *  Created on: Apr 7, 2021
 *      Author: Yousef_Khaled
 */

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

  /* Structure of the different parameters used in each task */
     typedef struct {
         uint32_t   Port_Clock;
         uint32_t   Port_Base;
         uint8_t    Pulse_Pin;
         uint8_t    Direction_Pin;
         uint8_t    Enable_Pin;
         uint8_t    Driver_Delay;
     }StepperConfig;

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
void vInit_Stepper_Driver(StepperConfig *steeringPtr);

/******************************************************************************
 *
 * Function Name: uMove_Stepper
 *
 * Description: Responsible of controlling the Stepper (any stepper) steps
 *
 * Arguments:
 * Return:      void
 *
 *****************************************************************************/
int32_t uMove_Stepper(QueueHandle_t Queue_Desired_Angle, int32_t movedSteps, long  stepsDesired , StepperConfig *steeringPtr);

#endif /* STEPPER_H_ */
