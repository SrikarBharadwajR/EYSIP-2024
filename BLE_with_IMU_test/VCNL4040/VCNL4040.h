#ifndef _VCNL4040_H_
#define _VCNL4040_H_

#include "app_error.h"
#include "boards.h"
#include "nrf_delay.h"
#include "nrf_drv_twi.h"

/**
 * @file vcnl4040.h
 * @brief Header file for VCNL4040 sensor module.
 *
 * This file contains the function prototypes, macros, and type definitions
 * for interfacing with the VCNL4040 proximity sensor.
 */

/**
 * @brief I2C address for the VCNL4040 sensor.
 */
#define VCNL4040_ADDRESS 0x60

/**
 * @brief Register addresses for the VCNL4040 sensor.
 */
#define VCNL4040_PS_CONF1_REG 0x03 /**< Configuration register 1 for proximity sensor */
#define VCNL4040_PS_CONF2_REG 0x04 /**< Configuration register 2 for proximity sensor */
#define VCNL4040_PS_CONF3_REG 0x05 /**< Configuration register 3 for proximity sensor */
#define VCNL4040_PS_MS_REG 0x06    /**< Measurement signal register for proximity sensor */
#define VCNL4040_PS_DATA_REG 0x08  /**< Data register for proximity sensor */

/**
 * @brief Initializes the VCNL4040 sensor.
 *
 * This function initializes the VCNL4040 sensor
 */
void vcnl4040_init(void);

/**
 * @brief Reads the proximity value from the VCNL4040 sensor.
 *
 * This function reads the proximity data from the VCNL4040 sensor and returns proximity data.
 *
 * @return The proximity value.
 */
uint16_t read_proximity(void);

/**
 * @brief TWI handler for the VCNL4040 sensor.
 *
 * This function handles TWI (I2C) events for the VCNL4040 sensor.
 *
 * @param p_event Pointer to the TWI event structure.
 * @param p_context Pointer to the context.
 */
void VCN4040_twi_handler(nrf_drv_twi_evt_t const *p_event, void *p_context);

/**
 * @brief Initializes the TWI (I2C) interface for the VCNL4040 sensor.
 *
 * This function sets up the TWI interface for communication with the VCNL4040 sensor.
 */
void VCN4040_TWI_initialize(void);

/**
 * @brief Enables or disables the VCNL4040 sensor.
 *
 * This function enables or disables the VCNL4040 sensor based on the isEnabled parameter.
 *
 * @param isEnabled Boolean value indicating whether to enable (true) or disable (false) the sensor.
 */
void VCN4040_enable(bool isEnabled);

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
bool VCN4040_writeByte(uint8_t devAddr, uint8_t regAddr, uint16_t data);

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
 * @return true if the read operation was successful, otherwise false.
 */
bool VCN4040_readBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data, uint16_t timeout);

#endif // _VCNL4040_H_
