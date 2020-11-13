 /******************************************************************************
 *
 * File Name:   STEPPER_TASKS.h
 *
 * Description: Stepper motor header file to initialize stepper motor tasks
 *
 * Date:        10/2/2020
 *
 ******************************************************************************/




#ifndef STEPPER_TASKS_H_
#define STEPPER_TASKS_H_

#include "inc/hw_ints.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"

#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <semphr.h>
#include <queue.h>
#include "orientationAndSteeringUtils.h"

/*******************************************************************************
 *                           Preprocessor Definies                             *
 *******************************************************************************/

/* Static Configurations */
#define STEPPER_vTASK_STACK_DEPTH 80

/* Steering Driver Port&Pins configurations */
 #define STEERING_DRIVER_PORT_CLOCK SYSCTL_PERIPH_GPIOF
 #define STEERING_DRIVER_PORT_BASE GPIO_PORTF_BASE
 #define STEERING_PULSE_PIN GPIO_PIN_1
 #define STEERING_DIRECTION_PIN GPIO_PIN_2
 #define STEERING_ENABLE_PIN GPIO_PIN_3
 #define STEERING_STEP_DELAY 5

 #define STEERING_vTASK_PRIO 2

/* Steering Driver Port&Pins configurations */
 #define BRAKES_DRIVER_PORT_CLOCK SYSCTL_PERIPH_GPIOB
 #define BRAKES_DRIVER_PORT_BASE GPIO_PORTB_BASE
 #define BRAKES_PULSE_PIN GPIO_PIN_2
 #define BRAKES_DIRECTION_PIN GPIO_PIN_3
 #define BRAKES_ENABLE_PIN GPIO_PIN_4
 #define BRAKES_STEP_DELAY 10

 #define BRAKES_vTASK_PRIO 1

 
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


/*****************************************************************
 *                          Variables                            *
******************************************************************/

 /* Queue Handlers for data from USB: The desired orientation of the vehicle */
extern QueueHandle_t Queue_Desired_Orientation;


/*******************************************************************************
*                          Functions Prototypes                                *
*******************************************************************************/
static void vInit_Stepper_Driver(void);
extern void vInit_Steppers_Tasks();
void vTask_Stepper(void *pvParameters);


 
 #endif /* STEPPER_TASKS_H_ */
 
