
#include "HAL/hal.h"
#include "libs/myLib.h"

//#include "serialPort/uartHW.h"
#include "serialPort/UARTs.h"

//-------------------------------------------MPU9250 FILE --------------------------------------------------------------//
#include "mpu9250/mpu9250.h"

#if defined(__HAL_USE_MPU9250_DMP__)       //  Compile only if module is enabled

#include "HAL/hal.h"
#include "libs/myLib.h"
#include "libs/helper_3dmath.h"

#include "mpu9250/eMPL/inv_mpu.h"
#include "mpu9250/eMPL/inv_mpu_dmp_motion_driver.h"

#include "mpu9250/registerMap.h"

//  Enable debug information printed on serial port
#define __DEBUG_SESSION__

//  Sensitivity of sensor while outputting quaternions
#define QUAT_SENS  1073741824.0f


#ifdef __DEBUG_SESSION__
#include "serialPort/UARTs.h"
#endif


///-----------------------------------------------------------------------------
///         DMP related structs --  Start
///-----------------------------------------------------------------------------

/* The sensors can be mounted onto the board in any orientation. The mounting
 * matrix seen below tells the MPL how to rotate the raw data from thei
 * driver(s).
 * TODO: The following matrices refer to the configuration on an internal test
 * board at Invensense. If needed, please modify the matrices to match the
 * chip-to-body matrix for your particular set up.
 */
//  In this case chip is rotated -90° around Y axis
//static signed char gyro_orientation[9] = { 0, 0, -1,
//                                           0, 1,  0,
//                                           1, 0,  0};
static signed char gyro_orientation[9] = { 1, 0, 0,
                                           0, 1,  0,
                                           0, 0,  1};


struct int_param_s int_param;

struct rx_s {
    unsigned char header[3];
    unsigned char cmd;
};
struct hal_s {
    unsigned char sensors;
    unsigned char dmp_on;
    unsigned char wait_for_tap;
    volatile unsigned char new_gyro;
    unsigned short report;
    unsigned short dmp_features;
    unsigned char motion_int_mode;
    struct rx_s rx;
};
static struct hal_s hal = {0};

///-----------------------------------------------------------------------------
///         DMP related structs --  End
///-----------------------------------------------------------------------------

///-----------------------------------------------------------------------------
///         DMP related functions --  Start
///-----------------------------------------------------------------------------

/* These next two functions converts the orientation matrix (see
 * gyro_orientation) to a scalar representation for use by the DMP.
 * NOTE: These functions are borrowed from Invensense's MPL.
 */
static inline unsigned short inv_row_2_scale(const signed char *row)
{
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;      // error
    return b;
}

static inline unsigned short inv_orientation_matrix_to_scalar(
    const signed char *mtx)
{
    unsigned short scalar;

    /*
       XYZ  010_001_000 Identity Matrix
       XZY  001_010_000
       YXZ  010_000_001
       YZX  000_010_001
       ZXY  001_000_010
       ZYX  000_001_010
     */

    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;


    return scalar;
}

/**
 * Return pointer to a singleton
 * @return pointer to a internal static instance
 */
MPU9250* GetP()
{
    static MPU9250 singletonInstance;
    return &(singletonInstance);
}

///-----------------------------------------------------------------------------
///         Public functions used for configuring MPU9250               [PUBLIC]
///-----------------------------------------------------------------------------

/**
 * Initialize hardware used by MPU9250
 * Initializes I2C bus for communication with MPU (SDA - PN4, SCL - PN5), bus
 * frequency 1MHz, connection timeout: 100ms. Initializes pin(PA5)
 * to be toggled by MPU9250 when it has data available for reading (PA5 is
 * push-pull pin with weak pull down and 10mA strength).
 * @return One of MPU_* error codes
 */
int8_t InitHW()
{
    HAL_MPU_Init();
    HAL_MPU_PowerSwitch(true);

    return MPU_SUCCESS;
}

/**
 * Initialize MPU sensor, load DMP firmware and configure DMP output. Prior to
 * any software initialization, this function power-cycles the board
 * @return One of MPU_* error codes
 */
int8_t InitSW()
{
    int result;

    //  Power cycle MPU chip on every SW initialization
    HAL_MPU_PowerSwitch(false);
    HAL_DelayUS(20000);
    HAL_MPU_PowerSwitch(true);
    HAL_DelayUS(30000);

    mpu_init(&int_param);

    //  Get/set hardware configuration. Start gyro.
    // Wake up all sensors.
    mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS);
    // Push accel and quaternion data into the FIFO.
    mpu_configure_fifo(INV_XYZ_ACCEL);
    mpu_set_sample_rate(50);

    // Initialize HAL state variables.
    memset(&hal, 0, sizeof(hal));


#ifdef __DEBUG_SESSION__
    UART0_SendString("Trying to load firmware\n");
#endif
    result = 7; //  Try loading firmware max 7 times
    while(result--)
        if (dmp_load_motion_driver_firmware() == 0)
            break;
#ifdef __DEBUG_SESSION__
        else
        {
            UART0_SendString("result when  Try loading firmware\n");
            UART_sendNumber((uint32_t)result);
        }
#endif

    if (result <= 0)    //  If loading failed 7 times hang here, DMP not usable
    {
#ifdef __DEBUG_SESSION__

        UART0_SendString("failed  \n");
#endif

        while(1);
    }

#ifdef __DEBUG_SESSION__


    UART0_SendString(" >Firmware loaded\n ");

    UART0_SendString(" >Updating DMP features...\n");

#endif

    dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation));

    hal.dmp_features = DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
        DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
        DMP_FEATURE_GYRO_CAL;
    dmp_enable_feature(hal.dmp_features);

    dmp_set_fifo_rate(20);//50
    mpu_set_dmp_state(1);
    hal.dmp_on = 1;
#ifdef __DEBUG_SESSION__
    //DEBUG_WRITE("done\n");

    UART0_SendString("done.\n");
#endif

    return MPU_SUCCESS;
}

/**
 * Trigger software reset of the MPU module by writing into corresponding
 * register. Wait for 50ms afterwards for sensor to start up.
 * @return One of MPU_* error codes
 */
int8_t Reset()
{
    HAL_MPU_WriteByte(MPU9250_ADDRESS, PWR_MGMT_1, 1 << 7);
    HAL_DelayUS(50000);

    return MPU_SUCCESS;
}

/**
 * Control power supply of the MPU9250
 * Enable or disable power supply of the MPU9250 using external MOSFET
 * @param en Power state
 * @return One of MPU_* error codes
 */
int8_t Enabled(bool en)
{
    HAL_MPU_PowerSwitch(en);

    return MPU_SUCCESS;
}

/**
 * Check if new sensor data has been received
 * @return true if new sensor data is available
 *        false otherwise
 */
bool IsDataReady()
{
    return HAL_MPU_DataAvail();
}

/**
 * Get ID from MPU, should always return 0x71
 * @return ID value stored in MPU's register
 */
uint8_t GetID()
{
    uint8_t ID;
    ID = HAL_MPU_ReadByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);

    return ID;
}

/**
 * Trigger reading data from MPU9250
 * Read data from MPU9250s' FIFO and extract quaternions, acceleration, gravity
 * vector & roll-pitch-yaw
 * @return One of MPU_* error codes
 */
int8_t ReadSensorData(MPU9250*M)
{
    int8_t retVal = MPU_ERROR;
    short gyro[3], accel[3], sensors;
    unsigned char more = 1;
    long quat[4];
    unsigned long sensor_timestamp;
    int cnt = 0;


     //  Make sure the fifo is empty before leaving this loop, in
     //  order to prevent fifo overflow on consecutive sensor reading
     while (cnt < 100)   //Read max 100 packets, if there's more we
                         //   have a problem
     {
         /* This function gets new data from the FIFO when the DMP is in
           * use. The FIFO can contain any combination of gyro, accel,
           * quaternion, and gesture data. The sensors parameter tells the
           * caller which data fields were actually populated with new data.
           * For example, if sensors == (INV_XYZ_GYRO | INV_WXYZ_QUAT), then
           * the FIFO isn't being filled with accel data.
           * The driver parses the gesture data to determine if a gesture
           * event has occurred; on an event, the application will be notified
           * via a callback (assuming that a callback function was properly
           * registered). The more parameter is non-zero if there are
           * leftover packets in the FIFO.
           */
         retVal = dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors, &more);
         cnt++;
#ifdef __DEBUG_SESSION__
         if (retVal == (-2))
         {
                      DEBUG_WRITE("READ_FIFO returned: %d \n" );
                      UART_sendNumber((uint32_t)retVal);
         }
#endif  /* __DEBUG_SESSION__ */

         if (sensors == 0)   //No data available
         {
             retVal = MPU_SUCCESS;
             break;
         }

         //  If reading fifo returned error, move to next packet
         if (retVal)
             continue;

         //  If there was no error, extract orientation data
         Quaternion qt;
         qt.x = (float)quat[0]/QUAT_SENS;
         qt.y = (float)quat[1]/QUAT_SENS;
         qt.z = (float)quat[2]/QUAT_SENS;
         qt.w = (float)quat[3]/QUAT_SENS;

         VectorFloat v;
         dmp_GetGravity(&v, &qt);

         dmp_GetYawPitchRoll((float*)(M->_ypr), &qt, &v);

         M->_quat[0] = qt.x;
         M->_quat[1] = qt.y;
         M->_quat[2] = qt.z;
         M->_quat[3] = qt.w;

         //  Copy to MPU class
         M->_gv[0] = v.x;
         M->_gv[1] = v.y;
         M->_gv[2] = v.z;

         M->_acc[0] = (float)accel[0]/32767.0;
         M->_acc[1] = (float)accel[1]/32767.0;
         M->_acc[2] = (float)accel[2]/32767.0;
     }

     return retVal;
}

/**
 * Copy orientation from internal buffer to user-provided one
 * @param RPY pointer to float buffer of size 3 to hold roll-pitch-yaw
 * @param inDeg if true RPY returned in degrees, if false in radians
 * @return One of MPU_* error codes
 */
int8_t RPY(float* RPY, bool inDeg,MPU9250*M)
{
    uint8_t j;
    for ( j = 0; j < 3; j++)
    {
        if (inDeg)
            RPY[j] = M->_ypr[j]*180.0/PI_CONST;
        else
            RPY[j] = M->_ypr[j];
    }

    return MPU_SUCCESS;
}

/**
 * Copy acceleration from internal buffer to user-provided one
 * @param acc Pointer a float array of min. size 3 to store 3-axis acceleration
 *        data
 * @return One of MPU_* error codes
 */
int8_t Acceleration(float *acc,MPU9250 *M)
{
    memcpy((void*)acc, (void*)(M->_acc), sizeof(float)*3);

    return MPU_SUCCESS;
}

/**
 * Copy angular rotation from internal buffer to user-provided one
 * @param gyro Pointer a float array of min. size 3 to store 3-axis rotation
 *        data
 * @return One of MPU_* error codes
 */
int8_t Gyroscope(float *gyro)
{
    //  Not available
    gyro[0] = gyro[1] = gyro[2] = 0.0f;

    return MPU_SUCCESS;
}

/**
 * Copy mag. field strength from internal buffer to user-provided one
 * @param mag Pointer a float array of min. size 3 to store 3-axis mag. field
 *        strength data
 * @return One of MPU_* error codes
 */
int8_t Magnetometer(float *mag)
{
    //  Not available
    mag[0] = mag[1] = mag[2] = 0.0f;

    return MPU_SUCCESS;
}

///-----------------------------------------------------------------------------
///                      Class constructor & destructor              [PROTECTED]
///-----------------------------------------------------------------------------

 void MPU9250_value_init(MPU9250 *M)
{
     M->dT=0;
     M->_magEn=true;
    //  Initialize arrays
    memset((void*)M->_ypr, 0, 3);
    memset((void*)M->_acc, 0, 3);
    memset((void*)M->_gyro, 0, 3);
    memset((void*)M->_mag, 0, 3);
}



#endif  /* __HAL_USE_MPU9250_DMP__ */
 //-------------------------------------------MPU9250 FILE --------------------------------------------------------------//





int main(void)

{
    //MPU9250& mpu = MPU9250::GetI();

    MPU9250 * M=GetP();

    //initialize the MPU value
    MPU9250_value_init(M);

    //  Initialize board and FPU
    HAL_BOARD_CLOCK_Init();

    //  Initialize serial port
   // SerialPort::GetI().InitHW();
    UART0_Config();


    UART0_SendString("Initialized Uart... \n");

    //  Initialize hardware used by MPU9250
    InitHW();

    UART0_SendString("ALOOOOO.. \n");
    //  Software initialization of MPU9250
    //  Either configure registers for direct sensor readings or load DMP
    //  firmware

   InitSW();

    UART0_SendString("SALAM. \n");

#ifdef __HAL_USE_MPU9250_NODMP__
    //  Set AHRS time step to 1kHz and configure gains
    // Settings in mpu.InitSW(); are using 200MHz data sampling rate
    mpu.SetupAHRS(0.005, 0.5, 0.00);
#endif  /* __HAL_USE_MPU9250_NODMP__ */

    float rpy[3];
    uint32_t counter = 0;
    while (1)
    {



        //  Check if MPU toggled interrupt pin
        //  (this example doesn't use actual interrupts, but polling)
        if (HAL_MPU_DataAvail())
        {

            //  Read sensor data
            ReadSensorData(M);

            //  Get RPY values
            /* get yaw pitch and roll values in degrees */
            RPY(rpy, true,M);
        }

        // INT pin can be held up for max 50us, so delay here to prevent reading the same data twice
        HAL_DelayUS(100);

        if (counter++ > 1000)
        {
            // Every 1000 * 100us print out data to prevent spamming uart

            //  Print out sensor measurements
//            DEBUG_WRITE("{%02d.%03d, %02d.%03d, %02d.%03d, ", _FTOI_(__mpu._gyro[0]), _FTOI_(__mpu._gyro[1]), _FTOI_(__mpu._gyro[2]));
//            DEBUG_WRITE("%02d.%03d, %02d.%03d, %02d.%03d, ", _FTOI_(__mpu._acc[0]), _FTOI_(__mpu._acc[1]), _FTOI_(__mpu._acc[2]));
//            DEBUG_WRITE("%02d.%03d, %02d.%03d, %02d.%03d},\n", _FTOI_(__mpu._mag[0]), _FTOI_(__mpu._mag[1]), _FTOI_(__mpu._mag[2]));

            //  Print out orientation
            //  note: _FTOI_ is just a macro to print float numbers, it takes
            //  a float a splits it in 2 integers that are printed separately


            UART0_SendString("Yaw angle is = ");

           UART_sendNumber(_FTOI_I(rpy[0]));


            UART0_SendChr('.');

            UART_sendNumber( _FTOI_F(rpy[0]));


            UART0_SendString("\r\n\n");

       //     UART_sendNumber(g_ui32SysClock);


            UART0_SendString("Pitch angle is  = ");
           UART_sendNumber( _FTOI_I(rpy[1]));
            UART0_SendChr('.');
            UART_sendNumber( _FTOI_F(rpy[1]));
            UART0_SendString("\r\n\n");

            UART0_SendString("Roll angle is    = ");
            UART_sendNumber( _FTOI_I(rpy[2]));
            UART0_SendChr('.');
            UART_sendNumber( _FTOI_F(rpy[2]));
            UART0_SendString("\r\n\n");


            counter = 0;
        }
    }
}
