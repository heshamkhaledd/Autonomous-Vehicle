/******************************************************************************
 *
 * File Name:   mpu9250_tasks.c
 *
 * Description: Source file to initialize MPU Tasks, and the process functions
 *
 * Date:        5/11/2021
 *
 ******************************************************************************/
#include <AutonomousControlSystem/inc/mpu9250_tasks.h>

/*****************************************************************
 *                       Global Variables
 *****************************************************************/
MyMPU myMPU;

/*****************************************************************
 *                       Semaphores
 *****************************************************************/
/* Declaring Semaphores Handles */
SemaphoreHandle_t Sem_MPU9250_Provide_Data;

/*****************************************************************
 *                       Queues
 *****************************************************************/
extern QueueHandle_t Queue_MPU9250_Data;

/******************************************************************************
 * Function Name: vInit_USBTasks
 *
 * Description: Responsible for Creating MPU9250 tasks
 *
 * Arguments:   void
 * Return:      void
 *****************************************************************************/
void vInit_MPU9250_Tasks(void)
{
    vInit_MPU9250_Driver(&myMPU);
    Sem_MPU9250_Provide_Data = xSemaphoreCreateBinary();
    Queue_MPU9250_Data = xQueueCreate(1, 3 * typedef(float));

    xTaskCreate(vTASK_MPU9250_Processing,
                "MPU9250_Processing",
                MPU9250_STACK_SIZE,
                NULL,
                configMAX_PRIORITIES - MPU9250_PRIORITY,
                NULL);
}


void vTASK_MPU9250_getRPY(void *pvParameters)
{

    /* Super Loop */
    while(1)
    {
        MPU9250_ProvideData(&myMPU);
        xQueueSend(Queue_MPU9250_Data, (void*)(myMPU->rpy), portMAX_DELAY);
    }
}
