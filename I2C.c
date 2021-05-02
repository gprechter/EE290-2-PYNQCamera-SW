#include "I2C.h"

void I2C_init() {
	reg_write8(ctrl_addr, 0x00);
        reg_write8(pre_lo_addr, 0x13);
        reg_write8(pre_hi_addr, 0x00);
        reg_write8(ctrl_addr, 0x80);
}

void I2C_beginTransmission(uint8_t addr) {
	while(reg_read8(cmd_stat_addr) & 0x02);
        reg_write8(data_addr, (addr << 1));
        reg_write8(cmd_stat_addr, 0x90);
        while(reg_read8(cmd_stat_addr) & 0x02);
}

void I2C_write(uint8_t data, bool lastByte) {
	while(reg_read8(cmd_stat_addr) & 0x02);
        reg_write8(data_addr, data);
        reg_write8(cmd_stat_addr, 0x10 | (lastByte << 6));
        while(reg_read8(cmd_stat_addr) & 0x02);
}

uint8_t I2C_read(uint8_t addr) {
	while(reg_read8(cmd_stat_addr) & 0x02);
        reg_write8(data_addr, (addr << 1) | 0x1);
        reg_write8(cmd_stat_addr, 0x90);
        while(reg_read8(cmd_stat_addr) & 0x02);
        reg_write8(cmd_stat_addr, 0x68);
        while(reg_read8(cmd_stat_addr) & 0x02);
	return reg_read8(data_addr);
}
