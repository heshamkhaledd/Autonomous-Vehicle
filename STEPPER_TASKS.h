/*
 * STEPPER_TASKS.h
 *
 *  Created on: 09/17/2019
 *      Author: Okasha
 */




#ifndef STEPPER_TASKS_H_
#define STEPPER_TASKS_H_


/*                      TIVAware libraries                   */
#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"

//freeRTOS includes

#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <semphr.h>
#include <queue.h>


/*****************************************************************
 *                      Static Configuration
 *****************************************************************/
#define STEPPER_vTASK_STACK_DEPTH 80



/*****************************************************************
 *           Steering Driver Port&Pins configurations
 *****************************************************************/
 #define STEERING_DRIVER_PORT_CLOCK SYSCTL_PERIPH_GPIOF
 #define STEERING_DRIVER_PORT_BASE GPIO_PORTF_BASE
 #define STEERING_PULSE_PIN GPIO_PIN_1
 #define STEERING_DIRECTION_PIN GPIO_PIN_2
 #define STEERING_ENABLE_PIN GPIO_PIN_3
 #define STEERING_STEP_DELAY 5

 #define STEERING_vTASK_PRIO 2


/*****************************************************************
 *           Steering Driver Port&Pins configurations
 *****************************************************************/
 #define BRAKES_DRIVER_PORT_CLOCK SYSCTL_PERIPH_GPIOB
 #define BRAKES_DRIVER_PORT_BASE GPIO_PORTB_BASE
 #define BRAKES_PULSE_PIN GPIO_PIN_2
 #define BRAKES_DIRECTION_PIN GPIO_PIN_3
 #define BRAKES_ENABLE_PIN GPIO_PIN_4
 #define BRAKES_STEP_DELAY 10

 #define BRAKES_vTASK_PRIO 1
/*****************************************************************
 *                    TASKS Declaration
 *****************************************************************/
 extern void vInit_Steppers_Tasks();
 extern void vTask_Stepper(void *);
 
 /****************************************************************
  *                        Structures
  ***************************************************************/

  /* Structure of the different parameters used in each task */
     typedef struct {
         QueueHandle_t  Queue_steps_desired;
         uint32_t driver_port_clock;
         uint32_t driver_port_base;
         uint8_t driver_pulse_pin;
         uint8_t driver_direction_pin;
         uint8_t driver_enable_pin;
         uint8_t driver_delay;
     } stepper_params;


 /*****************************************************************
  *                    Variables
  *****************************************************************/


 /* Queue Handlers for data from USB */
     extern QueueHandle_t Queue_steering;






 
 #endif /* STEPPER_TASKS_H_ */
 
