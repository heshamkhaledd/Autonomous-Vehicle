 /******************************************************************************
 *
 * File Name:   PID_Control.c
 *
 * Description: PID feedback control source file
 *
 * Date:        10/2/2020
 *
 ******************************************************************************/
#include "PID_Control.h"


/******************************************************************************
 *
 * Function Name: Steering_Saturation
 *
 * Description: Checks if the Steering output is adjusted to the max/min motor steering
 *              to be within the acceptable steering range.
 *
 * Arguments:   float a_SteeringOutput
 * Return:      float Max_Steering || float Steering_Output
 *
 *****************************************************************************/
static float Steering_Saturation (float a_SteeringOutput)
{
    a_SteeringOutput=a_SteeringOutput>MAX_STEERING ? MAX_STEERING:a_SteeringOutput;
    a_SteeringOutput=a_SteeringOutput<MIN_STEERING ? MIN_STEERING:a_SteeringOutput;

    return a_SteeringOutput ;
}

/***********************************************************************************
 *
 * Name : f_DecodingOrientIntoSteering
 *
 * Description: This function aims to decode or get the steering angle to be excuted from
 *              the desired orientation.
 *              It also checks 
 *              There are many approximations according to field experiments as maximum steering
 *              we can get from stepper is 360 degree which is almost equvilant to 45 degree
 *              orientation wise.
 *              The decoding process depends on factor (ORIENT_TO_STEERING_PARAM) to map from
 *              orientation into steering and this factor also hold ratio of losses in mechanical
 *              system during excuting steering.
 *
 * Arguments:   float f_Desired_Orientation
 *
 * Return:      float Steering_Degrees
 *
 ************************************************************************************/
/*TODO:: change ORIENT_TO_STEERING_PARAM*/
static float f_DecodeOrientationIntoSteering (float f_Desired_Orientation )
{
    float Steering_Degrees ;

    if ( (long) f_Desired_Orientation > 180)
    {
        Steering_Degrees = (((float)ORIENT_TO_STEERING_PARAM ) * (f_Desired_Orientation - OVERLAP_CORRECTION_FACTOR )) ;
    }
    else if ( (long) f_Desired_Orientation < -180)
    {
        Steering_Degrees = (((float)ORIENT_TO_STEERING_PARAM ) * (f_Desired_Orientation + OVERLAP_CORRECTION_FACTOR )) ;
    }
    else
    {
        Steering_Degrees = (((float)ORIENT_TO_STEERING_PARAM ) * (f_Desired_Orientation )) ;
    }

    return Steering_Degrees ;
}

/******************************************************************************
 *
 * Function Name: f_PID_Steering
 *
 * Description: PID Control Calculation function.
 *
 * Arguments:   float a_setPoint , float a_currentValue 
 * Return:      float motorSteeringDegrees
 *
 *****************************************************************************/
float PID_control (PIDcontroller* a_controller,float a_currentValue , float a_setPoint )
{
    float currentError ;
    float derivativeError ;
    float pidOutput ;
    float motorSteeringDegrees ;

    /* Calculating P component*/
    currentError = a_setPoint - a_currentValue ;

    /*Calculating I component*/
    a_controller->accumlativeError += (currentError * Time_Interval) ;

    /*Calculating D component*/
    derivativeError = (currentError-(a_controller->lastError)) / Time_Interval ;

    /*PID output*/
    pidOutput = currentError * a_controller->Kp +  a_controller->accumlativeError * a_controller->Ki + derivativeError * a_controller->Kd ;

    /*Change orientation degrees to steering and checks for direction*/
    motorSteeringDegrees = f_DecodeOrientationIntoSteering(pidOutput);

    /*Steering saturation Function */
    motorSteeringDegrees = Steering_Saturation(motorSteeringDegrees) ;

    /*updating controller's last Error*/
    a_controller->lastError = currentError;

    return motorSteeringDegrees;
}
