 /******************************************************************************
 *
 * File Name:   Orientation.h
 *
 * Description: Orientation adjustment header file
 *
 * Date:        10/2/2020
 *
 ******************************************************************************/

#ifndef ORIENTATION_H_
#define ORIENTATION_H_


#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <semphr.h>
#include <queue.h>

#include <stdint.h>



/*******************************************************************************
 *                           Preprocessor Definies                             *
 *******************************************************************************/

/* Dynamic Configurations */
#define ORIENT_TO_STEERING_PARAM 24
#define OVERLAP_CORRECTION_FACTOR 360.0f

/* Static Configurations */
#define Numbers_Ascii_Base 0x30
#define Numbers_Ascii_Offset 9

/*******************************************************************************
 *                          Functions Prototypes                               *
 *******************************************************************************/

extern float vGet_OrientationRef(void);
extern float f_DecodingOrientIntoSteering (float f_Desired_Orientation );


#endif /* ORIENTATION_H_ */
