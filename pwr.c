#include <stdint.h>
#include "pwr.h"
#include "shared_tools.h"

void set_core_scl(uint8_t scale)
{
	/* this function sets the desired voltage scaling for the core.
	Note that certain clocks speeds cannot be reached without raising
	the core scaling. Prior to using this functoin, the actvos ready flag
	must be 1.
	*/
	volatile uint32_t unused = 0;
	PWR->PWR_D3CR = ((PWR->PWR_D3CR & ~VOS_MASK) | (scale << 14U));
	unused = (PWR->PWR_D3CR & ~VOS_MASK); // read bit to delay.
	(void)unused; // avoid compiler warn.
	while ((PWR->PWR_D3CR & BIT(13)) == 0); // wait for VOSready.
}

void blk_til_vos_rdy(void)
{	// blocks until the voltage scaling is ready.
	while ((PWR->PWR_D3CR & BIT(13)) == 0);
}

void cfg_pwr_input(void)
{
	// Important notes because STM332H7 documentation is
	// kind of crap. The STM32H723 series does NOT have an
	// SMPS converter, so the only bits avaialble for configuration
	// are bypass and LDOEN. This hardcodes for LDOEN.
	volatile uint32_t unused = 0;
	PWR->PWR_CR3 = ((PWR->PWR_CR3 & ~PWR_CFG_MASK) | 0x02UL);
	unused = PWR->PWR_CR3 & PWR_CFG_MASK; // readback val for delay
	(void)unused;  // to prevent compiler complaint.
	while ((PWR->PWR_CSR1 & BIT(13)) == 0); // wait till actvos is ready.
}

void blk_til_actvos_rdy(void)
{	// block until the current vos setting is ready.
	while ((PWR->PWR_CSR1 & BIT(13)) == 0);
}