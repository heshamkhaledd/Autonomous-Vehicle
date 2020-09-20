/*
 * Orientation.h
 *
 *  Created on: ???/???/????
 *      Author: Tefa
 */

#ifndef ORIENTATION_H_
#define ORIENTATION_H_


#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <semphr.h>
#include <queue.h>

#include <stdint.h>


/***********************************************
 *             Dynamic Configurations
 ***********************************************/

#define ORIENT_TO_STEERING_PARAM 24
#define OVERLAP_CORRECTION_FACTOR 360.0f

/***********************************************
 *             Static Configurations
 ***********************************************/

#define Numbers_Ascii_Base 0x30
#define Numbers_Ascii_Offset 9

/***********************************************
 *                 Global Variables
 ***********************************************/

/*   Debuging variables   */
/*extern  long test_overlap ;
extern  float watch_steering_overlap ;*/

/***********************************************
 *                 Global Functions
 ***********************************************/

extern float vGet_OrientationRef(void);
extern float f_DecodingOrientIntoSteering (float f_Desired_Orientation );


#endif /* ORIENTATION_H_ */
