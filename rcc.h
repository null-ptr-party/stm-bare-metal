#ifndef RCC_H
#define RCC_h

#include <stdint.h>
// Reset and Clock Control

// 128 4byte words (including reserved)
struct rcc {
	volatile uint32_t CR, HSICFGR, CRRCR, CSICFGR, CFGR, RSV1, D1CFGR, D2CFGR, D3CFGR,
		RSV2, PLLCKSELR, PLLCFGR, PLL1DIVR, PLL1FRACR, PLL2DIVR, PLL2FRACR, PLL3DIVR, PLL3FRACR,
		RSV3, D1CCIPR, D2CCIP1R, D2CCIP2R, D3CCIPR, RSV4, CIER, CIFR, CICR, RVS5, BDCR, CSR, RSV6, AHB3RSTR,
		AHB1RSTR, AHB2RSTR, AHB4RSTR, APB3RSTR, APB1LRSTR, APB1HRSTR, APB2RSTR, APB4RSTR,
		GCR, RSV7, D3AMR, RSV8[9], RSR, AHB3ENR, AHB1ENR, AHB2ENR, AHB4ENR, APB3ENR, APB1LENR, APB1HENR,
		APB2ENR, APB4ENR, RSV9, AHB3LPENR, AHB1LPENR, AHB2LPENR, AHB4LPENR, APB3LPENR, APB1LLPENR,
		APB1HLPENR, APB2LPENR, APB4LPENR, RSV10[4], C1_RSR, C1_AHB3ENR, C1_AHB1ENR, C1_AHB2ENR, C1_AHB4ENR,
		C1_APB3ENR, C1_APB1LENR, C1_APB1HENR, C1_APB2ENR, C1_APB4ENR, RSV11, C1_AHB3LPENR, C1_AHB1LPENR,
		C1_AHB2LPENR, C1_AHB4LPENR, C1_APB3LPENR, C1_APB1LLPENR, C1_APB1HLPENR, C1_APB2LPENR,
		C1_APB4LPENR, RSV12[32];
};
// RCC macro
#define RCC ((struct rcc *)0x58024400)

//<=====================gpio========================================================>
// enum for enable 
enum gpio_enable { ENABLE_PA, ENABLE_PB, ENABLE_PC, ENABLE_PD, ENABLE_PE, ENABLE_PF, ENABLE_PG,
ENABLE_PH, ENABLE_PJ = 9, ENABLE_K = 10 }; // there is a gap between registers H and J

// Enable gpio bank
void enable_gpio_bank(enum gpio_enable port); // RCC_C1_AHB4ENR register

// enable config
void enable_cfg(void);

//<=====================pll======================================================>

// defines for PLL
// Clocksource defines
#define PLL_SRC_HSI 0U
#define PLL_SRC_CSI 1U
#define PLL_SRC_HSE 2U
#define PLL_SRC_NO_CLK 3U

// input range defines
#define PLL_IN_RNG_ONE_TWO 0U
#define PLL_IN_RNG_TWO_FOUR 1U
#define PLL_IN_RNG_FOUR_EIGHT 2U
#define PLL_IN_RNG_EIGHT_SIXTEEN 3U

// vco range defines
#define VCO_RNG_WIDE 0U
#define VCO_RNG_MED 1U

// PLL number defines
#define PLL1 0U
#define PLL2 1U
#define PLL3 2U

// structs
struct pll_config
{
	volatile uint8_t PLL_PRSCL, PLL_SRC, VCO, DIVP_EN, DIVQ_EN, DIVR_EN, PLL_IN_RNG, VCO_RNG, FRAC_EN,
		DIV_FCTR_P, DIV_FCTR_Q, DIV_FCTR_R, DIV_FCTR_FRAC;

	volatile uint32_t PLL_MULT;
};

// functions
void cfg_pll(struct pll_config* config, uint8_t pll);

void start_pll(uint8_t pll);
void start_hse(uint8_t bypass); //Note the nucleo board uses HSE bypass.

uint32_t pll_is_rdy(uint8_t pll);
uint32_t hse_is_rdy(void);

// system clock sources
#define SYSCLK_SRC_HSI 0U
#define SYSCLK_SRC_CSI 1U
#define SYSCLK_SRC_HSE 2U
#define SYSCLK_SRC_PLL1 3U

void set_sys_clk(uint8_t clksrc);

#endif