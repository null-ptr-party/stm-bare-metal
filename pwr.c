#include <stdint.h>
#include "pwr.h"
#include "shared_tools.h"

void set_core_scl(uint8_t scale)
{
	PWR->PWR_D3CR |= ((0x03U & scale) << 14);
}
void blk_til_vos_rdy(void)
{
	while ((PWR->PWR_D3CR & BIT(13)) == 0);
}

void cfg_pwr_input(void)
{
	// hard code for now.
	// Note: this register is weird and can only
	// be cleared by a long hard reset (no power).
	PWR->PWR_CR3 |= 0x06U;
}

void blk_til_actvos_rdy(void)
{
	while ((PWR->PWR_CSR1 & BIT(13)) == 0);
}