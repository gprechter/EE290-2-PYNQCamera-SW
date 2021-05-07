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

uint8_t grayScale(uint8_t r, uint8_t g, uint8_t b) {
	return (r >> 2) + (g >> 1) + (b >> 3);
}

void processFrameOLD() {
	uint8_t lineBuffer [160 * 2];
	uint16_t lineBufferIndex = 0;
	kputc(0x00);	
	for (uint16_t y = 0; y < 120; y++) {
		lineBufferIndex = 0;
		uint8_t sendWhileReadCounter = 4;
		lineBuffer[0] = 0;
		for (uint16_t x = 1; x < 160 * 2; x++) {
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

void processFrame(uint8_t* buffer) {
	int r;
	int c;
	for (r = 0; r < 120; r++) {
		buffer[r * 160 * 2] = 0;
		for (c = 0; c < 160; c++) {
			while (*((volatile uint8_t *)P_CLK) & 0x1);
			while (!(*((volatile uint8_t *)P_CLK) & 0x1));
			buffer[1 + (r * 160 + c) * 2] = *((volatile uint8_t *)DATA);
			while (*((volatile uint8_t *)P_CLK) & 0x1);
			while (!(*((volatile uint8_t *)P_CLK) & 0x1));
			buffer[1 + (r * 160 + c) * 2 + 1] = *((volatile uint8_t *)DATA);
		}
	}
}

uint8_t extractGrayScalePixel(uint8_t* buffer, int bufferIndex) {
	uint8_t first = buffer[2 * bufferIndex];
	uint8_t second = buffer[2 * bufferIndex + 1];
	uint8_t r = first & 0xF8;
	uint8_t g = (first & 0x7) << 5 | (second >> 3);
	uint8_t b = (second << 3) & 0xF8;
	return grayScale(r, g, b);
}

uint8_t applyKernel(uint8_t* buffer, int row, int column, int* kernel) {
	
	int sum = 0;
	for (int i = -1; i < 2; i++) {
		for (int j=-1; j < 2; j++) {
			if (!(row + i < 0 || column + j < 0 || row + i >= 120 || column + j >= 160)) {
				sum = sum + ((kernel[(4 + 3 * i + j)] * (int) extractGrayScalePixel(buffer, 160 * (row + i) + (column + j))) >> 4);
			}
		}
	}
	
	return sum < 0 ? 0 : sum > 255 ? 255 : (uint8_t) sum;
}

void sendFrame(uint8_t* buffer, int* kernel) {
	char c;
	kputc(0x00);
	int bufferIndex = 0;
	for (int r = 0; r < 120; r++) {
		for (int c = 0; c < 160; c++) {
			kputc(applyKernel(buffer, r, c, kernel) | 0x1);
		}
	}
	return;
}

int main(void) {			
	//reg_write8(UART_TX_CONTROL, 0x01);
	//reg_write8(UART_RX_CONTROL, 0x01);
	
    int kernel[9] = { 1, 2 ,  1, 
					  2,  4,  2, 
					  1,  2,  1};
	uint8_t imageBuffer[160 * 120 * 2 + 120];
	//char c;
	//I2C_init();
	while (true) {
		/*if (kgetc(&c)) {
			kputc(c);
		}*/
		//*((uint8_t *) LED0) = *((uint8_t *) SWITCH0);
		//*((uint8_t *) LED1) = *((uint8_t *) SWITCH1);  
		
		waitForVsync();			
		processFrame(imageBuffer);	
		sendFrame(imageBuffer, kernel);		
	}
	return 0;
}
