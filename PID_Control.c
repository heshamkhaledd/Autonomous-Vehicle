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
#include "Orientation.h"


/******************************************************************************
 *
 * Function Name: Steering_Saturation
 *
 * Description: Checks if the Steering output is adjusted to the max/min steering
 *              to be within the acceptable steering range.
 *
 * Arguments:   float Steering_Output
 * Return:      float Max_Steering || float Steering_Output
 *
 *****************************************************************************/
static float Steering_Saturation (float Steering_Output)
{
    if (Steering_Output > Max_Steering)
    {
        return Max_Steering ;
    }
    else if (Steering_Output < Min_Steering)
    {
        return Min_Steering ;
    }
    else
    {
        return Steering_Output ;
    }
}

/******************************************************************************
 *
 * Function Name: Orientation_Saturation
 *
 * Description: Checks if the Orientation output is adjusted to the max/min
 *              orientation to be within the acceptable orientation range.
 *
 * Arguments:   float Steering_Output
 * Return:      float Max_Steering || float Steering_Output
 *
 *****************************************************************************/
static float Orientation_Saturation (float Orientation_Output)
{
    if (Orientation_Output > Max_Orientation)
    {
        return Max_Orientation ;
    }
    else if (Orientation_Output < Min_Orientation)
    {
        return Min_Orientation ;
    }
    else
    {
        return Orientation_Output ;
    }
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

    /*Change orientation degrees to steering */
    motorSteeringDegrees = f_DecodingOrientIntoSteering(pidOutput);

    /*Steering saturation Function */
    motorSteeringDegrees = Steering_Saturation(motorSteeringDegrees) ;

    /*updating controller's last Error*/
    a_controller->lastError = currentError;

    return motorSteeringDegrees;
}
