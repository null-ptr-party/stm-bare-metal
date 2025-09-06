#include <stdint.h>
#include "gpio.h"
#include "rcc.h"
#include "systick.h"
#include "usart.h"
#include "shared_tools.h"
#include "pwr.h"
#include "debug.h"
#include "exti.h"
#include "syscfg.h"
#include "nvic.h"

// below are defined in linker script. Hence extern
extern void _estack(void);
extern long _sdata, _edata, _sidata, _sbss, _ebss;

// _reset is called on boot
void _reset(void);

// function prototypes:
void cnt_down(volatile uint32_t cnt);
void systick_handler(void);
void wait_ms(uint32_t ms);
void exti13_iqr(void);

// Globals
static volatile uint32_t s_ticks = 0;
char inbuff[256] = { 0 };

// Create section for NVIC vector table
__attribute__((section(".vectors"))) void(*const table[16 + 163])(void) = {
	// set _estack, reset to first elements of table.
	_estack, _reset, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, systick_handler, // systick is the 16th word
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, exti13_iqr
};

int main(void)
{
	// ====================== Configure power and voltage scaling====================
	cfg_pwr_input();
	// change VOS scaling to 2
	set_core_scl(VOS_SCALE_2);
	
	// =======================Configure clocks and pllss=============================
	// start HSE
	start_hse(TRUE);

	while (!hse_is_rdy())
	{
		;
	}

	while ((PWR->PWR_D3CR & BIT(13)) == 0);
	// setup pll1. 250 Mhz target. PLL div_fctrs cannot be 0.
	struct pll_config pll1config = { .pll_prscl = 1U, .pll_src = PLL_SRC_HSE, .divp_en = TRUE, .divq_en = FALSE,
		.divr_en = FALSE, .pll_in_rng = PLL_IN_RNG_FOUR_EIGHT, .vco_rng = VCO_RNG_MED, .frac_en = FALSE, .div_fctr_p = 1U,
		.div_fctr_q = 1U, .div_fctr_r = 1U, .pll_mult = 50U};

	cfg_pll(&pll1config, PLL1);
	start_pll(PLL1);

	while (!pll_is_rdy(PLL1))
	{
		; // block until pll ready
	}

	set_sys_clk(SYSCLK_SRC_PLL1);
	// setup pll2 384,000,000 hz target. 76.8 PLL multiplier. 11x multiplier 6525 fractional multiplier. Must be in range
	// 192-836 Mhz
	// this is mainly driven by the desire to have an integer multiple of standard baud rates.
	struct pll_config pll2config = { .pll_prscl = 1U, .pll_src = PLL_SRC_HSE, .divp_en = TRUE, .divq_en = TRUE,
		.divr_en = TRUE, .pll_in_rng = PLL_IN_RNG_FOUR_EIGHT, .vco_rng = VCO_RNG_WIDE, .frac_en = TRUE,
		.div_fctr_p = 10U, .pll_mult = 76U, .div_fctr_frac = 6555U, .div_fctr_q = 10U, .div_fctr_r = 10U };

	cfg_pll(&pll2config, PLL2);
	start_pll(PLL2);

	while (!pll_is_rdy(PLL2))
	{
		; // block until pll ready
	}

	// setup krnl clocks
	struct krnl_clk_cfg krnl_cfg = { .d2_usart234578 = 1U };
	cfg_krnl_clks(&krnl_cfg);

	// =======================End Clock Setup=============================

	// =======================Setup Systick ==============================
	struct systick_setup settings = { .ctr_enbl = CTR_ENABLE,
	.excpt_enbl = EXCPT_ENBL, .clksrc = CLKSRC_PRC, .rld_val = 0x3d090 };

	struct systick_setup* stg_ptr = &settings;

	init_systick(stg_ptr);

	// =======================End Systick ==============================

	// =======================GPIO setup ===============================
	// setup gpio for user LED
	enable_gpio_bank(ENABLE_PB); // Enable port B.
	set_gpio_mode(GPIOB, 14U, GPIO_MODE_OUTPUT);
	enable_gpio_bank(ENABLE_PC); // Enable port C.
	set_gpio_mode(GPIOC, 9U, GPIO_MODE_ALT_FUNC);
	set_gpio_speed(GPIOC, 9U, IOSPEED_VHIGH);
	set_alt_func(GPIOC, 9U, AF0); // MCO output. PLL2
	// setup GPIO for USARt
	// setup gpio for tx/rx
	enable_gpio_bank(ENABLE_PD);
	// tx
	set_gpio_mode(GPIOD, 8U, GPIO_MODE_ALT_FUNC);
	set_gpio_speed(GPIOD, 8U, IOSPEED_VHIGH);
	set_gpio_outtype(GPIOD, 8U, OUTTYPE_PUSHPULL);
	set_gpio_pullup(GPIOD, 8U, PULLTYPE_NONE);
	set_alt_func(GPIOD, 8U, AF7);
	// rx
	set_gpio_mode(GPIOD, 9U, GPIO_MODE_ALT_FUNC);
	set_alt_func(GPIOD, 9U, AF7);
	set_gpio_speed(GPIOD, 9U, IOSPEED_VHIGH);
	set_gpio_mode(GPIOB, 8U, GPIO_MODE_OUTPUT);
	set_gpio_speed(GPIOB, 8U, IOSPEED_VHIGH);

	// set up MCO for debugging
	set_mco_src(MCO2, MCO2_SRC_PLL2P);
	set_mco_prsc(MCO1, 1);

	// =======================End GPIO setup =============================
	// ======================= Interrupt Setup ===========================
	enable_cfg(); // enable syscfg in rcc
	set_gpio_mode(GPIOC, 13U, GPIO_MODE_INPUT);
	unmask_interrupt(40U);
	cfg_exti_interrupt(EXTI_PORT_C, 13); // configure interrupt for pin13
	cfg_interrupt(13, EDGETYPE_RISING); // unmask interrupt and set to rising edge.
	
	// set up interrupt for debug PC13
	// set exti13 for port C

	// ======================= USART Setup ===============================
	// clock usart
	enable_usart3();
	wait_ms(10);
	// 38.4 Mhz clock input to usart
	struct usart_cfg usart_cfg = { .word_len = DATABITS8, .oversmpl_mthd = OVERSAMPLE16, .baud = 0x14DUL }; // target baud 0x1800
	setup_usart(USART3, &usart_cfg);
	//enable_tx(USART3);
	//memdump_range(0x20000090U, 0x200000f0U, inbuff, 256U);
	
	while (1)
	{
		set_gpio_output(GPIOB, 14U);
		//set_gpio_output(GPIOB, 8U); // systick debug
		wait_ms(1000);
		reset_gpio_output(GPIOB, 14U);
		//reset_gpio_output(GPIOB, 8U);
		//usart_read_with_echo(USART_DEBUG, inbuff, 256U);
		//usart_read_bytes(USART3, inbuff, 256, '\n');
		//usart_read_bytes(USART3, inbuff, 256, '\r');
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

void exti13_iqr(void)
{
	read_memrange(inbuff, 256);
	EXTI->CPUPR1 |= BIT(13); // clear pending bit.
}