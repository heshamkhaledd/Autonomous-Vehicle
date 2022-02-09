 /******************************************************************************
 *
 * File Name:   steering_tasks.h
 *
 * Description: Steering Motor header file, includes the initializing task and
 *              the steering task.
 *
 * Date:        10/2/2020
 *
 *******************************************************************************/
#ifndef STEERING_TASKS_H
#define STEERING_TASKS_H

#include <AutonomousControlSystem/inc/common_includes.h>
#include <AutonomousControlSystem/inc/stepper_motor.h>

/*******************************************************************************
 *                           Preprocessor Defines                              *
 *******************************************************************************/

/* Steering motor task/HW Configurations */
#define STEPPER_vTASK_STACK_DEPTH       80
#define STEERING_vTASK_PRIO             2

/* Steering Driver Port & Pins configurations */
 #define STEERING_DRIVER_PORT_CLOCK     SYSCTL_PERIPH_GPIOF
 #define STEERING_DRIVER_PORT_BASE      GPIO_PORTF_BASE
 #define STEERING_PULSE_PIN             GPIO_PIN_1
 #define STEERING_DIRECTION_PIN         GPIO_PIN_2
 #define STEERING_ENABLE_PIN            GPIO_PIN_3
 #define STEERING_STEP_DELAY            5

/* Steering Configurations :
 *
 * 1) STEERING_DRV_ANGLES_PER_STEP:
 *      Steering driver used has 800 steps per revolution, therefore, in order
 *      to get degrees per step, divide 360 degrees by 800 step, to get
 *      0.45 degrees per driver step.
 *
 * 2) ORIENT_TO_STEERING_PARAM:
 *
 *
 *
 */
#define STEERING_DRV_ANGLES_PER_STEP 0.45f
#define ORIENT_TO_STEERING_PARAM 3.5f

/* These Values are Tested Physically. Date: 22/11/2020 */
#define MAX_ORIENTATION  26
#define MIN_ORIENTATION -24

/******************************************************************************
 *                          Variables                                         *
*******************************************************************************/

 /* Queue Handlers for data from USB: The desired orientation of the vehicle */
extern QueueHandle_t Queue_Desired_Orientation;

/******************************************************************************
*                          Functions Prototypes                               *
*******************************************************************************/

extern void init_steering_tasks();
void vTask_Steering(void *pvParameters);
 
 #endif /* STEERING_TASKS_H */
 
