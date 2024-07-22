/*
 * @file:     main.c
 * @brief:    Interfacing IMU, BLE UART, Proximity Sensor, RGB LED
 * @author:   Srikar Bharadwaj R
 */

/* Includes ------------------------------------------------------------------*/

#include "Ble_UART.h"
#include "I2Cdev.h"
#include "ICM20948.h"
#include "VCNL4040.h"
#include "WS2812B.h"
#include "math.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

/* Private variables ---------------------------------------------------------*/

int16_t accelData[3], gyroData[3];          // Array to store accelerometer and gyroscope data in X, Y, Z axes
uint8_t data_array[100];                    // Buffer to hold a collection of data
uint8_t ble_rcv_data[BLE_NUS_MAX_DATA_LEN]; // Buffer to hold received BLE data, maximum length defined by BLE_NUS_MAX_DATA_LEN
uint8_t rgb[] = {0, 0, 0};                  // Array to store RGB values, initialized to {0, 0, 0}
uint8_t ble_index;                          // Index variable for BLE recieved character

/* Private function prototypes -----------------------------------------------*/
void timer1_init(void);
uint32_t micros(void);
void led_strip(void);
void printAccelGyroData(void);

/* Private user functions ---------------------------------------------------------*/
/**
 * @brief Initializes Timer 1 with specific configurations.
 *
 * This function configures Timer 1 (NRF_TIMER1) with the following settings:
 * - Stops the timer before starting configuration.
 * - Sets the timer mode to Timer.
 * - Configures the timer to use a 32-bit width.
 * - Sets the prescaler to 4, resulting in a timer frequency of 1 MHz (1 microsecond tick).
 * - Clears the timer to reset its value.
 * - Starts the timer after configuration.
 *
 * @param None
 * @return None
 */
void timer1_init(void) {
  NRF_TIMER1->TASKS_STOP = 1;                                                     // Stop the timer before configuration
  NRF_TIMER1->MODE = TIMER_MODE_MODE_Timer;                                       // Set the timer mode to Timer
  NRF_TIMER1->BITMODE = TIMER_BITMODE_BITMODE_32Bit << TIMER_BITMODE_BITMODE_Pos; // Set the timer bit width to 32 bits
  NRF_TIMER1->PRESCALER = 4;                                                      // 16 MHz / (2^4) = 1 MHz    // Set the timer frequency to 1 MHz (1 microsecond tick)
  NRF_TIMER1->TASKS_CLEAR = 1;                                                    // Clear the timer
  NRF_TIMER1->TASKS_START = 1;                                                    // Start the timer
}

/**
 * @brief Returns the current microsecond count from Timer 1.
 *
 * This function captures the current value of Timer 1 (NRF_TIMER1) and returns it.
 * The value represents the elapsed time in microseconds since the timer was started.
 *
 * @param None
 * @return uint32_t The current microsecond count.
 */

uint32_t micros(void) {
  NRF_TIMER1->TASKS_CAPTURE[0] = 1; // Capture the current timer value
  return NRF_TIMER1->CC[0];         // Read and return the captured value
}

/**
 * @brief Updates the LED strip by setting one LED at a time.
 *
 * This function updates the LED strip by incrementing the current LED position.
 * When the end of the strip is reached, it wraps around to the beginning.
 * It uses a static variable to keep track of the current LED being set.
 * The `set_one_LED` function is called to set the color of the current LED using the `rgb` array.
 *
 * @param None
 * @return None
 */
void led_strip(void) {
  static int current_LED = 0;                       // Static variable to keep track of the current LED
  current_LED = (current_LED < MAX_LEDs_in_STRIP) ? // Increment the current LED position
                    ++current_LED
                                                  : 0; // Wrap around to the beginning if end is reached
  set_one_LED(rgb, current_LED);                       // Set the color of the current LED
}

/**
 * @brief Reads and prints accelerometer and gyroscope data.
 *
 * This function performs the following steps:
 * - Reads accelerometer and gyroscope data into `accelData` and `gyroData` arrays.
 * - Formats the read data into a string and stores it in the `data_array`.
 * - Logs the accelerometer and gyroscope data using `NRF_LOG_INFO`.
 * - Flushes the log buffer to ensure all log messages are output.
 *
 * @param None
 * @return None
 */
void printAccelGyroData(void) {
  readAccelGyroData(accelData, gyroData);                                               // Read accelerometer and gyroscope data
  sprintf((char *)data_array, "Accel: X=%d, Y=%d, Z=%d\nGyro: X=%d, Y=%d, Z=%d\n\n",    //
      accelData[0], accelData[1], accelData[2], gyroData[0], gyroData[1], gyroData[2]); // Prepare the data to be transfered via Bluetooth UART
  NRF_LOG_INFO("Accel: X=%d, Y=%d, Z=%d", accelData[0], accelData[1], accelData[2]);    // Log accelerometer data
  NRF_LOG_INFO("Gyro: X=%d, Y=%d, Z=%d", gyroData[0], gyroData[1], gyroData[2]);        // Log gyroscope data
  NRF_LOG_FLUSH();                                                                      // Flush the log buffer
}

/* Main code ---------------------------------------------------------*/
int main(void) {
  ble_uart_init();              // Initialise BLE UART
  timer1_init();                // Initialise timer 1
  TWI_initialize();             // Initialise two wire interface (I2C)
  VCN4040_TWI_initialize();     // Initialise VCN4040's I2C (used since in PCB, it is connected to different bus)
  vcnl4040_init();              // Initialise VCNL4040 Proximity Sensor
  WS2812B_Init();               // Initialise RGB LED
  NRF_GPIO->DIRSET = (1 << 31); // Configure the PCB LED pin as output
  NRF_GPIO->OUTCLR = (1 << 31); // Set the PCB LED pin low initially

  NRF_LOG_INFO("Debug logging for UART over RTT started."); // Log a message indicating that debug logging for UART over RTT has started

  bool imu_connected = false; // Flag to track the connection status of the IMU

  if (testConnection()) {              // Test the connection to the IMU
    initializeIMU();                   // Initialize the IMU if the connection test is successful
    imu_connected = true;              // Set the flag to true indicating that the IMU is connected
    printAccelGyroData();              // Print the accelerometer and gyroscope data to make sure right data is being printed
  } else                               //
    strcat(data_array, "Hell World!"); // Append error message to data_array if the connection test fails

  nrf_delay_ms(300); // Delay for initialisation

  // Variables to manage timing and refresh rate
  uint32_t prev_time = 0;         // Variable to store the previous time value
  uint32_t current_time;          // Variable to store the current time value
  uint16_t refresh_rate_ms = 100; // Refresh rate in milliseconds (100 ms)

  /* Main loop code ---------------------------------------------------------*/
  while (1) {
    NRF_GPIO->OUTSET = (1 << 31);                             // Set GPIO pin PCB Red LED high inidicating code is running
    current_time = micros();                                  // Get the current time in microseconds
    if (current_time - prev_time >= 1000 * refresh_rate_ms) { // Check if the refresh period has elapsed
      led_strip();                                            // Update the LED strip
      prev_time = current_time;                               // Update the previous time
    }                                                         //
    if (prev_time > (uint32_t)pow(2, 31))                     // Prevent overflow of the previous time variable
      prev_time = 0;

    if (ble_rcv_data[ble_index - 1] == '\n') { // Check if the last received BLE character is a newline
      rgb[0] = 255;                            // Set RGB values to white
      rgb[1] = 255;                            //
      rgb[2] = 255;                            //
      ble_index = 0;                           // Reset the BLE index
    }

    if (imu_connected) {                    // If the IMU is connected, read and process data
      printAccelGyroData();                 // Print accelerometer and gyroscope data
      char prox[20];                        // Buffer for proximity data
      uint8_t proximity = read_proximity(); // Read proximity value

      if (proximity > 100) { // Set RGB values based on proximity value
        rgb[0] = 0;          //
        rgb[1] = 0;          //
        rgb[2] = 255;        // Set RGB LED to blue if proximity is greater than 100
      } else {               //
        rgb[0] = 0;          //
        rgb[1] = 255;        //
        rgb[2] = 0;          // Set RGB LED to green if proximity is 100 or less
      }
      sprintf(prox, "Prox: %li\n", proximity); // Format and append proximity data to data_array
      strcat(data_array, prox);                //
      transmitIMUdata();                       // Transmit the IMU data via BLE UART
    }
  }
}
