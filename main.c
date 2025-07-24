#include <stdint.h>
#include "gpio.h"
#include "rcc.h"
#include "systick.h"
#include "usart.h"
#include "shared_tools.h"

// below are defined in linker script. Hence extern
extern void _estack(void);
extern long _sdata, _edata, _sidata, _sbss, _ebss;

// _reset is called on boot
void _reset(void);

// function prototypes:
void cnt_down(volatile uint32_t cnt);
void systick_handler(void);
void wait_ms(uint32_t ms);

// Globals
static volatile uint32_t s_ticks = 0;

// Create section for NVIC vector table
__attribute__((section(".vectors"))) void(*const table[16 + 163])(void) = {
	// set _estack, reset to first elements of table.
	_estack, _reset, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, systick_handler // systick is the 16th word
};

int main(void)
{
	// =======================Configure clocks and pllss=============================

	// start HSE
	start_hse(TRUE);

	while (!hse_is_rdy())
	{
		;
	}

	// setup pll1. 250 Mhz target
	struct pll_config pll1config = { .pll_prscl = 1U, .pll_src = PLL_SRC_HSE, .divp_en = TRUE, .divq_en = FALSE,
		.divr_en = FALSE, .pll_in_rng = PLL_IN_RNG_FOUR_EIGHT, .vco_rng = VCO_RNG_WIDE, .frac_en = FALSE, .div_fctr_p = 0x00, .pll_mult = 0x31};

	cfg_pll(&pll1config, PLL1);
	start_pll(PLL1);

	while (!pll_is_rdy(PLL1))
	{
		; // block until pll ready
	}

	// setup pll2 58,982,400 hz target. 11.79648 PLL multiplier. 11x multiplier 6525 fractional multiplier.
	// this is mainly driven by the desire to have an integer multiple of standard baud rates.
	struct pll_config pll2config = { .pll_prscl = 1U, .pll_src = PLL_SRC_HSE, .divp_en = TRUE, .divq_en = TRUE,
		.divr_en = TRUE, .pll_in_rng = PLL_IN_RNG_FOUR_EIGHT, .vco_rng = VCO_RNG_WIDE, .frac_en = TRUE,
		.div_fctr_p = 0x00U, .pll_mult = 0x0AU, .div_fctr_frac = 0x186FU};

	cfg_pll(&pll2config, PLL2);
	start_pll(PLL2);

	while (!pll_is_rdy(PLL2))
	{
		; // block until pll ready
	}

	//setup sysclk
	set_sys_clk(SYSCLK_SRC_PLL1);

	// setup krnl clocks
	struct krnl_clk_cfg krnl_cfg = { .d2_usart234578 = 1U };
	cfg_krnl_clks(&krnl_cfg);

	// =======================End Clock Setup=============================

	// =======================Setup Systick ==============================
	struct systick_setup settings = { .ctr_enbl = CTR_ENABLE,
	.excpt_enbl = EXCPT_ENBL, .clksrc = CLKSRC_PRC, .rld_val = 0x3D08F };

	struct systick_setup* stg_ptr = &settings;

	init_systick(stg_ptr);

	// =======================End Systick ==============================

	// =======================GPIO setup ===============================
	// setup gpio for user LED
	enable_gpio_bank(ENABLE_PB); // Enable port B.
	set_gpio_mode(GPIOB, 14U, GPIO_MODE_OUTPUT);

	// setup gpio for tx/rx
	enable_gpio_bank(ENABLE_PD);
	// tx
	set_gpio_mode(GPIOD, 8U, GPIO_MODE_ALT_FUNC);
	set_alt_func(GPIOD, 8U, AF1);
	// rx
	set_gpio_mode(GPIOD, 9U, GPIO_MODE_ALT_FUNC);
	set_alt_func(GPIOD, 9U, AF1);

	// =======================End GPIO setup =============================

	// ======================= USART Setup ===============================
	struct usart_cfg usart_cfg = { .word_len = DATABITS8,.oversmpl_mthd = OVERSAMPLE16,.baud = 0x1800 }; // target baud 0x1800
	setup_usart(USART3, &usart_cfg);
	enable_tx(USART3);

	while (1)
	{
		set_gpio_output(GPIOB, 14U);
		wait_ms(1000);
		reset_gpio_output(GPIOB, 14U);
		wait_ms(1000);
	}
	return 0;
}

// function
__attribute__((naked, noreturn)) void _reset(void)
{
	for (long *dst = &_sdata, *source = &_sidata; dst < &_edata;)
	{
		*dst++ = *source++; // Copy data from flash to RAM
	}

	for (long *ptr = &_sbss; ptr < &_ebss; ptr++)
	{ 
		*ptr = 0; // zero sbss data in RAM
	}

	main(); // call main

	for (;;) (void) 0; // loop forever if main returns.
}

void cnt_down(volatile uint32_t cnt)
{
	while (cnt--) (void) 0;
}

void systick_handler(void)
{
	s_ticks++;
}

void wait_ms(uint32_t ms)
{
	uint32_t t_stop = s_ticks + ms;
	
	while (s_ticks < t_stop)
	{
		;
	}
}