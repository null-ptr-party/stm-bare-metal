#include <stdint.h>

void set_bits(volatile uint32_t *reg_addr, uint32_t bitmask)
{
	// takes pointer reg address
	// sets bits that correspond to bitmask
	*reg_addr |= bitmask;
}

void clr_bits(uint32_t reg, uint32_t bitmask)
{
	// clears register bits that correspond to bits in bitmask (1 in bitmask clears bit).
	*reg_addr |= ~bitmask;
}