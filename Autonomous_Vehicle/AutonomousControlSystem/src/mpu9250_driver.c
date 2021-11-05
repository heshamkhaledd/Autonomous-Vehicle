 /******************************************************************************
 *
 * File Name:   mpu9250_driver.c
 *
 * Description: Source file to initialize MPU driver, and provide some functions
 *
 * Date:        4/11/2021
 *
 ******************************************************************************/

#include <AutonomousControlSystem/inc/mpu9250_driver.h>

 /******************************************************************************
  * Function Name: vInit_MPU9250_Driver
  *
  * Description: For initializing the MPU9250 driver tasks.
  *
  * Arguments:   MyMPU *myMPU
  * Return:      void
  *****************************************************************************/
void vInit_MPU9250_Driver(MyMPU *myMPU)
 {
     myMPU->M = GetP();

     //initialize the MPU value
     MPU9250_value_init(myMPU->M);

     //  Initialize board and FPU
     HAL_BOARD_CLOCK_Init();

     //  Initialize hardware used by MPU9250
     InitHW();

     //  Software initialization of MPU9250
     //  Either configure registers for direct sensor readings or load DMP
     //  firmware

     InitSW();

#ifdef __HAL_USE_MPU9250_NODMP__
    //  Set AHRS time step to 1kHz and configure gains
    // Settings in mpu.InitSW(); are using 200MHz data sampling rate
    mpu.SetupAHRS(0.005, 0.5, 0.00);
#endif  /* __HAL_USE_MPU9250_NODMP__ */
 }

/******************************************************************************
 * Function Name: MPU9250_ProvideData
 *
 * Description: For get yaw, pitch, and roll values in degrees.
 *
 * Arguments:   MyMPU *myMPU
 * Return:      void
 *****************************************************************************/
void MPU9250_ProvideData(MyMPU *myMPU)
{
    //  Check if MPU toggled interrupt pin
    //  (this example doesn't use actual interrupts, but polling)
    if (HAL_MPU_DataAvail())
    {

        //  Read sensor data
        ReadSensorData(myMPU->M);

        //  Get RPY values
        /* get yaw pitch and roll values in degrees */
        RPY(myMPU->rpy, true, myMPU->M);
    }

    // INT pin can be held up for max 50us, so delay here to prevent reading the same data twice
    HAL_DelayUS(100);
}
