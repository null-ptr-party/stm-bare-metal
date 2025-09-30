#include "syscfg.h"

void cfg_exti_interrupt(uint32_t port, uint32_t pin)
{	/* This function configures the supplied port/pin as an exti interrupt. Note that
	the interrupt itself must be configured using the exti and nvic headers */

	uint32_t shift = pin % 4; // modulo pin to get correct offset for each register.

	if (pin <= 3)
	{
		SYSCFG->EXTICR1 = ((SYSCFG->EXTICR1 & ~(0x0FU << 4*shift)) | (port << 4*shift));
	}
	else if (pin <= 7)
	{
		SYSCFG->EXTICR2 = ((SYSCFG->EXTICR2 & ~(0x0FU << 4*shift)) | (port << 4*shift));
	}
	else if (pin <= 11)
	{
		SYSCFG->EXTICR3 = ((SYSCFG->EXTICR3 & ~(0x0FU << 4*shift)) | (port << 4*shift));
	}
	else if (pin <= 15)
	{
		SYSCFG->EXTICR4 = ((SYSCFG->EXTICR4 & ~(0x0FU << 4*shift)) | (port << 4*shift));
	}
}
