/*
 * VL53L3CX.c
 *
 *  Created on: Nov 3, 2020
 *      Author: Aniket
 */


/* INCLUDES */

#include "vl53lx_api.h"
/* MACROS */

#define SLAVEADDRESS 0x52

/* Data Definition */
uint32_t  MeasurementTimingBudgetMicroSeconds = 3000000;
uint8_t DataReady = 0;

VL53LX_DEV  dev = {0};
VL53LX_MultiRangingData_t Data = {0};


VL53L3CX_init()
{
    //Initializing device handle

    dev.i2c_slave_address = SLAVEADDRESS;
    dev.new_data_ready_poll_duration_ms = MeasurementTimingBudgetMicroSeconds;
    dev.comms_type = 1;
    dev.comms_speed_khz = 400;
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

VL53L3CX_Measure()
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
        printf("\n Stream Count : %d", Data.StreanCount);
        printf("\nNumber of Objects: %d", Data.NumberOfObjectsFound);
        for(int i = 0; i < Data.NumberOfObjectsFound ; i++)
        {
            printf("\n Distance %d: %d", i+1, Data.RangeData[i]);
        }

    }
    else
    {
        printf("issue in starting measurement");
    }
}
