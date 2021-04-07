 /******************************************************************************
 *
 * File Name:   THROTTLE_TASKS.h
 *
 * Description: Throttle control header file
 *
 * Date:        10/2/2020
 *
 ******************************************************************************/

#ifndef THROTTLE_TASKS_H_
#define THROTTLE_TASKS_H_

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
#include "STEPPER.h"
/*******************************************************************************
 *                           Preprocessor Definies                             *
 *******************************************************************************/

/* Static Configuration */
#define THROTTLE_vTASK_STACK_DEPTH 80

/* Throttle Driver Port&Pins configurations */
 #define THROTTLE_DRIVER_PORT_CLOCK      SYSCTL_PERIPH_GPIOE
 #define THROTTLE_DRIVER_PORT_BASE       GPIO_PORTE_BASE
 #define THROTTLE_PULSE_PIN              GPIO_PIN_1
 #define THROTTLE_DIRECTION_PIN          GPIO_PIN_2
 #define THROTTLE_ENABLE_PIN             GPIO_PIN_3
 #define THROTTLE_STEP_DELAY             10
 #define THROTTLE_vTASK_PRIO             2

/* Stepper steps & angles parameters */
/* 800 pulse/rev step 2.22 CONVERT_TO_ANGLES 0.45  */
/* 3200 pulse/rev step 8.69 CONVERT_TO_ANGLES 0.1125*/
/* 1600 pulse/rev step 4.44 CONVERT_TO_ANGLES 0.225*/

#define STEP           2.22 
#define CONVERT_TO_ANGLES 0.45 


#define LOW            0
/* range from MIN_ANGLE to MAX_ANGLE*/
#define MAX_ANGLE   15
#define MIN_ANGLE   -5


/*****************************************************************
 *                    Variables
 *****************************************************************/

/* Queue Handlers for data from USB */
extern QueueHandle_t Queue_Throttle_Orientation;

/*******************************************************************************
*                          Functions Prototypes                                *
*******************************************************************************/
extern void vInit_throttle_Tasks();
extern void vTask_throttle(void *);

#endif /* THROTTLE_TASKS_H_ */
