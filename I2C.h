#include <stdint.h>
#include <stdbool.h>
#include "mmio.h"
#define I2C_BASE_ADDR 0x10015000
#define pre_lo_addr 0x10015000
#define pre_hi_addr 0x10015004
#define ctrl_addr 0x10015008
#define data_addr 0x1001500C
#define cmd_stat_addr 0x10015010

void I2C_init();

void I2C_beginTransmission(uint8_t addr);

void I2C_write(uint8_t data, bool lastByte);

uint8_t I2C_read(uint8_t reg);
