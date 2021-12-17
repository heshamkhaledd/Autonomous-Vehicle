/******************************************************************************
 *
 * File Name:   mpu9250_tasks.c
 *
 * Description: Source file to initialize MPU Tasks, and the process functions
 *
 * Date:        5/11/2021
 *
 ******************************************************************************/

#include "mpu9250_tasks.h""

/*****************************************************************
 *                       Global Variables
 *****************************************************************/
MPU9250 *M;

/*****************************************************************
 *                       Semaphores
 *****************************************************************/
/* Declaring Semaphores Handles */
// SemaphoreHandle_t Sem_MPU9250_Provide_Data;

/*****************************************************************
 *                       Queues
 *****************************************************************/
// QueueHandle_t Queue_MPU9250_Data;

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
    vInit_MPU9250_Driver(M);
    // Sem_MPU9250_Provide_Data = xSemaphoreCreateBinary();
    // Queue_MPU9250_Data = xQueueCreate(1, 4 * sizeof(float));

    xTaskCreate(vTASK_MPU9250_Processing,
                "MPU9250_Processing",
                MPU9250_STACK_SIZE,
                NULL,
                configMAX_PRIORITIES - MPU9250_PRIORITY,
                NULL);
}


void vTASK_MPU9250_Processing(void *pvParameters)
{
    // uint8_t FeedbackDataToTransmit[4];
    float rpy[3];
    float xyz[3];
    /* Super Loop */
    while(1)
    {
        // xSemaphoreTake(Sem_MPU9250_Provide_Data, portMAX_DELAY);
        MPU9250_ProvideData(M, rpy, xyz);

        /* Prepare the bucket */
        /*
        FeedbackDataToTransmit[0] = (uint8_t)rpy[2];
        FeedbackDataToTransmit[1] = (uint8_t)xyz[0];
        FeedbackDataToTransmit[2] = (uint8_t)xyz[1];
        FeedbackDataToTransmit[3] = (uint8_t)xyz[2];
        xQueueSend(Queue_MPU9250_Data, (void*)(FeedbackDataToTransmit), portMAX_DELAY);
        */
        UART0_SendString("Yaw angle is = ");
        UART_sendNumber(_FTOI_I(rpy[0]));
        UART0_SendChr('.');
        UART_sendNumber(_FTOI_F(rpy[0]));
        UART0_SendString("\r\n");

        UART0_SendString("Pitch angle is  = ");
        UART_sendNumber(_FTOI_I(rpy[1]));
        UART0_SendChr('.');
        UART_sendNumber(_FTOI_F(rpy[1]));
        UART0_SendString("\r\n");

        UART0_SendString("Roll angle is    = ");
        UART_sendNumber(_FTOI_I(rpy[2]));
        UART0_SendChr('.');
        UART_sendNumber(_FTOI_F(rpy[2]));
        UART0_SendString("\r\n");

     }
}
