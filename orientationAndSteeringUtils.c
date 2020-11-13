 /******************************************************************************
 *
 * File Name:   orientationAndSteeringUtils.c
 *
 * Description: source file for utility functions and definitions
 *              Related to orientation and steering
 *
 * Date:        10/12/2020
 *
 ******************************************************************************/
#include "orientationAndSteeringUtils.h"


/******************************************************************************
 *
 * Function Name: int8_getOrientationWrapAroundFlag
 *
 * Description: Function that checks if the desired motion will cause a wrapping around 
 *              the discontinuity point (-180/+180 degrees) and return a flag that defines 
 *              the wrap-around type.
 *
 * Arguments:   a_currentOrientation, float a_desiredOrienatation
 * Return:      int_8 wrap around flag
 *
 *****************************************************************************/
WRAP_AROUND_FLAG int8_getOrientationWrapAroundFlag(float a_currentOrientation, float a_desiredOrienatation)
{
    /*check for positive to negative wrap around*/
    if(CHECK_POSITIVE_ANGLE(a_currentOrientation) && a_desiredOrienatation>180)
        return POSITIVE_TO_NEGATIVE_WRAP_AROUND;
    
    /*check for negative to positive wrap aorund*/
    if(CHECK_NEGATIVE_ANGLE(a_currentOrientation) && a_desiredOrienatation<-180)
        return NEGATIVE_TO_POSITIVE_WRAP_AROUND;
    
    /*No wrap around, return -1*/
    return -1;
}

/******************************************************************************
 *
 * Function Name: v_adjustDesiredOrientaion
 *
 * Description: Function that adjusts desired orientation according to type of wrap-around
 *
 * Arguments:   a_wrapAroundFlag,currentOrientation,a_desiredOrientationPtr
 * Return:      void
 *
 *****************************************************************************/
void v_adjustDesiredOrientaion(WRAP_AROUND_FLAG a_wrapAroundFlag, float currentOrientation, float * a_desiredOrientationPtr)
{
    /*A positive to negative wrap around flag is raised and angle wraps around 180*/
    if(a_wrapAroundFlag==POSITIVE_TO_NEGATIVE_WRAP_AROUND && CHECK_NEGATIVE_ANGLE(currentOrientation))
        *a_desiredOrientationPtr=(*a_desiredOrientationPtr) - WRAP_AROUND_CORRECTION_FACTOR;

    /*A enegative to positive wrap around flag is raised and angle wraps around 180*/
    else if(a_wrapAroundFlag==NEGATIVE_TO_POSITIVE_WRAP_AROUND && CHECK_POSITIVE_ANGLE(currentOrientation))
        *a_desiredOrientationPtr=(*a_desiredOrientationPtr) + WRAP_AROUND_CORRECTION_FACTOR;
}

/***********************************************************************************
 *
 * Name : f_DecodingOrientIntoSteering
 *
 * Description: Function that aims to decode the desired orientation outputed
 *              by PID controller to motor steering steps
 *
 * Arguments:   float a_desiredOrientation
 *
 * Return:      float Steering_Degrees
 *
 ************************************************************************************/
float f_DecodeOrientationIntoSteering (float a_desiredOrientation)
{
    /*Check for wheel orientation saturartion*/
    a_desiredOrientation=a_desiredOrientation>MAX_ORIENTATION ? MAX_ORIENTATION:a_desiredOrientation;
    a_desiredOrientation=a_desiredOrientation<MIN_ORIENTATION ? MIN_ORIENTATION:a_desiredOrientation;

    float steeringDegrees=a_desiredOrientation * (float)ORIENT_TO_STEERING_PARAM;
    
    /*Multiplying by motor factor*/
    steeringDegrees= (long)((steeringDegrees) / STEERING_STEP);
    return steeringDegrees ;
}
