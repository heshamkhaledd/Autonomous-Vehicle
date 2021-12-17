/******************************************************************************
 *
 * File Name:   mpu9250_tasks.h
 *
 * Description: Header file to initialize MPU Tasks, and the process functions
 *
 * Date:        5/11/2021
 *
 ******************************************************************************/

#ifndef AUTONOMOUSCONTROLSYSTEM_SRC_MPU9250_MPU9250_TASKS_H_
#define AUTONOMOUSCONTROLSYSTEM_SRC_MPU9250_MPU9250_TASKS_H_

#include <AutonomousControlSystem/inc/common_includes.h>
#include <AutonomousControlSystem/src/MPU9250/serialPort/UARTs.h>
#include "mpu9250_driver.h"
#include <FreeRTOS/include/semphr.h>

/*****************************************************************
 *                      Static Configuration
 *****************************************************************/

#define MPU9250_PRIORITY 2

// Needs refining after the final code
#define MPU9250_STACK_SIZE 200

/*****************************************************************
 *                       Global Variables
 *****************************************************************/
//extern MyMPU myMPU;

/*****************************************************************
 *                       Semaphores
 *****************************************************************/
extern SemaphoreHandle_t Sem_MPU9250_Provide_Data;

/*****************************************************************
 *                       Queues
 *****************************************************************/
extern QueueHandle_t Queue_MPU9250_Data;



/*****************************************************************
 *                    TASKS Declaration
 *****************************************************************/
void vInit_MPU9250_Tasks(void);
void vTASK_MPU9250_Processing(void * pvParameters);

#endif /* AUTONOMOUSCONTROLSYSTEM_SRC_MPU9250_MPU9250_TASKS_H_ */
