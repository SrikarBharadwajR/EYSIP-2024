#ifndef _ICM20948_H_
#define _ICM20948_H_

#include "I2Cdev.h"  // Include the I2Cdev library for I2C communication
#include "stdbool.h" // Include standard boolean type definitions
#include <stdint.h>  // Include standard integer type definitions
#include <string.h>  // Include string manipulation functions

// External declarations of accelerometer and gyroscope data arrays
extern int16_t accelData[3], gyroData[3];

// ICM-20948 I2C address
#define ICM20948_ADDRESS 0x69

// ICM-20948 register addresses and expected values
#define WHO_AM_I_REG 0x00      // WHO_AM_I register address
#define WHO_AM_I_EXPECTED 0xEA // Expected value of the WHO_AM_I register
#define ACCEL_XOUT_H 0x2D      // Accelerometer X-axis high byte register address
#define GYRO_XOUT_H 0x33       // Gyroscope X-axis high byte register address

// Function prototypes
bool testConnection(void);                                     // Function to test the connection to the ICM-20948
bool initializeIMU(void);                                      // Function to initialize the ICM-20948 IMU
void readAccelGyroData(int16_t *accelData, int16_t *gyroData); // Function to read accelerometer and gyroscope data

#endif
