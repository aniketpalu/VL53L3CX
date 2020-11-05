/*
 * VL53L3CX.c
 *
 *  Created on: Nov 3, 2020
 *      Author: Aniket
 */


/* INCLUDES */

#include "vl53lx_api.h"
#include <ti/drivers/I2C.h>
/* MACROS */

#define SLAVEADDRESS 0x52
#define SENSOR_INDEX 0


/* Data Definition */
uint32_t  MeasurementTimingBudgetMicroSeconds = 3000000;
uint8_t DataReady = 0;

I2C_Params params;
I2C_Handle i2cHandle;

VL53LX_DEV  dev = {0};
VL53LX_MultiRangingData_t Data = {0};


void VL53L3CX_init()
{
    //Initializing I2C instance


    I2C_init();

    params.bitRate = I2C_400kHz;

    I2C_Params_init(&params);

    i2cHandle = I2C_open(SENSOR_INDEX, &params);


    //Initializing device handle

    dev->i2c_slave_address = SLAVEADDRESS;
    dev->comms_type = 1;
    dev->comms_speed_khz = 400;
    dev->I2cHandle = &i2cHandle;
    /* Waiting for device to start */
    if(VL53LX_WaitDeviceBooted(dev) == VL53LX_ERROR_NONE)
    {
        printf("Device Started");
    }
    else
    {
        printf("Device has some issue");
    }

    // Setting device address

    if(VL53LX_SetDeviceAddress(dev, SLAVEADDRESS) == VL53LX_ERROR_NONE)
    {
        printf("Device address set");
    }
    else
    {
        printf("Device has some issue in setting address");
    }

    // Setting offset correction mode

    if(VL53LX_SetOffsetCorrectionMode(dev, VL53LX_OFFSETCORRECTIONMODE_STANDARD) == VL53LX_ERROR_NONE)
    {
        printf("Offset correction mode set to Standard");
    }
    else
    {
        printf("Device has some issue in setting offset mode");
    }

    // One time initilization

    if(VL53LX_DataInit(dev) == VL53LX_ERROR_NONE)
    {
        printf("Device initialized");
    }
    else
    {
        printf("Device has issue in initialization");
    }

    // Setting Distance Mode

    if(VL53LX_SetDistanceMode(dev , VL53LX_DISTANCEMODE_MEDIUM) == VL53LX_ERROR_NONE)
    {
        printf("Distance mode set to medium");
    }
    else
    {
        printf("Device has issue in setting distance mode");
    }

    // Setting timing budget

    if(VL53LX_SetMeasurementTimingBudgetMicroSeconds(dev , MeasurementTimingBudgetMicroSeconds) == VL53LX_ERROR_NONE)
    {
        printf("Timing budget set as 3 second");
    }
    else
    {
        printf("Device has issue in setting Timing Budget");
    }
}

void VL53L3CX_Measure()
{
    if(VL53LX_StartMeasurement(dev) == VL53LX_ERROR_NONE)
    {
        printf("Measurement Started");
    }
    else
    {
        printf("issue in starting measurement");
    }

    if(VL53LX_GetMeasurementDataReady(dev, &DataReady) == VL53LX_ERROR_NONE && DataReady == 1)
    {
        printf("Measurement Data is Ready");
    }
    else
    {
        printf("Measurement Data is Not ready");
    }
    if(VL53LX_GetMultiRangingData(dev, &Data) == VL53LX_ERROR_NONE)
    {
        printf("\nTime: %d", Data.TimeStamp);
        printf("\n Stream Count : %d", Data.StreamCount);
        printf("\nNumber of Objects: %d", Data.NumberOfObjectsFound);
        for(int i = 0; i < Data.NumberOfObjectsFound ; i++)
        {
            printf("\n**********************Object %d************************", i+1);
            printf("\n Max Distance: %d",Data.RangeData[i].RangeMaxMilliMeter);
            printf("\n Min Distance: %d",Data.RangeData[i].RangeMinMilliMeter);
            printf("\n Distance in mm: %d",Data.RangeData[i].RangeMilliMeter);
            printf("\n*******************************************************");
        }

    }
    else
    {
        printf("issue in starting measurement");
    }
}
