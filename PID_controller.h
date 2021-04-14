 /******************************************************************************
 *
 * File Name:   PID_control.h
 *
 * Description: PID feedback control header file
 *
 * Date:        10/2/2020
 *
 ******************************************************************************/
#ifndef PID_CONTROLLER_H_
#define PID_CONTROLLER_H_


#include "common_includes.h"

#include <stdint.h>



/*******************************************************************************
 *                           Preprocessor Definies                             *
 *******************************************************************************/

/* Configurations */
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
extern float f_PID_control (PIDcontroller* controller,float SP , float PV);


#endif /* PID_CONTROLLER_H_ */
