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
#define VCO_RNG_WIDE 0U // Range 192-836 MHz
#define VCO_RNG_MED 1U // Range 150-420 Mhz

// PLL number defines
#define PLL1 0U
#define PLL2 1U
#define PLL3 2U

// PLL structs
struct pll_config
{
	uint8_t pll_prscl, pll_src, vco, divp_en, divq_en, divr_en, pll_in_rng, vco_rng, frac_en,
		div_fctr_p, div_fctr_q, div_fctr_r;

	uint32_t pll_mult, div_fctr_frac;
};

// Kernel clock selection register structs (separated by domain)
struct krnl_clk_cfg
{
	uint8_t d1_per_clk, d1_sdmmc, d1_octospi, d1_fmc, d2_swpmi, d2_fdcan,
		d2_dfsdm1, d2_spdifrx, d2_spi45, d2_spi123, d2_sai1, d2_lptim1,
		d2_cec, d2_usb, d2_i2c1235, d2_rng, d2_usart16910, d2_usart234578,
		d3_spi6, d3_sai4b, d3_sai4a, d3_adc, d3_lptim345, d3_lptim2, d3_i2c4,
		d3_lpuart1;
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

// configure kernel clocks
void cfg_krnl_clks(struct krnl_clk_cfg* cfg);
void enable_usart3(void);
void enable_usart2(void);

// MCO.
// MCO1
#define MCO1 0U
#define MCO2 1U

#define MCO1_SRC_HSI 0U
#define MCO1_SRC_LSE 1U
#define MCO1_SRC_HSE 2U
#define MCO1_SRC_PLL1Q 3U
#define MCO1_SRC_HSI48 4U

// MCO2
#define MCO2_SRC_SYSCLK 0U
#define MCO2_SRC_PLL2P 1U
#define MCO2_SRC_HSE 2U
#define MCO2_SRC_PLL1P 3U
#define MCO2_SRC_CSI 4U
#define MCO2_SRC_LSI 5U

void set_mco_src(uint8_t mco, uint8_t src);

void set_mco_prsc(uint8_t mco, uint8_t prscl);

//<=====================timers========================================================>
// Adv Timer Defines
#define RCC_EN_ADV_TIM1 0
#define RCC_EN_ADV_TIM2 1

// Adv Timer Enable
void enable_adv_timer(uint8_t adv_tim);


#endif