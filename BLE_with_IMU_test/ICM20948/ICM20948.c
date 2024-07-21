#include "ICM20948.h"

bool testConnection(void) {
  uint8_t who_am_i = 0;
  if (readBytes(ICM20948_ADDRESS, WHO_AM_I_REG, 1, &who_am_i, 1000)) {
    return who_am_i == WHO_AM_I_EXPECTED;
  } else {
    return false;
  }
}
bool initializeIMU(void) {
  if (!writeByte(ICM20948_ADDRESS, 0x06, 0x01)) {
    return false;
  }
  if (!writeByte(ICM20948_ADDRESS, 0x14, 0x10)) {
    return false;
  }
  if (!writeByte(ICM20948_ADDRESS, 0x15, 0x10)) {
    return false;
  }
  return true;
}

void readAccelGyroData(int16_t *accelData, int16_t *gyroData) {
  uint8_t rawData[12]; // 6 bytes for accelerometer and 6 bytes for gyroscope
  readBytes(ICM20948_ADDRESS, ACCEL_XOUT_H, 12, rawData, 1000);
  // Accel data
  accelData[0] = (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]);
  accelData[1] = (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]);
  accelData[2] = (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]);
  // Gyro data
  gyroData[0] = (int16_t)(((int16_t)rawData[6] << 8) | rawData[7]);
  gyroData[1] = (int16_t)(((int16_t)rawData[8] << 8) | rawData[9]);
  gyroData[2] = (int16_t)(((int16_t)rawData[10] << 8) | rawData[11]);
}

