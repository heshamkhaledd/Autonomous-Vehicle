 /******************************************************************************
 *
 * File Name:   throttle_tasks.h
 *
 * Description: Throttle Motor header file, includes the initializing task and
 *              the throttle task.
 *
 * Date:        10/2/2020
 *
 ******************************************************************************/

#ifndef AUTONOMOUSCONTROLSYSTEM_INC_THROTTLE_TASKS_H_
#define AUTONOMOUSCONTROLSYSTEM_INC_THROTTLE_TASKS_H_

#include <AutonomousControlSystem/inc/common_includes.h>
#include <AutonomousControlSystem/inc/stepper_motor.h>
#include <AutonomousControlSystem/inc/debug.h>
#include "portable.h"

/*******************************************************************************
 *                           Preprocessor Defines                              *
 *******************************************************************************/

/* Static Configuration */
#define THROTTLE_vTASK_STACK_DEPTH      80

/* Throttle Driver Port & Pins configurations */
 #define THROTTLE_DRIVER_PORT_CLOCK      SYSCTL_PERIPH_GPIOE
 #define THROTTLE_DRIVER_PORT_BASE       GPIO_PORTE_BASE
 #define THROTTLE_PULSE_PIN              GPIO_PIN_1
 #define THROTTLE_DIRECTION_PIN          GPIO_PIN_2
 #define THROTTLE_ENABLE_PIN             GPIO_PIN_3
 #define THROTTLE_STEP_DELAY             10
 #define THROTTLE_vTASK_PRIO             2

/* Stepper Steps & Angles Parameters:
 * Throttle stepper motor driver allows three different configurations regarding
 * the number of pulses it outputs per revolution: 800,1600,3200.
 *
 * ANGLE_TO_THROTTLE_PARAM = (pulse/revolution)/360.
 * THROTTLE_DRV_ANGLES_PER_STEP = 1/ANGLE_TO_THROTTLE_PARAM.
 *
 * Throttle Stepper Motor Configurations:
 * 800  pulse/revolution -> ANGLE_TO_THROTTLE_PARAM  2.22   -> THROTTLE_DRV_ANGLES_PER_STEP 0.45
 * 1600 pulse/revolution -> ANGLE_TO_THROTTLE_PARAM  4.44   -> THROTTLE_DRV_ANGLES_PER_STEP 0.225
 * 3200 pulse/revolution -> ANGLE_TO_THROTTLE_PARAM  8.69   -> THROTTLE_DRV_ANGLES_PER_STEP 0.1125
*/

/* Stepper Steps & Angles Parameters: */
#define ANGLE_TO_THROTTLE_PARAM         2.22f
#define THROTTLE_DRV_ANGLES_PER_STEP    0.45f

//to be tested
/* range from MIN_ANGLE to MAX_ANGLE*/
#define MAX_ANGLE                       50
#define MIN_ANGLE                       0

//#define TEST_STEPPER

/*******************************************************************************
*                               Variables                                      *
********************************************************************************/
/* Queue Handlers for data from USB */
extern QueueHandle_t Queue_Throttle_Orientation;

extern QueueHandle_t Queue_Speed;

/* Queue to send the measurement from to the PID*/
extern QueueHandle_t Queue_Measurement;

/*******************************************************************************
*                          Functions Prototypes                                *
*******************************************************************************/
extern void init_throttle_tasks();
extern void vTask_Throttle(void *);

#endif /* AUTONOMOUSCONTROLSYSTEM_INC_THROTTLE_TASKS_H_ */
