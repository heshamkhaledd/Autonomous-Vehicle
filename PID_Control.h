/*
 * PID_Control.h
 *
 *  Created on: ???/???/????
 *      Author: Tefa
 */

#ifndef PID_CONTROL_H_
#define PID_CONTROL_H_


#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <semphr.h>
#include <queue.h>

#include <stdint.h>



/***********************************************
 *                 Configurations
 ***********************************************/

#define P_Constant_Steering 0.8
#define I_Constant_Steering 0
#define D_Constant_Steering 0

#define Max_Steering 360
#define Min_Steering -360

#define Max_Orientation 45
#define Min_Orientation -45

#define Time_Interval 0.1

/***********************************************
 *                 Global Functions
 ***********************************************/

extern float f_PID_Steering (float SP , float PV ,float * Accumlative_Error , float * Last_Error);


#endif /* PID_CONTROL_H_ */
