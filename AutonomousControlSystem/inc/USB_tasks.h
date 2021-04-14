/*
 * USB_tasks.h
 *
 *  Created on: ???/???/????
 *      Author: Tefa
 */

#ifndef AUTONOMOUSCONTROLSYSTEM_INC_USB_TASKS_H_
#define AUTONOMOUSCONTROLSYSTEM_INC_USB_TASKS_H_

#include <AutonomousControlSystem/inc/common_includes.h>
#include <AutonomousControlSystem/inc/state_decoding.h>
#include <AutonomousControlSystem/inc/state_decoding.h>
#include <AutonomousControlSystem/inc/usb_serial_structs.h>
#include "driverlib/usb.h"
#include "usblib/usblib.h"
#include "usblib/usbcdc.h"
#include "usblib/usb-ids.h"
#include "usblib/device/usbdevice.h"
#include "usblib/device/usbdcdc.h"

/*****************************************************************
 *                      Static Configuration
 *****************************************************************/

#define USBReceive_prio 1
#define USBTransmit_prio 1


//needs refining after the final code
#define USB_StackDepth 200

/*****************************************************************
 *                       Global Variables
 *****************************************************************/

extern uint8_t * uint8_USBTx  ;
extern uint8_t * uint8_USBRx  ;

extern volatile UBaseType_t USBRecieveHighWaterMark ;

/*****************************************************************
 *                       Semaphores
 *****************************************************************/

extern SemaphoreHandle_t Sem_USBReceive ;

/*****************************************************************
 *                    TASKS Declaration
 *****************************************************************/

extern void vTASK_USBReceive (void *) ;
extern void vTASK_USBTransmit (void *) ;
extern void vInit_USBTasks() ;



#endif /* AUTONOMOUSCONTROLSYSTEM_INC_USB_TASKS_H_ */
