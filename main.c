#include <stdint.h>
#include <stdio.h>
#include "shared_tools.h"
#include "h723_drivers.h"

// function prototypes:
void cnt_down(volatile uint32_t cnt);
void wait_ms(uint32_t ms);

// Globals
static volatile uint32_t s_ticks = 0;
char inbuff[256] = { 0 };

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
	// enable gpio troubleshooting
	set_gpio_mode(GPIOB, 4U, GPIO_MODE_OUTPUT);
	set_gpio_speed(GPIOB, 4U, IOSPEED_VHIGH);
	set_gpio_outtype(GPIOB, 4U, OUTTYPE_PUSHPULL);
	set_gpio_pullup(GPIOB, 4U, PULLTYPE_NONE);
	// setup port C
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
	// timer GPIO
	enable_gpio_bank(ENABLE_PA);
	// tim 2 channel 1
	set_gpio_mode(GPIOA, 0U, GPIO_MODE_ALT_FUNC);
	set_alt_func(GPIOA, 0U, AF1);
	// tim 2 channel 2
	set_gpio_mode(GPIOB, 3U, GPIO_MODE_ALT_FUNC);
	set_alt_func(GPIOB, 3U, AF1);

	// set up MCO for debugging
	set_mco_src(MCO2, MCO2_SRC_PLL2P);
	set_mco_prsc(MCO1, 1);

	// =======================End GPIO setup =============================
	// ======================= Interrupt Setup ===========================
	
	// setup interrupt for debug PC13
	enable_cfg(); // enable syscfg in rcc
	set_gpio_mode(GPIOC, 13U, GPIO_MODE_INPUT); // gpio input conencted to bluehat button
	unmask_interrupt(40U); // unmask the interrupt in nvic
	cfg_exti_interrupt(EXTI_PORT_C, 13); // configure interrupt for pin13
	cfg_interrupt(13, EDGETYPE_RISING); // unmask interrupt and set to rising edge.

	// ======================= End Interrupt Setup =======================

	// ======================= USART Setup ===============================

	enable_usart3(); // clock usart
	wait_ms(10);
	// 38.4 Mhz clock input to usart
	struct usart_cfg usart_cfg = { .word_len = DATABITS8, .oversmpl_mthd = OVERSAMPLE16, .baud = 0x14DUL }; // target baud 0x1800
	setup_usart(USART3, &usart_cfg);

	// ======================= End USART Setup ===========================
	
	// ======================= Timer Setup ===============================
	// enable timer in RCC
	enable_gptimer(RCC_EN_GPTIM2);
	// set capture modes
	gptim_set_capmode(TIM2, CC_CH1, CC_MODE_SELF);
	gptim_set_capmode(TIM2, CC_CH2, CC_MODE_SELF);

	// set timer mode to slave mode.
	gptim_set_slave_mode(TIM2, GPTIM_SMS_ENC3);

	// set timer reload value
	gptim_set_rld_val(TIM2, 0xfffffffU);
	gptim_ctr_enbl(TIM2);

	// setup tim
	//uint16_t capture = 0;

	while (1)
	{
		//set_gpio_output(GPIOB, 14U);
		;
		//set_gpio_output(GPIOB, 8U); // systick debug
		//wait_ms(1000);
		//reset_gpio_output(GPIOB, 14U);
		//wait_ms(1000);
		//set_gpio_output(GPIOB, 4U);
		//capture = get_atim_capval(TIM1, ATIM_CC_REG_1); // discard every other reading
		//wait_ms(1);
		//reset_gpio_output(GPIOB, 4U);
		//wait_ms(1);
		//set_gpio_output(GPIOB, 4U);
		//capture = get_atim_capval(TIM1, ATIM_CC_REG_1) - capture;
		// (void)get_addr_contents((uint32_t)&(TIM1->CCR1), &capture); need to unit test this.
		//(void)snprintf(inbuff, 256, "%u\n\r", capture);
		//usart_transmit_bytes(USART_DEBUG, inbuff, 10, '\0'); // need to fix
		//reset_gpio_output(GPIOB, 14U);
		
	}
	return 0;
}

void cnt_down(volatile uint32_t cnt)
{
	while (cnt--) (void) 0;
}

void wait_ms(uint32_t ms)
{
	uint32_t t_stop = s_ticks + ms;
	
	while (s_ticks < t_stop)
	{
		;
	}
}

// <============================== definitions for interrupt handler functions ============================================>
// systick handler definition
void systick_handler(void)
{
	s_ticks++;
}

// interrupt definition for exti13
void exti13_iqr(void)
{
	EXTI->CPUPR1 |= BIT(13); // clear pending bit.
	ser_memdump_range_req(inbuff, 256);
}
