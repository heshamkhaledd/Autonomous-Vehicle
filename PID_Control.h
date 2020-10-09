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
#define Max_Steering 360
#define Min_Steering -360

#define Max_Orientation 45
#define Min_Orientation -45

#define Time_Interval 0.1

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
