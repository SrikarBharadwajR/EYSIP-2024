#include "VCNL4040.h"

static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(1); // TWI (I2C) instance
static volatile bool m_xfer_done = false;                   // Flag to indicate that a TWI transfer has completed

/**
 * @brief TWI handler for the VCNL4040 sensor.
 *
 * This function handles TWI (I2C) events for the VCNL4040 sensor.
 *
 * @param p_event Pointer to the TWI event structure.
 * @param p_context Pointer to the context.
 */
void VCN4040_twi_handler(nrf_drv_twi_evt_t const *p_event, void *p_context) {

  switch (p_event->type) { // Check the event to see what type of event occurred

  case NRF_DRV_TWI_EVT_DONE: // If data transmission or receiving is finished
    m_xfer_done = true;      // Set the flag
    break;

  default:
    break;
  }
}

/**
 * @brief Initializes the TWI (I2C) interface for the VCNL4040 sensor.
 *
 * This function sets up the TWI interface for communication with the VCNL4040 sensor.
 */
void VCN4040_TWI_initialize(void) {
  ret_code_t err_code;

  // Configuration settings for the TWI interface
  const nrf_drv_twi_config_t twi_config = {
      .scl = 4,
      .sda = 5,
      .frequency = NRF_DRV_TWI_FREQ_100K,
      .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
      .clear_bus_init = false};

  err_code = nrf_drv_twi_init(&m_twi, &twi_config, VCN4040_twi_handler, NULL); // Initialize the TWI interface
  APP_ERROR_CHECK(err_code);                                                   // Check for the error code for debug
  VCN4040_enable(true);                                                        // Enable the TWI interface
}

/**
 * @brief Enables or disables the VCNL4040 sensor.
 *
 * This function enables or disables the VCNL4040 sensor based on the isEnabled parameter.
 *
 * @param isEnabled Boolean value indicating whether to enable (true) or disable (false) the sensor.
 */
void VCN4040_enable(bool isEnabled) {
  if (isEnabled)
    nrf_drv_twi_enable(&m_twi);
  else
    nrf_drv_twi_disable(&m_twi);
}

/**
 * @brief Initializes the VCNL4040 sensor.
 *
 * This function initializes the VCNL4040 sensor by setting the PS_CONF1 register.
 */
void vcnl4040_init(void) {
  VCN4040_writeByte(VCNL4040_ADDRESS, VCNL4040_PS_CONF1_REG, 0x0080); // Set PS_CONF1 register
}

/**
 * @brief Reads the proximity value from the VCNL4040 sensor.
 *
 * This function reads the proximity data from the VCNL4040 sensor and returns it.
 *
 * @return The proximity value or -1 if read failed.
 */
uint16_t read_proximity(void) {
  uint8_t data[2];
  uint16_t proximity;
  if (VCN4040_readBytes(VCNL4040_ADDRESS, VCNL4040_PS_DATA_REG, 2, data, 1000)) {
    proximity = (data[1] << 8) | data[0];
    return proximity;
  }
  return -1;
}

/**
 * @brief Writes a byte to a register of the VCNL4040 sensor.
 *
 * This function writes a byte of data to the specified register of the VCNL4040 sensor.
 *
 * @param devAddr The I2C address of the device.
 * @param regAddr The register address to write to.
 * @param data The data to write.
 * @return true if the write operation was successful, otherwise false.
 */
bool VCN4040_writeByte(uint8_t devAddr, uint8_t regAddr, uint16_t data) {
  ret_code_t err_code;
  uint8_t w2_data[3];
  w2_data[0] = regAddr;
  w2_data[1] = data;
  w2_data[2] = data << 8;

  m_xfer_done = false;
  err_code = nrf_drv_twi_tx(&m_twi, devAddr, w2_data, sizeof(w2_data), false);
  APP_ERROR_CHECK(err_code);
  while (!m_xfer_done) {
  }
  return err_code == NRF_SUCCESS;
}
/**
 * @brief Reads bytes from a register of the VCNL4040 sensor.
 *
 * This function reads multiple bytes of data from the specified register of the VCNL4040 sensor.
 *
 * @param devAddr The I2C address of the device.
 * @param regAddr The register address to read from.
 * @param length The number of bytes to read.
 * @param data Pointer to the buffer to store the read data.
 * @param timeout The timeout period for the read operation.
 * @return The number of bytes read, or a negative value if an error occurred.
 */
bool VCN4040_readBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data, uint16_t timeout) {
  ret_code_t err_code;
  m_xfer_done = false;                // Set the flag to false to show the receiving is not yet completed
  uint8_t register_address = regAddr; // Send the Register address where we want to write the data

  err_code = nrf_drv_twi_tx(&m_twi, devAddr, &register_address, 1, true);

  // TODO: Implement Timeout
  // Wait for the transmission to get completed
  while (m_xfer_done == false) {
  }
  // If transmission was not successful, exit the function with false as return value
  if (NRF_SUCCESS != err_code) {
    return false;
  }

  m_xfer_done = false;                                      // Set the flag again so that we can read data from the VCNL4040's internal register
  err_code = nrf_drv_twi_rx(&m_twi, devAddr, data, length); // Receive the data from the VCNL4040

  while (m_xfer_done == false) { // Wait until the transmission is completed
  }
  if (NRF_SUCCESS != err_code) { // If data was successfully read, return true else return false

    return false;
  }
  return true;
}