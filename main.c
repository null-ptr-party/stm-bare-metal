#include <stdint.h>
#include "gpio.h"
#include "rcc.h"
#include "systick.h"
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

// oeate section for NVIC vector table
__attribute__((section(".vectors"))) void(*const table[16 + 163])(void) = {
	// set _estack, reset to first elements of table.
	_estack, _reset, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, systick_handler // systick is the 16th word
};

int main(void)
{
	// start HSE
	start_hse(TRUE); // this bricks the board... why? Well maybe not this. It is actually probably just the PLL

	while (!hse_is_rdy())
	{
		;
	}

	// setup pll1. 250 Mhz target
	struct pll_config pllconfig = { .PLL_PRSCL = 1U, .PLL_SRC = PLL_SRC_HSE, .DIVP_EN = TRUE, .DIVQ_EN = FALSE,
		.DIVR_EN = FALSE, .PLL_IN_RNG = PLL_IN_RNG_FOUR_EIGHT, .VCO_RNG = VCO_RNG_WIDE, .FRAC_EN = FALSE, .DIV_FCTR_P = 0x00, .PLL_MULT = 0x31};

	cfg_pll(&pllconfig, PLL1);
	start_pll(PLL1);

	while (!pll_is_rdy(PLL1))
	{
		; // block until pll ready
	}

	//setup sysclk
	set_sys_clk(SYSCLK_SRC_PLL1);
	// setup systick
	struct systick_setup settings = { .ctr_enbl = CTR_ENABLE,
	.excpt_enbl = EXCPT_ENBL, .clksrc = CLKSRC_PRC, .rld_val = 0x3D08F };

	struct systick_setup* stg_ptr = &settings;

	init_systick(stg_ptr);
	// setup gpio
	enable_gpio_bank(ENABLE_PB); // Enable port B.
	set_gpio_mode(GPIOB, 14U, GPIO_MODE_OUTPUT);

	while (1)
	{
		set_gpio_output(GPIOB, 14U);
		wait_ms(1000);
		reset_gpio_output(GPIOB, 14U);
		wait_ms(1000);
	}
	return 0;
}

// fucntion
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