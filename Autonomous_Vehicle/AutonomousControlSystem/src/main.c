/**********************************************************
 *                                                        *
 *      Cairo University Shell Eco-Racing Team            *
 *      Shell Eco Marathon 2021 Autonomous System         *
 *      Embedded Autonomous Movement Control Sub-team     *
 *                                                        *
 **********************************************************/

 /******************************************************************************
 *
 * File Name:   main.c
 *
 * Description: main source file to initialize and start the Autonomous System
 *
 * Date:        10/2/2020
 *
 ******************************************************************************/

#include <AutonomousControlSystem/inc/common_includes.h>
#include <AutonomousControlSystem/inc/state_decoding.h>
#include <AutonomousControlSystem/inc/steering_tasks.h>
#include <AutonomousControlSystem/inc/throttle_tasks.H>
#include <AutonomousControlSystem/inc/USB_tasks.h>
#include <AutonomousControlSystem/inc/QEI.h>
#include <AutonomousControlSystem/inc/debug.h>
#include <AutonomousControlSystem/inc/PIDThrottle.h>
#include <AutonomousControlSystem/src/MPU9250/mpu9250_tasks.h>
#include "inc/hw_uart.h"

/* Tiva-Ware Drivers Includes */
#include "driverlib/interrupt.h"
#include "driverlib/uart.h"
#include "driverlib/usb.h"
#include "usblib/usblib.h"
#include "usblib/usbcdc.h"
#include "usblib/usb-ids.h"
#include "usblib/device/usbdevice.h"
#include "usblib/device/usbdcdc.h"
#include "utils/ustdlib.h"

/* Queues handles declarations */
QueueHandle_t Queue_Desired_Orientation;
QueueHandle_t Queue_Throttle_Orientation;
QueueHandle_t Queue_Speed;
QueueHandle_t Queue_Feedback;
QueueHandle_t Queue_Measurement_From_Wheel_Enchoder;
QueueHandle_t Queue_Measurement;

SemaphoreHandle_t QEI_Block_Sem;


int main(void)
{
    /* Enable lazy stacking for interrupt handlers. This allows floating-point
     * instructions to be used within interrupt handlers, but at the expense of
     * extra stack usage. */
    MAP_FPULazyStackingEnable();
    MAP_FPUEnable();

    /* Set the clocking to run at 80 MHz from the PLL. */
    MAP_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    /* Enable Global Interrupt-bit */
    MAP_IntMasterEnable();

    /* Creating the Queues and storing their addresses in their handles */
    Queue_Desired_Orientation = xQueueCreate(1,4);
    Queue_Throttle_Orientation = xQueueCreate(1,4);
    Queue_Speed = xQueueCreate(1,5);
    Queue_Measurement = xQueueCreate(1,5);
    Queue_Measurement_From_Wheel_Enchoder = xQueueCreate(1,5);
    Queue_Feedback = xQueueCreate(1,5);


    /* Initializing System's modules */
    init_steering_tasks();
    init_throttle_tasks();

    vInit_USBTasks();

    UART0_init();
    vPID_Init();


    //QEI1_Init();
    //vInit_MPU9250_Tasks();

    /* Run the Kernel's Scheduler */
    vTaskStartScheduler();

    while(1);
}
