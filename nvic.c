#include "nvic.h"
#include "shared_tools.h"

// functions
void unmask_interrupt(uint8_t interrupt_num)
{
	// enables NVIC.
	if (interrupt_num <= 31)
	{
		NVIC->ISER0 |= BIT(interrupt_num);
	}
	else if (interrupt_num <= 63)
	{
		NVIC->ISER1 |= BIT((interrupt_num % 32));
	}
	else if (interrupt_num <= 95)
	{
		NVIC->ISER2 |= BIT((interrupt_num % 32));
	}
	else if (interrupt_num <= 127)
	{
		NVIC->ISER3 |= BIT((interrupt_num % 32));
	}
	else if (interrupt_num <= 159)
	{
		NVIC->ISER4 |= BIT((interrupt_num % 32));
	}
	else if (interrupt_num <= 191)
	{
		NVIC->ISER5 |= BIT((interrupt_num % 32));
	}
	else if (interrupt_num <= 223)
	{
		NVIC->ISER6 |= BIT((interrupt_num % 32));
	}
	else
	{
		NVIC->ISER6 |= BIT((interrupt_num % 32));
	}
}