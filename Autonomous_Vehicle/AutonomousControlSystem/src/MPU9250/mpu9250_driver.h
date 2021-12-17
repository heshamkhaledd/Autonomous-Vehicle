 /******************************************************************************
 *
 * File Name:   mpu9250_driver.h
 *
 * Description: Header file to initialize MPU driver, and provide some functions
 *
 * Date:        4/11/2021
 *
 ******************************************************************************/

#ifndef AUTONOMOUSCONTROLSYSTEM_SRC_MPU9250_MPU9250_DRIVER_H_
#define AUTONOMOUSCONTROLSYSTEM_SRC_MPU9250_MPU9250_DRIVER_H_

#include "mpu9250/mpu9250.h"

/******************************************************************************
*                               Functions Prototypes                          *
 *****************************************************************************/
/******************************************************************************
 * Function Name: vInit_MPU9250_Driver
 *
 * Description: For initializing the MPU9250 driver tasks.
 *
 * Arguments:   MyMPU *myMPU
 * Return:      void
 *****************************************************************************/
void vInit_MPU9250_Driver(MPU9250 *M);
/******************************************************************************
 * Function Name: MPU9250_ProvideData
 *
 * Description: For get yaw, pitch, and roll values in degrees.
 *
 * Arguments:   MyMPU *myMPU
 * Return:      void
 *****************************************************************************/
void MPU9250_ProvideData(MPU9250 *M, float *rpy, float *xyz);



#endif /* AUTONOMOUSCONTROLSYSTEM_SRC_MPU9250_MPU9250_DRIVER_H_ */
