 /******************************************************************************
 *
 * File Name:   PID_control.c
 *
 * Description: PID feedback control source file
 *
 * Date:        10/2/2020
 *
 ******************************************************************************/
#include <AutonomousControlSystem/inc/PID_controller.h>

/******************************************************************************
 *
 * Function Name: f_PID_Steering
 *
 * Description: PID Control Calculation function.
 *
 * Arguments:   float a_controller, a_setPoint , float a_currentValue 
 * Return:      float motorSteeringDegrees
 *
 *****************************************************************************/
float f_PID_control (PIDcontroller* a_controller,float a_currentValue , float a_setPoint )
{
    float currentError ;
    float derivativeError ;
    float pidOutput ;

    /* Calculating P component*/
    currentError = a_setPoint - a_currentValue ;

    /*Calculating I component*/
    a_controller->accumlativeError += (currentError * Time_Interval) ;

    /*Calculating D component*/
    derivativeError = (currentError-(a_controller->lastError)) / Time_Interval ;

    /*PID output*/
    pidOutput = currentError * a_controller->Kp +  a_controller->accumlativeError * a_controller->Ki + derivativeError * a_controller->Kd ;

    /*updating controller's last Error*/
    a_controller->lastError = currentError;

    return pidOutput;
}
