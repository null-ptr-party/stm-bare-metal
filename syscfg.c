#include "syscfg.h"

void cfg_exti_interrupt(uint8_t port, uint8_t pin) // configure exti GPIO interrupt.
{
	if (pin <= 3)
	{
		SYSCFG->EXTICR1 = ((SYSCFG->EXTICR1 & ~(0x0F << 4*pin)) | (port << 4*pin));
	}
	else if (pin <= 7)
	{
		SYSCFG->EXTICR2 = ((SYSCFG->EXTICR2 & ~(0x0F << 4*pin)) | (port << 4*pin));
	}
	else if (pin <= 11)
	{
		SYSCFG->EXTICR3 = ((SYSCFG->EXTICR3 & ~(0x0F << 4*pin)) | (port << 4*pin));
	}
	else if (pin <= 15)
	{
		SYSCFG->EXTICR4 = ((SYSCFG->EXTICR4 & ~(0x0F << 4*pin)) | (port << 4*pin));
	}
}
