#include <stdint.h>
#include "rcc.h"
#include "shared_tools.h"

void enable_gpio_bank(enum gpio_enable port)
{// RCC_C1_AHB4ENR register
	RCC->C1_AHB4ENR &= ~(0x01U << port); // clear bit of interest. Leave others.
	RCC->C1_AHB4ENR |= (0x01U << port);
	// No return
}

void enable_cfg(void)
{
	RCC->APB4ENR &= ~(0x01U << 1);
	RCC->APB4ENR |= (0x01U << 1);
}

// configures PLL using pll_config struct
void cfg_pll(struct pll_config* config, uint8_t pll)
{
	//note that PLL macros are pll1 -> 0, pll2->1, pll3->2

	// set pll source clock and prescaler. note prescaler range is 1 to 63. 0 is precsaler disabled
	RCC->PLLCKSELR &= ~(0x03U); // clear bits
	RCC->PLLCKSELR |= (0x03U & config->PLL_SRC); // set pll clock source.
	// set prescaler
	RCC->PLLCKSELR &= ~(0x3fU << (4 + 8 * (uint32_t)pll)); // clear bits
	RCC->PLLCKSELR |= ((0x3fU & config->PLL_PRSCL) << (4 + 8 * (uint32_t)pll));
	// set pll range
	RCC->PLLCFGR &= ~(0x03U << (4 * (uint32_t)pll + 2U)); // clear bits
	RCC->PLLCFGR |= ((0x03U & config->PLL_IN_RNG) << (4 * (uint32_t)pll + 2U));
	// set vco range
	RCC->PLLCFGR &= ~(0x01U << (4 * (uint32_t)pll + 1U)); // clear bits
	RCC->PLLCFGR |= ((0x01U & config->VCO_RNG) << (4 * (uint32_t)pll + 1U));

	switch (pll)
	{
	case(PLL1):
		// set pll multiplication factor (set vco)
		// Note this must be set appropriately such that
		// it is in the range set by VCO range.
		// set pll multiplication factor (set vco).  Range is 1 to 512. note 0 is 1x
		RCC->PLL1DIVR &= ~(0x1FFU);
		RCC->PLL1DIVR |= (0x1FFU & config->PLL_MULT);
		// set pll div factor for P output. Range is 1 to 128. note 0 is 1x
		RCC->PLL1DIVR &= ~(0x7FU << 9U);
		RCC->PLL1DIVR |= ((0x7FU & config->DIV_FCTR_P) << 9U);
		// set pll div factor for Q output
		RCC->PLL1DIVR &= ~(0x7FU << 16U);
		RCC->PLL1DIVR |= ((0x7FU & config->DIV_FCTR_Q) << 16U);
		// set pll div factor for R output
		RCC->PLL1DIVR &= ~(0x7FU << 24U);
		RCC->PLL1DIVR |= ((0x7FU & config->DIV_FCTR_R) << 24U);

		// Enable outputs that are enabled in config struct
		if (config->DIVP_EN)
		{
			RCC->PLLCFGR |= BIT(16);
		}
		if (config->DIVQ_EN)
		{
			RCC->PLLCFGR |= BIT(17);
		}
		if (config->DIVR_EN)
		{
			RCC->PLLCFGR |= BIT(18);
		}
		break;

	case(PLL2):
		// set pll multiplication factor (set vco)
		RCC->PLL2DIVR &= ~(0x1FFU);
		RCC->PLL2DIVR |= (0x1FFU & config->PLL_MULT);
		// set pll div factor for P output
		RCC->PLL2DIVR &= ~(0x7FU << 9U);
		RCC->PLL2DIVR |= ((0x7FU & config->DIV_FCTR_P) << 9U);
		// set pll div factor for Q output
		RCC->PLL2DIVR &= ~(0x7FU << 16U);
		RCC->PLL2DIVR |= ((0x7FU & config->DIV_FCTR_Q) << 16U);
		// set pll div factor for R output
		RCC->PLL2DIVR &= ~(0x7FU << 24U);
		RCC->PLL2DIVR |= ((0x7FU & config->DIV_FCTR_R) << 24U);

		// Enable outputs that are enabled in config struct
		if (config->DIVP_EN)
		{
			RCC->PLLCFGR |= BIT(19);
		}
		if (config->DIVQ_EN)
		{
			RCC->PLLCFGR |= BIT(20);
		}
		if (config->DIVR_EN)
		{
			RCC->PLLCFGR |= BIT(21);
		}

		break;

	case(PLL3):
		// set pll multiplication factor (set vco)
		RCC->PLL3DIVR &= ~(0x1FFU);
		RCC->PLL3DIVR |= (0x1FFU & config->PLL_MULT);
		// set pll div factor for P output
		RCC->PLL3DIVR &= ~(0x7FU << 9U);
		RCC->PLL3DIVR |= ((0x7FU & config->DIV_FCTR_P) << 9U);
		// set pll div factor for Q output
		RCC->PLL3DIVR &= ~(0x7FU << 16U);
		RCC->PLL3DIVR |= ((0x7FU & config->DIV_FCTR_Q) << 16U);
		// set pll div factor for R output
		RCC->PLL3DIVR &= ~(0x7FU << 24U);
		RCC->PLL3DIVR |= ((0x7FU & config->DIV_FCTR_R) << 24U);

		// Enable outputs that are enabled in config struct
		if (config->DIVP_EN)
		{
			RCC->PLLCFGR |= BIT(22);
		}
		if (config->DIVQ_EN)
		{
			RCC->PLLCFGR |= BIT(23);
		}
		if (config->DIVR_EN)
		{
			RCC->PLLCFGR |= BIT(24);
		}

		break;

	default:
		break;
	}
}

	
// PLL toggled on by separate function
// 
void start_pll(uint8_t pll)
{
	RCC->CR |= (0x01U << (2 * pll + 24U));
}

void start_hse(uint8_t bypass)
{
	// starts the HSE. Sets bypass bit if
	// bypass is true. Note Nucleo board has
	// clock supplied by stlink
	if (bypass)
	{
		RCC->CR |= BIT(18); // enable bypass
	}

	RCC->CR |= BIT(16); // Start HSE
}

uint32_t hse_is_rdy(void)
{
	return (uint32_t)(0x01U & (RCC->CR >> 17U));
}

uint32_t pll_is_rdy(uint8_t pll)
{
	return (uint32_t)(0x01U & (RCC->CR >> (25 + 2 * pll)));
}

void set_sys_clk(uint8_t clksrc)
{
	RCC->CFGR &= ~(0x03U);
	RCC->CFGR |= (0x03U & clksrc);
}