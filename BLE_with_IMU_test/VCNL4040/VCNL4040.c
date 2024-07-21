#include "VCNL4040.h"

void vcnl4040_init(void) {
  VCN4040_writeByte(VCNL4040_ADDRESS, VCNL4040_PS_CONF1_REG, 0x0080); // Set PS_CONF1 register
}

uint16_t read_proximity(void) {
  uint8_t data[2];
  uint16_t proximity;
  if (readBytes(VCNL4040_ADDRESS, VCNL4040_PS_DATA_REG, 2, data, 1000)) {
    proximity = (data[1] << 8) | data[0];
    return proximity;
  }
  return -1;
}