 /******************************************************************************
 *
 * File Name:   mpu9250_driver.h
 *
 * Description: Header file to initialize MPU driver, and provide some functions
 *
 * Date:        4/11/2021
 *
 ******************************************************************************/

#ifndef AUTONOMOUSCONTROLSYSTEM_INC_MPU9250_DRIVER_H_
#define AUTONOMOUSCONTROLSYSTEM_INC_MPU9250_DRIVER_H_

/******************************************************************************
*                               Includes                                      *
 *****************************************************************************/
#include "../MPU9250/HAL/hal.h"
#include "../MPU9250/libs/myLib.h"
#include "../MPU9250/mpu9250/mpu9250.h"
#if defined(__HAL_USE_MPU9250_DMP__)       //  Compile only if module is enabled
#endif
#include "../MPU9250/HAL/hal.h"
#include "../MPU9250/libs/myLib.h"
#include "../MPU9250/libs/helper_3dmath.h"
#include "../MPU9250/mpu9250/eMPL/inv_mpu.h"
#include "../MPU9250/mpu9250/eMPL/inv_mpu_dmp_motion_driver.h"
#include "../MPU9250/mpu9250/registerMap.h"

/******************************************************************************
*                                 Needed Types                                *
 *****************************************************************************/
typedef struct {
    MPU9250 *M;
    float rpy[3];
    float xyz[3];
    uint32_t counter;
} MyMPU;

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
#endif /* AUTONOMOUSCONTROLSYSTEM_INC_MPU9250_DRIVER_H_ */
