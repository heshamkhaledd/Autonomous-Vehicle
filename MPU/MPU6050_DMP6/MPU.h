 /****************************************************************************************************
 *
 * Module: MPU
 *
 * File Name: MPU.h
 *
 * Description: Header file for the MPU
 * I2C device class (I2Cdev) demonstration Arduino sketch for MPU6050 class using DMP (MotionApps v2.0)
 *  I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
 *  for both classes must be in the include path of your project . 
 *  In addition to connection 3.3v, GND, SDA, and SCL, this sketch
 *  depends on the MPU-6050's INT pin being connected to the Arduino's
 *  external interrupt #0 pin. On the Arduino Uno and Mega 2560, this is
 *  digital I/O pin 2.
 * ***************************************************************************************************/
#ifndef MPU_H 
#define MPU_H 

/******************************************************************************************************
 *                                     PREPROCESSOR FILES
 * ****************************************************************************************************/
   #include "I2Cdev.h"
   #include "MPU6050_6Axis_MotionApps20.h"
   /* not necessary if using MotionApps include file */
      //#include "MPU6050.h" 
   /* Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation is used in I2Cdev.h */
   #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
     #include "Wire.h"
   #endif 

 /* Initiliaze Class,  class default I2C address is 0x68 */ 
           MPU6050 mpu;

 /******************************************************************************************************
 *                                     PREPROCESSOR DEFINES
 * ****************************************************************************************************/        
   #define OUTPUT_READABLE_YAWPITCHROLL
   /*(Arduino is 13, Teensy is 11, Teensy++ is 6) */
   #define LED_PIN 13 
   

#endif 