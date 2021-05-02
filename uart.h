#define UART_TX_ADDR 0x10013000
#define UART_RX_ADDR 0x10013004
#define UART_TX_CONTROL 0x10013008
#define UART_RX_CONTROL 0x1001300C
#define REG32(p, i)     ((p)[(i) >> 2])
#include <stdint.h>
#include <stdbool.h>

static volatile uint32_t * const uart_tx = (void *)(UART_TX_ADDR);
static volatile uint32_t * const uart_rx = (void *)(UART_RX_ADDR);

static inline void kputc(char c)
{
        volatile uint32_t *tx = &REG32(uart_tx, 0x00);
        while ((int32_t)(*tx) < 0);
        *tx = c;
}

static bool kgetc(char *c) 
{
	volatile uint32_t *rx = &REG32(uart_rx, 0x00);
	int32_t value = *rx;
	bool valuePresent = (value >= 0);
	*c = ((uint8_t)value);
	return valuePresent;
}

extern void kputs(const char *, bool newline);

