#include <stdint.h>
#include "rcc.h"
#include "shared_tools.h"

void enable_gpio_bank(enum gpio_enable port)
{	/* Enables the desired gpio port
	in the RCC_C1_AHB4ENR register. */
	RCC->C1_AHB4ENR &= ~(0x01U << port); // clear bit of interest. Leave others.
	RCC->C1_AHB4ENR |= (0x01U << port);
	// No return
}

void enable_stlink(void)
{
	// uart 3 is connected to stlink. Nucleo board specific.
	RCC->APB1LENR |= BIT(18);
}

void enable_cfg(void)
{	// enable syscfg.
	RCC->APB4ENR &= ~(0x01U << 1);
	RCC->APB4ENR |= (0x01U << 1);
}

// configures PLL using pll_config struct
void cfg_pll(struct pll_config* config, uint8_t pll)
{
	/* This function configures the target PLL based on the contents
	of the config struct. Note that no div/multipliers in the struct
	can be 0.*/
	//note that PLL macros are pll1 -> 0, pll2->1, pll3->2

	// todo for PLL: need it to where user just enters the desired number for mult/div factor.
	// for fractional, the value should be calculated from a double.
	// set pll source clock and prescaler. note prescaler range is 1 to 63. 0 is precsaler disabled
	RCC->PLLCKSELR &= ~(0x03U); // clear bits
	RCC->PLLCKSELR |= (0x03U & config->pll_src); // set pll clock source.
	// set prescaler
	RCC->PLLCKSELR &= ~(0x3fU << (4 + 8 * (uint32_t)pll)); // clear bits
	RCC->PLLCKSELR |= ((0x3fU & config->pll_prscl) << (4 + 8 * (uint32_t)pll));
	// set pll range
	RCC->PLLCFGR &= ~(0x03U << (4 * (uint32_t)pll + 2U)); // clear bits
	RCC->PLLCFGR |= ((0x03U & config->pll_in_rng) << (4 * (uint32_t)pll + 2U));
	// set vco range
	RCC->PLLCFGR &= ~(0x01U << (4 * (uint32_t)pll + 1U)); // clear bits
	RCC->PLLCFGR |= ((0x01U & config->vco_rng) << (4 * (uint32_t)pll + 1U));

	volatile uint32_t* divr_ptr = &RCC->PLL1DIVR;
	volatile uint32_t* fracr_ptr = &RCC->PLL1FRACR;

	// create handle to register for desired pll.
	switch (pll)
	{
	case(PLL1):
		divr_ptr = &RCC->PLL1DIVR;
		fracr_ptr = &RCC->PLL1FRACR;
		break;

	case(PLL2):
		divr_ptr = &RCC->PLL2DIVR;
		fracr_ptr = &RCC->PLL2FRACR;
		break;

	case(PLL3):
		divr_ptr = &RCC->PLL3DIVR;
		fracr_ptr = &RCC->PLL3FRACR;
		break;

	default:
		break;
	}

	*divr_ptr &= ~(0x1FFU);
	*divr_ptr |= (0x1FFU & (config->pll_mult - 1)); // subtract 1 to make 1:1 match.
	// set pll div factor for P output. Range is 1 to 128. note 0 is 1x. 
	// ** important. 0 is not a valid option here. it will make it roll over
	*divr_ptr &= ~(0x7FU << 9U);
	*divr_ptr |= ((0x7FU & (config->div_fctr_p - 1)) << 9U); // subtract 1 to make 1:1 match.
	// set pll div factor for Q output
	*divr_ptr &= ~(0x7FU << 16U);
	*divr_ptr |= ((0x7FU & (config->div_fctr_q - 1)) << 16U);
	// set pll div factor for R output
	*divr_ptr &= ~(0x7FU << 24U);
	*divr_ptr |= ((0x7FU & (config->div_fctr_r - 1)) << 24U);

	if (config->frac_en)
	{
		// enable fractional bit
		RCC->PLLCFGR &= ~(BIT(4*pll)); // note this is necessary since PLL sets on change from 0->1.
		RCC->PLLCFGR |= BIT(4*pll);
		// set fractional part of multiplier if enabled
		*fracr_ptr &= ~(0x1FFFU << 3); // Clear bit
		*fracr_ptr |= ((0x1FFFU & config->div_fctr_frac) << 3);
	}
	// Enable outputs that are enabled in config struct
	if (config->divp_en)
	{
		RCC->PLLCFGR |= BIT((16 + 3*pll));
	}
	if (config->divq_en)
	{
		RCC->PLLCFGR |= BIT((17 + 3*pll));
	}
	if (config->divr_en)
	{
		RCC->PLLCFGR |= BIT((18 + 3*pll));
	}
}

	
// PLL toggled on by separate function
// 
void start_pll(uint8_t pll)
{	// Start the target PLL
	RCC->CR |= (0x01U << (2 * pll + 24U));
}

void start_hse(uint8_t bypass)
{
	/* starts the HSE. Sets bypass bit if
	bypass is true. Note Nucleo board has
	clock supplied by stlink and thus uses bypass. */
	if (bypass)
	{
		RCC->CR |= BIT(18); // enable bypass
	}

	RCC->CR |= BIT(16); // Start HSE
}

uint32_t hse_is_rdy(void)
{	/* Blocks until the high speed external oscillator
	is ready*/
	return (uint32_t)(0x01U & (RCC->CR >> 17U));
}

uint32_t pll_is_rdy(uint8_t pll)
{	/* Blocks until the target PLL
	is ready*/
	return (uint32_t)(0x01U & (RCC->CR >> (25 + 2 * pll)));
}

void set_sys_clk(uint8_t clksrc)
{
	RCC->CFGR &= ~(0x03U);
	RCC->CFGR |= (0x03U & clksrc);
}

void cfg_krnl_clks(struct krnl_clk_cfg* cfg)
{
	/* Controls mux settings kernel clocks based off the contents of cfg struct.
	see STM32H723ZG reference manual RCC section for mux options for each peripheral*/
	uint32_t word = (0x03U & cfg->d1_fmc) | ((0x03U & cfg->d1_octospi) << 4) |
		((0x03U & cfg->d1_sdmmc) << 17) | ((0x03U & cfg->d1_per_clk) << 28);

	RCC->D1CCIPR |= word;

	// setup krnl clk for d2 (first d2 reg)
	word = (0x03U & cfg->d2_sai1) | ((0x03U & cfg->d2_spi123) << 12) |
		((0x03U & cfg->d2_spi45) << 16) | ((0x03U & cfg->d2_spdifrx) << 20) |
		((0x01U & cfg->d2_dfsdm1) << 24) | ((0x03U & cfg->d2_fdcan) << 28) |
		((0x01U & cfg->d2_swpmi) << 31);

	RCC->D2CCIP1R |= word;

	// setup krnl clk for d2 (first d2 reg)
	word = (0x03U & cfg->d2_usart234578) | ((0x03U & cfg->d2_usart16910) << 3) |
		((0x03U & cfg->d2_rng) << 8) | ((0x03U & cfg->d2_i2c1235) << 12) |
		((0x03U & cfg->d2_usb) << 20) | ((0x03U & cfg->d2_cec) << 22) |
		((0x07U & cfg->d2_swpmi) << 28);

	RCC->D2CCIP2R |= word;

	// setup krnl clk for d3.
	word = (0x03U & cfg->d3_lpuart1) | ((0x03U & cfg->d3_i2c4) << 8) |
		((0x03U & cfg->d3_lptim2) << 10) | ((0x03U & cfg->d3_lptim345) << 13) |
		((0x03U & cfg->d3_adc) << 16) | ((0x03U & cfg->d3_sai4a) << 21) |
		((0x03U & cfg->d3_sai4b) << 24) | ((0x03U & cfg->d3_spi6) << 28);

	RCC->D3CCIPR |= word;

}

void enable_usart3(void)
{	
	RCC->APB1LENR |= BIT(18);
}

void enable_usart2(void)
{
	RCC->APB1LENR |= BIT(17);
}

// MCO.
void set_mco_src(uint8_t mco, uint8_t src)
{
	// use mco macros
	if (mco == MCO1)
	{
		RCC->CFGR &= ~(0x07U << 22U); // clear bits
		RCC->CFGR |= ((0x07U & src) << 22U);
	}
	if (mco == MCO2)
	{
		RCC->CFGR &= ~(0x07U << 31U); // clear bits
		RCC->CFGR |= ((0x07U & src) << 29U);
	}
}

void set_mco_prsc(uint8_t mco, uint8_t prsc)
{
	// use mco macros
	if (mco == MCO1)
	{
		RCC->CFGR &= ~(0x0FU << 18U); // clear bits
		RCC->CFGR |= ((0xFU & prsc) << 18U);
	}
	if (mco == MCO2)
	{
		RCC->CFGR &= ~(0x0FU << 25U); // clear bits
		RCC->CFGR |= ((0xFU & prsc) << 25U);
	}
}

// Timer functions
// advanced timers
void enable_adv_timer(uint8_t adv_tim)
{	// Enables the advanced timer 1 or 8.
	RCC->APB2ENR |= BIT(adv_tim);
}

// general purpose timer
void enable_gptimer(uint8_t gptimer)
{
	RCC->APB1LENR |= BIT(gptimer);
}
