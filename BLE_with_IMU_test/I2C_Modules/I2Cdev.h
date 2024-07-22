
#ifndef _I2CDEV_H_
#define _I2CDEV_H_
#include "compiler_abstraction.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_drv_twi.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include <stdio.h>

#ifdef SOFTDEVICE_PRESENT
#include "app_error.h"
#include "nrf_soc.h"
#endif

#define TWI_INSTANCE_ID 0

#define I2C_SDA_PORT gpioPortA
#define I2C_SDA_PIN 0
#define I2C_SDA_MODE gpioModeWiredAnd
#define I2C_SDA_DOUT 1

#define I2C_SCL_PORT gpioPortA
#define I2C_SCL_PIN 1
#define I2C_SCL_MODE gpioModeWiredAnd
#define I2C_SCL_DOUT 1

#define I2CDEV_DEFAULT_READ_TIMEOUT 1000

void TWI_initialize();
void enable(bool isEnabled);

int8_t readBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data, uint16_t timeout);
int8_t readBitW(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t *data, uint16_t timeout);
int8_t readBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data, uint16_t timeout);
int8_t readBitsW(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t *data, uint16_t timeout);
int8_t readByte(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint16_t timeout);
int8_t readWord(uint8_t devAddr, uint8_t regAddr, uint16_t *data, uint16_t timeout);
int8_t readBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data, uint16_t timeout);
int8_t readWords(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data, uint16_t timeout);

bool writeBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data);
bool writeBitW(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t data);
bool writeBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data);
bool writeBitsW(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t data);
bool writeByte(uint8_t devAddr, uint8_t regAddr, uint8_t data);
bool writeWord(uint8_t devAddr, uint8_t regAddr, uint16_t data);
bool writeBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data);
bool writeWords(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data);
void twi_handler(nrf_drv_twi_evt_t const *p_event, void *p_context);
extern uint16_t readTimeout;

#endif /* _I2CDEV_H_ */
