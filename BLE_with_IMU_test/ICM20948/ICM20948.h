#ifndef _ICM20948_H_
#define _ICM20948_H_

#include <stdint.h>
#include "stdbool.h"
#include <string.h>
#include "I2Cdev.h"


extern int16_t accelData[3], gyroData[3];

#define ICM20948_ADDRESS 0x69
#define WHO_AM_I_REG 0x00
#define WHO_AM_I_EXPECTED 0xEA
#define ACCEL_XOUT_H 0x2D
#define GYRO_XOUT_H 0x33



bool testConnection(void);
bool initializeIMU(void);
void readAccelGyroData(int16_t *accelData, int16_t *gyroData);



#endif