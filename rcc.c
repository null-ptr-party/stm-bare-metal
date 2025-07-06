#include "rcc.h"

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

void  