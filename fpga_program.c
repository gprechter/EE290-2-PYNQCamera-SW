#define SWITCH0 0x2008
#define SWITCH1 0x2009
#define LED0 0x2000
#define LED1 0x2001
#define USER_PROG_START 0x8000C000
#define DATA 0x200C
#define VSYNC 0x200D
#define P_CLK 0x200F
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "mmio.h"
#include "uart.h"
/*
uint8_t OV7670_readRegister(uint8_t reg) {
	I2C_beginTransmission(0x21);
	I2C_write(reg, true);
	return I2C_read(0x21);
}

void OV7670_writeRegister(uint8_t reg, uint8_t value) {
	I2C_beginTransmission(0x21);
	I2C_write(reg, false);
	I2C_write(value, true);
}
*/
void waitForVsync() {
	while(!reg_read8(VSYNC));
}

void waitForPCLKRisingEdge() {
	while(reg_read8(P_CLK));
	while(!reg_read8(P_CLK));
}

void processFrame() {
	uint8_t lineBuffer [160 * 2 + 1 + 5];
	uint16_t lineBufferIndex = 0;
	kputc(0x00);	
	for (uint16_t y = 0; y < 120; y++) {
		lineBufferIndex = 0;
		uint8_t sendWhileReadCounter = 4;
		lineBuffer[0] = 0;
		for (uint16_t x = 1; x < 160 * 2 + 1; x++) {
			if (sendWhileReadCounter) {
				sendWhileReadCounter--;
			} else {
				if (lineBufferIndex & 0x01) {
					kputc(lineBuffer[lineBufferIndex] | 0b00100001);
				} else {
					kputc(lineBuffer[lineBufferIndex] | 0b00001000);					
				}
				lineBufferIndex++;
				sendWhileReadCounter = 4;
			}
		//	waitForPCLKRisingEdge();
			while (*((volatile uint8_t *)P_CLK) & 0x1);
			while (!(*((volatile uint8_t *)P_CLK) & 0x1));
			lineBuffer[x] = *((volatile uint8_t *)DATA);
		}
		while (lineBufferIndex < 160 * 2) {
			if (lineBufferIndex & 0x01) {
                kputc(lineBuffer[lineBufferIndex] | 0b00100001);
            } else {
                kputc(lineBuffer[lineBufferIndex] | 0b00001000);
            }	
			lineBufferIndex++;
		}
	}
}

int main(void) {			
	reg_write8(UART_TX_CONTROL, 0x01);
	reg_write8(UART_RX_CONTROL, 0x01);

	char c;
	//kputs("Connected to FPGA...", true);
	//I2C_init();
	while (true) {
		*((uint8_t *) LED0) = *((uint8_t *) SWITCH0);
		*((uint8_t *) LED1) = *((uint8_t *) SWITCH1);
        /*
		while(!kgetc(&c));
		waitForVsync();			
		processFrame();		
		*/
	}
	return 0;
}
