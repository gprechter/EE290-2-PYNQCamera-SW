#include "mmio.h"

void reg_write8(uintptr_t addr, uint8_t data)
{
        volatile uint8_t *ptr = (volatile uint8_t *) addr;
        *ptr = data;
}

uint8_t reg_read8(uintptr_t addr)
{
        volatile uint8_t *ptr = (volatile uint8_t *) addr;
        return *ptr;
}

