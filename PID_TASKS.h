 /******************************************************************************
 *
 * File Name:   PID_tasks.h
 *
 * Description: PID header file to initialize and start the PID Control tasks.
 *
 * Date:        10/2/2020
 *
 ******************************************************************************/

#ifndef PID_TASKS_H_
#define PID_TASKS_H_

#include <PID_controller.h>
#include "common_includes.h"
#include "orientation_decoding.h"

/*******************************************************************************
 *                           Preprocessor Definies                             *
 *******************************************************************************/
#define ERROR_FACTOR    0
#define PID_STACK_DEPTH 200
#define PID_vTASK_PRIO  1

/****************************************************
 *                Global Variables                  *
 ****************************************************/
extern QueueHandle_t Queue_steering;
extern QueueHandle_t Queue_Desired_Orientation;
extern QueueHandle_t Queue_Current_Orientation;
extern QueueHandle_t Queue_steering;

extern float watch_steering ;

extern volatile UBaseType_t PIDTaskHighWaterMark ;

/*******************************************************************************
 *                          Functions Prototypes                               *
 *******************************************************************************/
extern void vInit_PID();
void vTask_PID(void * para);


#endif /* PID_TASKS_H_ */
