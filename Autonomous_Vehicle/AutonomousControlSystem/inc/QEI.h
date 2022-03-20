#include <AutonomousControlSystem/inc/common_includes.h>
#include <AutonomousControlSystem/inc/common_macros.h>
#include <AutonomousControlSystem/inc/debug.h>

// Comment this line if Terminal debugging (PuTTY) isn't required.
#define MEASUREMENT_FROM_ENCODER
//#define DEBUG_QEI
//#define MATLAB_DRAWING

#define VEL_INT_FREQ         2.0f        // Macro to store the Interrupt frequency of QEI1. will be changed according to need of PID.
#define QEI1_PPR             103          // Macro to store the PPR of the QEI1.
#define Wheel_Circumference  1.7522787f  // As car's diameter was measured manually to be equal to ... meters,
// hence, circumference = 2*PI*r = PI*d = ...

/* Queue to send the speed to the PID */
extern QueueHandle_t Queue_Measurement;


/* -----------------------      Global Variables        --------------------- */
// volatile int32_t   i32QEI1Dir;        // Variable to store the direction of QEI1

volatile uint32_t Pulses_per_period;     // Variable to store the number of pulses accumulated through Velocity Timer's Period in QE1
volatile uint32_t Total_pulses;          // Variable to store the number of pulses accumulated since the Vehicle started moving

volatile float Distance_per_period;      // Value in meters
volatile float Total_distance;           // Value in meters

 float Velocity_km_per_hour;


 // float New_speed=0;
 // float Old_speed=0;

/* -----------------------      Function Prototypes     --------------------- */
void QEI1IntHandler(void);
void QEI1_Init (void);
