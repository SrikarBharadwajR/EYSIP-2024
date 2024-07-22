#define SCL_PIN 15
#define SDA_PIN 14

#include "I2Cdev.h"

// TWI instance
static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);
static volatile bool m_xfer_done = false;

uint16_t readTimeout = I2CDEV_DEFAULT_READ_TIMEOUT;

/** Initialize I2C0
 */

void twi_handler(nrf_drv_twi_evt_t const *p_event, void *p_context) {
  // Check the event to see what type of event occurred
  switch (p_event->type) {
  // If data transmission or receiving is finished
  case NRF_DRV_TWI_EVT_DONE:
    m_xfer_done = true; // Set the flag
    break;

  default:
    break;
  }
}

void TWI_initialize(void) {
  ret_code_t err_code;

  const nrf_drv_twi_config_t twi_config = {
      .scl = SCL_PIN,
      .sda = SDA_PIN,
      .frequency = NRF_DRV_TWI_FREQ_100K,
      .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
      .clear_bus_init = false};

  err_code = nrf_drv_twi_init(&m_twi, &twi_config, twi_handler, NULL);
  APP_ERROR_CHECK(err_code);
  enable(true);
}

/** Enable or disable I2C
 * @param isEnabled true = enable, false = disable
 */
void enable(bool isEnabled) {
  if (isEnabled)
    nrf_drv_twi_enable(&m_twi);
  else
    nrf_drv_twi_disable(&m_twi);
}

/** Read a single bit from an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param bitNum Bit position to read (0-7)
 * @param data Container for single bit value
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in I2Cdev::readTimeout)
 * @return Status of read operation (true = success)
 */
int8_t readBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data, uint16_t timeout) {
  uint8_t b;
  uint8_t count = readByte(devAddr, regAddr, &b, timeout);
  *data = b & (1 << bitNum);
  return count;
}

/** Read multiple bits from an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param bitStart First bit position to read (0-7)
 * @param length Number of bits to read (not more than 8)
 * @param data Container for right-aligned value (i.e. '101' read from any bitStart position will equal 0x05)
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in I2Cdev::readTimeout)
 * @return Status of read operation (true = success)
 */
// 01101001 read byte
// 76543210 bit numbers
//    xxx   args: bitStart=4, length=3
//    010   masked
//   -> 010 shifted
int8_t readBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data, uint16_t timeout) {
  uint8_t count, b;
  if ((count = readByte(devAddr, regAddr, &b, timeout)) != 0) {
    uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
    b &= mask;
    b >>= (bitStart - length + 1);
    *data = b;
  }
  return count;
}

/** Read single byte from an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param data Container for byte value read from device
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in I2Cdev::readTimeout)
 * @return Status of read operation (true = success)
 */
int8_t readByte(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint16_t timeout) {
  return readBytes(devAddr, regAddr, 1, data, timeout);
}

/** Read multiple bytes from an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr First register regAddr to read from
 * @param length Number of bytes to read
 * @param data Buffer to store read data in
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in I2Cdev::readTimeout)
 * @return I2C_TransferReturn_TypeDef http://downloads.energymicro.com/documentation/doxygen/group__I2C.html
 */

// int8_t readBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data, uint16_t timeout) {
//   ret_code_t r;

//  // bool transfer_succeeded;
//  nrf_drv_twi_tx(&m_twi, devAddr, &register_address, 1, true);
//  //    transfer_succeeded  = twi_master_transfer(m_device_address, &register_address, 1, TWI_DONT_ISSUE_STOP);
//  r = nrf_drv_twi_rx(&m_twi, devAddr, data, length);
//  // transfer_succeeded &= twi_master_transfer(m_device_address|TWI_READ_BIT, destination, number_of_bytes, TWI_ISSUE_STOP);
//  return r == NRF_SUCCESS;
//}
int8_t readBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data, uint16_t timeout) {

  ret_code_t err_code;
  // Set the flag to false to show the receiving is not yet completed
  m_xfer_done = false;
  // Send the Register address where we want to write the data
  uint8_t register_address = regAddr;
  err_code = nrf_drv_twi_tx(&m_twi, devAddr, &register_address, 1, true);

  // TODO Implement Timeout
  // Wait for the transmission to get completed
  while (m_xfer_done == false) {
  }
  // If transmission was not successful, exit the function with false as return value
  if (NRF_SUCCESS != err_code) {
    return false;
  }

  // set the flag again so that we can read data from the MPU6050's internal register
  m_xfer_done = false;
  // Receive the data from the MPU6050
  err_code = nrf_drv_twi_rx(&m_twi, devAddr, data, length);
  // wait until the transmission is completed
  while (m_xfer_done == false) {
  }
  // if data was successfully read, return true else return false
  if (NRF_SUCCESS != err_code) {
    return false;
  }
  return true;
}

/** write a single bit in an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to write to
 * @param bitNum Bit position to write (0-7)
 * @param value New bit value to write
 * @return Status of operation (true = success)
 */
bool writeBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data) {
  uint8_t b;
  readByte(devAddr, regAddr, &b, readTimeout);
  b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
  return writeByte(devAddr, regAddr, b);
}

/** Write multiple bits in an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to write to
 * @param bitStart First bit position to write (0-7)
 * @param length Number of bits to write (not more than 8)
 * @param data Right-aligned value to write
 * @return Status of operation (true = success)
 */
//      010 value to write
// 76543210 bit numbers
//    xxx   args: bitStart=4, length=3
// 00011100 mask byte
// 10101111 original value (sample)
// 10100011 original & ~mask
// 10101011 masked | value

bool writeBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data) {
  uint8_t b;
  if (readByte(devAddr, regAddr, &b, readTimeout) != 0) {
    uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
    data <<= (bitStart - length + 1); // shift data into correct position
    data &= mask;                     // zero all non-important bits in data
    b &= ~(mask);                     // zero all important bits in existing byte
    b |= data;                        // combine data with existing byte
    return writeByte(devAddr, regAddr, b);
  } else {
    return false;
  }
}

/** Write single byte to an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register address to write to
 * @param data New byte value to write
 * @return Status of operation (true = success)
 */
// bool writeByte(uint8_t devAddr, uint8_t regAddr, uint8_t data) {
//   uint8_t w2_data[2];

//  w2_data[0] = regAddr;
//  w2_data[1] = data;
//  return NRF_SUCCESS == nrf_drv_twi_tx(&m_twi, devAddr, w2_data, 2, false);
//}

bool writeByte(uint8_t devAddr, uint8_t regAddr, uint8_t data) {
  ret_code_t err_code;
  uint8_t w2_data[2];
  // Write the register address and data into transmit buffer
  w2_data[0] = regAddr;
  w2_data[1] = data;
  // Set the flag to false to show the transmission is not yet completed
  m_xfer_done = false;
  // Transmit the data over TWI Bus
  err_code = nrf_drv_twi_tx(&m_twi, devAddr, w2_data, 2, false);
  // Wait until the transmission of the data is finished
  while (m_xfer_done == false) {
  }
  // if there is no error then return true else return false
  if (NRF_SUCCESS != err_code) {
    return false;
  }
  return true;
}

/** Write multiple bytes to an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr First register address to write to
 * @param length Number of bytes to write
 * @param data Buffer to copy new data from
 * @return Status of operation (true = success)
 */

bool writeBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data) {
  const uint8_t buf_len = length + 1; // Register address + number of bytes
  uint8_t tx_buf[buf_len];

  tx_buf[0] = regAddr;
  memcpy(tx_buf + 1, data, length);

  return NRF_SUCCESS == nrf_drv_twi_tx(&m_twi, devAddr, tx_buf, buf_len, true);
}

/** Write single word to a 16-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register address to write to
 * @param data New word value to write
 * @return Status of operation (true = success)
 */
bool writeWord(uint8_t devAddr, uint8_t regAddr, uint16_t data) {
  return writeWords(devAddr, regAddr, 1, &data);
}

/** Write multiple words to a 16-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr First register address to write to
 * @param length Number of words to write
 * @param data Buffer to copy new data from
 * @return Status of operation (true = success)
 */
bool writeWords(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data) {
  if (NRF_SUCCESS != nrf_drv_twi_tx(&m_twi, devAddr, &regAddr, 1, true))
    return false;

  for (int i = 0; i < length; i++) {
    uint8_t d[2];
    d[0] = data[i] >> 8;
    d[1] = data[i] & 0xff;
    bool pending = i < length - 1;
    if (NRF_SUCCESS != nrf_drv_twi_tx(&m_twi, devAddr, d, 2, pending))
      return false;
  }
  return true;
}
