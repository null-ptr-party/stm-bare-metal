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
void cfg_pll(struct pll_config* config, uint8_t pll_num)
{
	//note that PLL macros are pll1 -> 0, pll2->1, pll3->2
	/* todo: need to make this capable of configuring in any state other than reset*/

	// set pll source clock and prescaler
	RCC->PLL_CKSELR |= ((0x03) & config->PLL_SRC); // set pll clock source.
	// set prescaler
	RCC->PLL_CKSELR |= ((0x3f & config->PLL_PRSCL) << (4 + 8*(uint32_t)pll_num));
	// set pll range
	RCC->PLL_CFGR |= ((0x03 & config->PLL_IN_RNG) << (4*(uint32_t)pll_num + 2U));
	// set vco range
	RCC->PLL_CFGR |= ((0x01 & config->VCO_RNG) << (4*(uint32_t)pll_num + 1U));

	switch (pll_num)
	{
	case(PLL1):
		// set pll multiplication factor (set vco)
		// Note this must be set appropriately such that
		// it is in the range set by VCO range.
		RCC->PLL1DIVR |= (0x1FF & config->PLL_MULT)
		// set pll div factor for P output
		RCC->PLL1DIVR |= ((0x7F & config->DIV_FCTR_P) << 9U)
		// set pll div factor for Q output
		RCC->PLL1DIVR |= ((0x7F & config->DIV_FCTR_Q) << 16U)
		// set pll div factor for R output
		RCC->PLL1DIVR |= ((0x7F & config->DIV_FCTR_R) << 24U)

		// Enable outputs that are enabled in config struct
		if (config->DIVP_EN)
		{
			RCC->PLL_CFGR |= BIT(16);
		}
		if (config->DIVQ_EN)
		{
			RCC->PLL_CFGR |= BIT(17);
		}
		if (config->DIVR_EN)
		{
			RCC->PLL_CFGR |= BIT(18);
		}
		break

	case(PLL2):
		// set pll multiplication factor (set vco)
		RCC->PLL2DIVR |= (0x1FF & config->PLL_MULT)
		// set pll div factor for P output
		RCC->PLL2DIVR |= ((0x7F & config->DIV_FCTR_P) << 9U)
		// set pll div factor for Q output
		RCC->PLL2DIVR |= ((0x7F & config->DIV_FCTR_Q) << 16U)
		// set pll div factor for R output
		RCC->PLL2DIVR |= ((0x7F & config->DIV_FCTR_R) << 24U)

		// Enable outputs that are enabled in config struct
		if (config->DIVP_EN)
		{
			RCC->PLL_CFGR |= BIT(19);
		}
		if (config->DIVQ_EN)
		{
			RCC->PLL_CFGR |= BIT(20);
		}
		if (config->DIVR_EN)
		{
			RCC->PLL_CFGR |= BIT(21);
		}

		break

	case(PLL3):
		// set pll multiplication factor (set vco)
		RCC->PLL3DIVR |= (0x1FF & config->PLL_MULT)
		// set pll div factor for P output
		RCC->PLL3DIVR |= ((0x7F & config->DIV_FCTR_P) << 9U)
		// set pll div factor for Q output
		RCC->PLL3DIVR |= ((0x7F & config->DIV_FCTR_Q) << 16U)
		// set pll div factor for R output
		RCC->PLL3DIVR |= ((0x7F & config->DIV_FCTR_R) << 24U)

		// Enable outputs that are enabled in config struct
		if (config->DIVP_EN)
		{
			RCC->PLL_CFGR |= BIT(22);
		}
		if (config->DIVQ_EN)
		{
			RCC->PLL_CFGR |= BIT(23);
		}
		if (config->DIVR_EN)
		{
			RCC->PLL_CFGR |= BIT(24);
		}

		break

	default:
		break
	}

	// PLL toggled on by separate function

}