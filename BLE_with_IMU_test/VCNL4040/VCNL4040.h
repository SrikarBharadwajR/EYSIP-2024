#ifndef _VCNL4040_H_
#define _VCNL4040_H_

#include "app_error.h"
#include "boards.h"
#include "nrf_delay.h"
#include "I2Cdev.h"

#define VCNL4040_ADDRESS 0x60

#define VCNL4040_PS_CONF1_REG 0x03
#define VCNL4040_PS_CONF2_REG 0x04
#define VCNL4040_PS_CONF3_REG 0x05
#define VCNL4040_PS_MS_REG 0x06
#define VCNL4040_PS_DATA_REG 0x08


void vcnl4040_init(void);
uint16_t read_proximity(void);

#endif