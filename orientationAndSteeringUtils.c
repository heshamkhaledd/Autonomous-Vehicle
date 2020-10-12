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
 * Description: Function that checks if the current orientation wrapped around the 
 *              discontinuity point (-180/+180) and adjusts the desired orientation 
 *              so that the error calculation in PID is correct.
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
 * Description: Function that adjusts desired orientation according to type of wrap around
 *
 * Arguments:   a_wrapAroundFlag,currentOrientation,a_desiredOrientationPtr
 * Return:      void
 *
 *****************************************************************************/
void v_adjustDesiredOrientaion(WRAP_AROUND_FLAG a_wrapAroundFlag, float currentOrientation, float * a_desiredOrientationPtr)
{
    /*Positive to negative wrap around*/
    if(a_wrapAroundFlag==POSITIVE_TO_NEGATIVE_WRAP_AROUND && CHECK_NEGATIVE_ANGLE(currentOrientation))
        *a_desiredOrientationPtr=(*a_desiredOrientationPtr) - WRAP_AROUND_CORRECTION_FACTOR;

    /*Negative to positive wrap around*/
    else if(a_wrapAroundFlag==NEGATIVE_TO_POSITIVE_WRAP_AROUND && CHECK_POSITIVE_ANGLE(currentOrientation))
        *a_desiredOrientationPtr=(*a_desiredOrientationPtr) + WRAP_AROUND_CORRECTION_FACTOR;
}
