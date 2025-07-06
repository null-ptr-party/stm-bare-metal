#include "systick.h"
#include "rcc.h"

void set_systick_ctr(uint32_t ctr_status)
{
	//SYSTICK->CSR &= ((uint32_t)(~0x01U)); // clear bit
	SYSTICK->CSR |= (uint32_t)ctr_status;
}

void set_systick_clksrc(uint32_t clksrc)
{
	//SYSTICK->CSR &= ((uint32_t)(~(0x01U << 2)));
	SYSTICK->CSR |= (uint32_t)(clksrc << 2);
}

void set_systick_excpt(uint32_t excpt_enbl)
{
	//SYSTICK->CSR &= ((uint32_t)(~(0x01U << 1))); // clear bit
	SYSTICK->CSR |= (uint32_t)(excpt_enbl << 1); // set bit
}

void clear_systick(void)
{
	// clear field. note writing any value clear field
	SYSTICK->CVR |= 0x0U;
}

void set_rld_val(uint32_t rld_val)
{
	SYSTICK->RVR &= 0x000000U; // clear reload value
	SYSTICK->RVR |= (0xFFFFFFU & rld_val); // set reload val
}

void init_systick(struct systick_setup *setup)
{
	// systick setup function:
	// Note below is necessary because the systick is
	// uninitialized at boot.

	// 1. program reload value
	// 2. Clear current value
	// 3. Program control and status register
	set_rld_val(setup->rld_val);
	clear_systick();
	//set_systick_clksrc(setup->clksrc);
	//set_systick_excpt(setup->excpt_enbl);
	//set_systick_ctr(setup->ctr_enbl);
	SYSTICK->CSR |= ((setup->ctr_enbl & BIT(0)) | ((setup->excpt_enbl << 1) & BIT(1)) | ((setup->clksrc << 2) & BIT(2)));
	void enable_cfg(void);
}