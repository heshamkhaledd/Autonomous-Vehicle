/*
 * USB_tasks.h
 *
 *  Created on: ???/???/????
 *      Author: Tefa
 */

#ifndef USB_TASKS_H_
#define USB_TASKS_H_

//freeRTOS includes

#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <semphr.h>
#include <queue.h>


//CCS Default includes

#include <stdint.h>
#include <stdbool.h>


//TivaWARE minimal includes

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "inc/hw_sysctl.h"

#include "inc/hw_uart.h"




#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"

#include "driverlib/uart.h"
#include "driverlib/usb.h"



#include "usblib/usblib.h"
#include "usblib/usbcdc.h"
#include "usblib/usb-ids.h"
#include "usblib/device/usbdevice.h"
#include "usblib/device/usbdcdc.h"
#include "utils/ustdlib.h"
#include "utils/uartstdio.h"


#include "usb_serial_structs.h"

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
extern SemaphoreHandle_t Sem_USBTransmit ;

/*****************************************************************
 *                    TASKS Declaration
 *****************************************************************/

extern void vTASK_USBReceive (void *) ;
extern void vTASK_USBTransmit (void *) ;
extern void vInit_USBTasks(void (* Ptr_TxHandler)(void), void (* Ptr_RxHandler)(void)) ;



#endif /* USB_TASKS_H_ */
