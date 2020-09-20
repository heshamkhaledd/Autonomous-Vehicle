/*
 * PID_TASKS.h
 *
 *  Created on: 7 Oct 2019
 *      Author: okasha
 */

#ifndef PID_TASKS_H_
#define PID_TASKS_H_

//freeRTOS includes

#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <semphr.h>
#include <queue.h>

#include <stdint.h>

#include "PID_Control.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"


/****************************************************
 *                Global Variables
 ****************************************************/
extern QueueHandle_t Queue_steering;
extern QueueHandle_t Queue_Feedback_Orientation;
extern QueueHandle_t Queue_Desired_Orientation;
extern QueueHandle_t Queue_Current_Orientation;
extern QueueHandle_t Queue_steering;

extern float watch_steering ;

extern volatile UBaseType_t PIDTaskHighWaterMark ;

/****************************************************
 *                Defines
 ****************************************************/

#define STEERING_STEP 1.8
#define PID_STACK_DEPTH 200
#define PID_vTASK_PRIO 1



extern void vInit_PID();
void vTask_PID(void * para);


#endif /* PID_TASKS_H_ */
