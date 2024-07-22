#include "ICM20948.h"

/**
 * @brief Tests the connection to the ICM-20948 sensor.
 *
 * This function reads the WHO_AM_I register to verify the connection to the sensor.
 *
 * @return True if the sensor responds with the expected WHO_AM_I value, false otherwise.
 */
bool testConnection(void) {
  uint8_t who_am_i = 0;                                                // Variable to store the WHO_AM_I register value
  if (readBytes(ICM20948_ADDRESS, WHO_AM_I_REG, 1, &who_am_i, 1000)) { // Read the WHO_AM_I register
    return who_am_i == WHO_AM_I_EXPECTED;                              // Check if the value matches the expected WHO_AM_I value
  } else {                                                             //
    return false;                                                      // Return false if the read operation failed
  }
}

/**
 * @brief Initializes the ICM-20948 IMU.
 *
 * This function configures the ICM-20948 IMU by writing to specific configuration registers.
 *
 * @return True if initialization was successful, false otherwise.
 */
bool initializeIMU(void) {
  if (!writeByte(ICM20948_ADDRESS, 0x06, 0x01)) { // Write to register 0x06
    return false;                                 // Return false if the write operation failed
  }
  if (!writeByte(ICM20948_ADDRESS, 0x14, 0x10)) { // Write to register 0x14
    return false;                                 // Return false if the write operation failed
  }
  if (!writeByte(ICM20948_ADDRESS, 0x15, 0x10)) { // Write to register 0x15
    return false;                                 // Return false if the write operation failed
  }                                               //
  return true;                                    // Return true if all write operations were successful
}

/**
 * @brief Reads accelerometer and gyroscope data from the ICM-20948.
 *
 * This function reads raw accelerometer and gyroscope data from the sensor and stores it in the provided arrays.
 *
 * @param accelData Pointer to an array where accelerometer data will be stored.
 * @param gyroData Pointer to an array where gyroscope data will be stored.
 */
void readAccelGyroData(int16_t *accelData, int16_t *gyroData) {
  uint8_t rawData[12];                                          // Array to store raw data read from the sensor (6 bytes for accelerometer, 6 bytes for gyroscope)
  readBytes(ICM20948_ADDRESS, ACCEL_XOUT_H, 12, rawData, 1000); // Read 12 bytes of data starting from ACCEL_XOUT_H register

  // Extract accelerometer data from rawData array
  accelData[0] = (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]); // X-axis accelerometer data
  accelData[1] = (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]); // Y-axis accelerometer data
  accelData[2] = (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]); // Z-axis accelerometer data

  // Extract gyroscope data from rawData array
  gyroData[0] = (int16_t)(((int16_t)rawData[6] << 8) | rawData[7]);   // X-axis gyroscope data
  gyroData[1] = (int16_t)(((int16_t)rawData[8] << 8) | rawData[9]);   // Y-axis gyroscope data
  gyroData[2] = (int16_t)(((int16_t)rawData[10] << 8) | rawData[11]); // Z-axis gyroscope data
}
