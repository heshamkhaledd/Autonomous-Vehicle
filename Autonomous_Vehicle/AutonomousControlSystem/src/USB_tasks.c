/******************************************************************************
 *
 * File Name:   USB_tasks.c
 *
 * Description: USB source file to initialize USB module tasks
 *
 * Date:        10/2/2020
 *
 ******************************************************************************/
#include <AutonomousControlSystem/inc/USB_tasks.h>

/* If we are testing using laptop (PuTTY) connected to TivaC, define this macro */
//#define TESTING_ON_LAPTOP

/* If we connect the board tQSo Xavier we need to define the received packet size,
  initially it is 10 bytes (XXXoXXT) X stands for ASCII numbers or minus sign */
/* Note: We can change it from here. */

/*#define PACKET_SIZE 16*/

/* Declaring Semaphores Handles */
SemaphoreHandle_t Sem_USBReceive;
SemaphoreHandle_t Sem_USBTransmit;

uint8_t dataFromHost1[8];
uint8_t dataFromHost2[8];
uint8_t dataFromHost3[3];

bool g_bUSBConfigured = false;

#define TESTING_ON_LAPTOP
/******************************************************************************
 *
 * Function Name: USB_receiveString
 *
 * Description: Responsible of receiving frame of testing on laptop for rotating
 *              and throttling in the string Str.
 *
 * Arguments:   uint8_t *Str
 * Return:      void
 *
 *****************************************************************************/
void USB_receiveString(uint8_t *Str)
{
    /* Reading the value sent from PC,
      this reads the latest 5 bytes received in the buffer*/
    USBBufferRead((tUSBBuffer *)&g_sRxBuffer, Str, 5);

    /* If the latest character in the frame is "ENTER"
       (As Echo in PuTTY is forced on), replace it with the NULL character,
       if not, then the sent frame contains errors,
       so flush what is inside the buffer */
    if (*(Str+4)== 13 )
    {
        *(Str+4) = '\0';
    }
    else
    {
        USBBufferFlush((tUSBBuffer *)&g_sRxBuffer);
    }
}

/******************************************************************************
 *
 * Function Name: USB_GetLineCoding
 *
 * Description: Responsible for setting the USB serial configurations.
 *
 * Arguments:   tLineCoding *psLineCoding
 * Return:      void
 *
 *****************************************************************************/
void USB_GetLineCoding(tLineCoding *psLineCoding)
{
    /*115200 baud rate*/
    psLineCoding->ui32Rate =115200;
    /*one stop bit*/
    psLineCoding->ui8Stop = USB_CDC_STOP_BITS_1;
    /*8-bit data*/
    psLineCoding->ui8Databits = 8;
    /*no parity bits*/
    psLineCoding->ui8Parity =USB_CDC_PARITY_NONE;
}

/******************************************************************************
 *
 * Function Name: USB_HardwareConfiguration
 *
 * Description: Responsible for initializing the USB module for a specific
 *              configuration.
 *
 * Arguments:   void
 * Return:      void
 *
 *****************************************************************************/
static void USB_HardwareConfiguration (void )
{
    /* Configure the required pins for USB operation AS AN ANALOG function pins.*/
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);                    /* Enable clock to GPIOD */
    MAP_GPIOPinTypeUSBAnalog(GPIO_PORTD_BASE, GPIO_PIN_5 | GPIO_PIN_4); /* PD4(PIN43):= (USB0D-) , PD5(PIN44):= (USB0D+) */

    /*Initialize Tx (Transmit) and Rx (Receive) Buffers */
    USBBufferInit(&g_sTxBuffer);
    USBBufferInit(&g_sRxBuffer);

    /* Configure the TivaC to be device(slave) mode on the bus) */
    USBStackModeSet(0, eUSBModeForceDevice, 0);

    /* Configure handshaking to  start (Enum) */
    USBDCDCInit(0, &g_sCDCDevice);

    /*Enable USB0 Interrupt*/
    ROM_IntEnable(INT_USB0);
}

/*****************************************************************
 *                       Callback Routines
 *****************************************************************/

uint32_t ControlHandler(void *pvCBData, uint32_t ui32Event,uint32_t ui32MsgValue, void *pvMsgData)
{
    tLineCoding *pvdata = pvMsgData;

    switch(ui32Event)
    {
    case USB_EVENT_CONNECTED:
        g_bUSBConfigured = true;
        /*flushing the buffers*/
        USBBufferFlush(&g_sTxBuffer);
        USBBufferFlush(&g_sRxBuffer);
        break;

    case USB_EVENT_DISCONNECTED:
        g_bUSBConfigured = false;
        break;

    case USBD_CDC_EVENT_GET_LINE_CODING:
    case USBD_CDC_EVENT_SET_LINE_CODING:
        /*setting the USB serial configurations*/
        USB_GetLineCoding(pvdata);
        break;

    default:
        break;
    }
    return 0;
}

/*****************************************************************
 *                       Transmit Handler
 *****************************************************************/
uint32_t TxHandler(void *pvCBData, uint32_t ui32Event, uint32_t ui32MsgValue,void *pvMsgData)
{
    /* We don't need to do anything here, as this handler
       will be called after we send the data in the Transmit task */
    return(0);  
}

/*****************************************************************
 *                       Receive Handler
 *****************************************************************/
uint32_t RxHandler(void *pvCBData, uint32_t ui32Event, uint32_t ui32MsgValue,void *pvMsgData)
{

#ifndef TESTING_ON_LAPTOP

    uint8_t bytegotfromusb;

    USBBufferRead((tUSBBuffer*) &g_sRxBuffer, &bytegotfromusb, 1);

    if (bytegotfromusb == 'x')
    {
        /*Give Transmit semaphore here to send feedback*/
        xSemaphoreGiveFromISR(Sem_USBTransmit, NULL);
    }
    else if(bytegotfromusb == 'F')
    {
        USBBufferRead((tUSBBuffer*) &g_sRxBuffer, dataFromHost1, 7);
        USBBufferRead((tUSBBuffer*) &g_sRxBuffer, dataFromHost2, 7);
        USBBufferRead((tUSBBuffer*) &g_sRxBuffer, dataFromHost3, 2);

        dataFromHost1[7] = '\0';
        dataFromHost2[7] = '\0';
        dataFromHost3[2] = '\0';
        /*we have a new packet incoming*/
    }

#endif

#ifdef TESTING_ON_LAPTOP

    /* Checking the receive event to give the semaphore */
    if(ui32Event == USB_EVENT_RX_AVAILABLE)
    {
        /* Only give the semaphore when the buffer contains 5 bytes according to the frame XXXR or XXXT */
        if(ui32MsgValue == 5)
        {
            xSemaphoreGiveFromISR(Sem_USBReceive, NULL);
        }
    }

#endif

    return(0);
}


/******************************************************************************
 *
 * Function Name: vTASK_USBReceive
 *
 * Description: Responsible for initializing the USB module for a specific
 *              configuration.
 *
 * Arguments:   void
 * Return:      void
 *
 *****************************************************************************/

#ifdef TESTING_ON_LAPTOP

void vTASK_USBReceive (void *pvParameters)
{
    /* Array to store the data sent form the USB */
    uint8_t dataFromHost[arr_size];

    while(1)
    {
        /* Wait until USB receives a new frame */
        xSemaphoreTake(Sem_USBReceive,portMAX_DELAY);

        /* Receive valid frame only in the array */
        USB_receiveString(dataFromHost);

        /* If the frame is valid, then send to the stepper motor through state_decoding */
        if (dataFromHost[arr_size-1]=='\0')
        {
            switch (dataFromHost[arr_size-2])
            {
            case 'R':
            case 'r':
            case 'T':
            case 't':
                state_decoding (dataFromHost);
                break;
            }
        }
    }
}
#endif

#ifndef TESTING_ON_LAPTOP

void vTASK_USBReceive (void *pvParameters)
{
    while(1)
    {
        xSemaphoreTake(Sem_USBReceive,portMAX_DELAY);

        state_decoding (dataFromHost1);
        state_decoding (dataFromHost2);
        state_decoding (dataFromHost3);

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
#endif

/******************************************************************************
 *
 * Function Name: vTASK_USBTransmit
 *
 * Description:
 *
 *
 * Arguments:   void
 * Return:      void
 *
 *****************************************************************************/
void vTASK_USBTransmit (void * params)
{
#warning ("Not yet finished")

    /*We need a queue here to communicate with the received data from UART "in a string format" 
    to send it back to the processor*/

    vTaskSuspend(NULL);

    uint8_t FeedbackDataToTransmit[5];

    while(1)
    {
        xQueueReceive(Queue_Feedback, (void *)FeedbackDataToTransmit, portMAX_DELAY);

        xSemaphoreTake(Sem_USBTransmit,portMAX_DELAY);

        USBBufferWrite((tUSBBuffer *)&g_sTxBuffer,FeedbackDataToTransmit,5);
    }
}

/******************************************************************************
 *
 * Function Name: vInit_USBTasks
 *
 * Description: Responsible Creating USB Tasks (vTASK_USBReceive, vTASK_USBTransmit)
 *
 * Arguments:   void
 * Return:      void
 *
 *****************************************************************************/
void vInit_USBTasks()
{
    /* Getting Hardware Ready */
    USB_HardwareConfiguration();

    /* Creating Semaphores */
    Sem_USBReceive = xSemaphoreCreateBinary();
    Sem_USBTransmit= xSemaphoreCreateBinary();

    /* Creating Tasks */
    xTaskCreate(vTASK_USBReceive,                       /* Task Address       */
                "USB_Recieve_task",                     /* Task Name          */
                USB_StackDepth,                         /* Size of the stack. */
                NULL,                                   /* Task Parameters    */
                configMAX_PRIORITIES-USBReceive_prio,   /* Task Priority      */
                NULL);

    xTaskCreate(vTASK_USBTransmit,
                "USB_Transmit_task",
                USB_StackDepth,
                NULL,
                configMAX_PRIORITIES-USBTransmit_prio,
                NULL);
}
