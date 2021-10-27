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
#include <AutonomousControlSystem/inc/UART.h>
#include <AutonomousControlSystem/inc/UART_tasks.h>
#include <AutonomousControlSystem/inc/USB_tasks.h>
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

/* Configurations Includes */






/* Queues handles declarations */
QueueHandle_t Queue_Desired_Orientation;
QueueHandle_t Queue_Current_Orientation;
QueueHandle_t Queue_Throttle_Orientation;
QueueHandle_t Queue_Feedback;


int main(void)
{
    /* Enable lazy stacking for interrupt handlers. This allows floating-point
     * instructions to be used within interrupt handlers, but at the expense of
     * extra stack usage. */
    MAP_FPULazyStackingEnable();
    MAP_FPUEnable();

    MAP_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); /* Set the clocking to run at 80 MHz from the PLL. */

    MAP_IntMasterEnable();                                                                      /* Enable Global Interrupt-bit */

    SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_EEPROM0)){}
    EEPROMInit();

    /* Creating the Queues and storing their addresses in their handles */
    Queue_Current_Orientation = xQueueCreate(1,4);
    Queue_Desired_Orientation = xQueueCreate(1,4);
    Queue_Throttle_Orientation = xQueueCreate(1,4);
    Queue_Feedback = xQueueCreate(1,5);

    /* Initializing System's modules */
    vInit_Steppers_Tasks();
    vInit_throttle_Tasks();
    vInit_USBTasks();
    //UART1_Init(115200);
    //vInit_UART();

        /* Prototype for xTaskCreate:
        *
        *  BaseType_t xTaskCreate( TaskFunction_t pvTaskCode,
        *                          const char * const pcName,
        *                          uint16_t usStackDepth,
        *                          void *pvParameters,
        *                          UBaseType_t uxPriority,
        *                          TaskHandle_t *pvCreatedTask);
        */

    vTaskStartScheduler();  /* Run the Kernel's Scheduler */

    /* UNREACHABLE CODE */
    while(1);

}
