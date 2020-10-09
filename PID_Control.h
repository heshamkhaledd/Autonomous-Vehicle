 /******************************************************************************
 *
 * File Name:   PID_Control.h
 *
 * Description: PID feedback control header file
 *
 * Date:        10/2/2020
 *
 ******************************************************************************/

#ifndef PID_CONTROL_H_
#define PID_CONTROL_H_


#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <semphr.h>
#include <queue.h>

#include <stdint.h>



/*******************************************************************************
 *                           Preprocessor Definies                             *
 *******************************************************************************/

/* Configurations */
#define Time_Interval 0.1

/* steering Configurations */
#define ORIENT_TO_STEERING_PARAM 24
#define OVERLAP_CORRECTION_FACTOR 360.0f
#define MAX_STEERING 360
#define MIN_STEERING -360

/****************************************************************
 *                       Structures                             *
*****************************************************************/
/*Struct for the PID controller that hols its constants and parameters*/
typedef struct{
    /*controller constants*/
    float Kp;
    float Ki;
	float Kd;

    /*Controller parameters*/
    float accumlativeError;
    float lastError;

} PIDcontroller;

/*******************************************************************************
 *                          Functions Prototypes                               *
 *******************************************************************************/
extern float PID_control (PIDcontroller* controller,float SP , float PV);


#endif /* PID_CONTROL_H_ */
