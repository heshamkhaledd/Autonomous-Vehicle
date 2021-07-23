 /******************************************************************************
 *
 * File Name:   THROTTLE_TASKS.h
 *
 * Description: Throttle control header file
 *
 * Date:        10/2/2020
 *
 ******************************************************************************/

#ifndef AUTONOMOUSCONTROLSYSTEM_INC_THROTTLE_TASKS_H_
#define AUTONOMOUSCONTROLSYSTEM_INC_THROTTLE_TASKS_H_

#include <AutonomousControlSystem/inc/common_includes.h>
#include <AutonomousControlSystem/inc/stepper_motor.h>
#include "portable.h"

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

#endif /* AUTONOMOUSCONTROLSYSTEM_INC_THROTTLE_TASKS_H_ */
