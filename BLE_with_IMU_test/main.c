/** @file
 *
 * @defgroup ble_sdk_uart_over_ble_main main.c
 * @{
 * @ingroup  ble_sdk_app_nus_eval
 * @brief    UART over BLE application main file.
 *
 * This file contains the source code for a sample application that uses the Nordic UART service.
 * This application uses the @ref srvlib_conn_params module.
 */

#include "Ble_UART.h"
#include "I2Cdev.h"
#include "ICM20948.h"
#include "VCNL4040.h"
#include "WS2812B.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

int16_t accelData[3], gyroData[3];
uint8_t data_array[100];

void led_strip() {
  WS2812B_Init();
  uint8_t rgb[] = {255, 180, 0};
  for (int i = 1; i <= MAX_LEDs_in_STRIP; i++) {
    set_one_LED(rgb, i);
    nrf_delay_ms(10);
  }
}

void printAccelGyroData() {
  readAccelGyroData(accelData, gyroData);
  sprintf((char *)data_array, "Accel: X=%d, Y=%d, Z=%d\nGyro: X=%d, Y=%d, Z=%d\n\n", accelData[0], accelData[1], accelData[2], gyroData[0], gyroData[1], gyroData[2]);
  NRF_LOG_INFO("Accel: X=%d, Y=%d, Z=%d", accelData[0], accelData[1], accelData[2]);
  NRF_LOG_INFO("Gyro: X=%d, Y=%d, Z=%d", gyroData[0], gyroData[1], gyroData[2]);
  NRF_LOG_FLUSH();
}

int main(void) {

  TWI_initialize();
  ble_uart_init();
  vcnl4040_init();

  NRF_LOG_INFO("Debug logging for UART over RTT started.");
  bool imu_connected = false;

  if (testConnection()) {
    initializeIMU();
    imu_connected = true;
    printAccelGyroData();
  } else
    strcat(data_array, "Hell World!");

  nrf_delay_ms(300);

  while (1) {
    led_strip();
    if (imu_connected) {
      printAccelGyroData();
      char prox[20];
      sprintf(prox, "Prox: %li\n", read_proximity());
      strcat(data_array, prox);
      transmitIMUdata();
    }
    //nrf_delay_ms(50);
  }
}
