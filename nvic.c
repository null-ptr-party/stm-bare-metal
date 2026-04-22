#include "nvic.h"
#include "shared_tools.h"
#include <stdint.h>

// functions
void unmask_interrupt(uint32_t interrupt_num)
{
	// enables NVIC. Note accesses must be 32 bit aligned.
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
	else if (interrupt_num <= 255)
	{
		NVIC->ISER6 |= BIT((interrupt_num % 32));
	}
	else
	{
		; // do nothing if input exceeds range.
	}
}

// set nvic priority
void set_priority(uint32_t interrupt_num, uint8_t priority)
{	// TODO FIX THIS AS IT IS NOT WORKING
	priority = (priority & 0x0fU) << 4; // strip priority down to nybble.
	// and left justify.
	uint8_t ipr = (uint8_t)interrupt_num / 4;
	uint8_t shift = (interrupt_num % 4) * 8;

	switch(ipr)
	{
	    case 0:
	        NVIC->IPR0 &= ~(0xf0U << shift); // clear current priority.
	        NVIC->IPR0 |= (priority << shift); // set priority.
	        break;

	    case 1:
	        NVIC->IPR1 &= ~(0xf0U << shift); // clear current priority.
	        NVIC->IPR1 |= (priority << shift); // set priority.
	        break;

	    case 2:
	        NVIC->IPR2 &= (0xf0U << shift); // clear current priority.
	        NVIC->IPR2 |= (priority << shift); // set priority.
	        break;

	    case 3:
	        NVIC->IPR3 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR3 |= (priority << shift); // set priority.
	        break;

	    case 4:
	        NVIC->IPR4 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR4 |= (priority << shift); // set priority.
	        break;

	    case 5:
	        NVIC->IPR5 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR5 |= (priority << shift); // set priority.
	        break;

	    case 6:
	        NVIC->IPR6 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR6 |= (priority << shift); // set priority.
	        break;

	    case 7:
	        NVIC->IPR7 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR7 |= (priority << shift); // set priority.
	        break;

	    case 8:
	        NVIC->IPR8 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR8 |= (priority << shift); // set priority.
	        break;

	    case 9:
	        NVIC->IPR9 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR9 |= (priority << shift); // set priority.
	        break;

	    case 10:
	        NVIC->IPR10 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR10 |= (priority << shift); // set priority.
	        break;

	    case 11:
	        NVIC->IPR11 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR11 |= (priority << shift); // set priority.
	        break;

	    case 12:
	        NVIC->IPR12 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR12 |= (priority << shift); // set priority.
	        break;

	    case 13:
	        NVIC->IPR13 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR13 |= (priority << shift); // set priority.
	        break;

	    case 14:
	        NVIC->IPR14 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR14 |= (priority << shift); // set priority.
	        break;

	    case 15:
	        NVIC->IPR15 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR15 |= (priority << shift); // set priority.
	        break;

	    case 16:
	        NVIC->IPR16 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR16 |= (priority << shift); // set priority.
	        break;

	    case 17:
	        NVIC->IPR17 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR17 |= (priority << shift); // set priority.
	        break;

	    case 18:
	        NVIC->IPR18 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR18 |= (priority << shift); // set priority.
	        break;

	    case 19:
	        NVIC->IPR19 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR19 |= (priority << shift); // set priority.
	        break;

	    case 20:
	        NVIC->IPR20 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR20 |= (priority << shift); // set priority.
	        break;

	    case 21:
	        NVIC->IPR21 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR21 |= (priority << shift); // set priority.
	        break;

	    case 22:
	        NVIC->IPR22 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR22 |= (priority << shift); // set priority.
	        break;

	    case 23:
	        NVIC->IPR23 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR23 |= (priority << shift); // set priority.
	        break;

	    case 24:
	        NVIC->IPR24 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR24 |= (priority << shift); // set priority.
	        break;

	    case 25:
	        NVIC->IPR25 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR25 |= (priority << shift); // set priority.
	        break;

	    case 26:
	        NVIC->IPR26 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR26 |= (priority << shift); // set priority.
	        break;

	    case 27:
	        NVIC->IPR27 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR27 |= (priority << shift); // set priority.
	        break;

	    case 28:
	        NVIC->IPR28 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR28 |= (priority << shift); // set priority.
	        break;

	    case 29:
	        NVIC->IPR29 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR29 |= (priority << shift); // set priority.
	        break;

	    case 30:
	        NVIC->IPR30 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR30 |= (priority << shift); // set priority.
	        break;

	    case 31:
	        NVIC->IPR31 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR31 |= (priority << shift); // set priority.
	        break;

	    case 32:
	        NVIC->IPR32 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR32 |= (priority << shift); // set priority.
	        break;

	    case 33:
	        NVIC->IPR33 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR33 |= (priority << shift); // set priority.
	        break;

	    case 34:
	        NVIC->IPR34 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR34 |= (priority << shift); // set priority.
	        break;

	    case 35:
	        NVIC->IPR35 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR35 |= (priority << shift); // set priority.
	        break;

	    case 36:
	        NVIC->IPR36 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR36 |= (priority << shift); // set priority.
	        break;

	    case 37:
	        NVIC->IPR37 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR37 |= (priority << shift); // set priority.
	        break;

	    case 38:
	        NVIC->IPR38 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR38 |= (priority << shift); // set priority.
	        break;

	    case 39:
	        NVIC->IPR39 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR39 |= (priority << shift); // set priority.
	        break;

	    case 40:
	        NVIC->IPR40 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR40 |= (priority << shift); // set priority.
	        break;

	    case 41:
	        NVIC->IPR41 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR41 |= (priority << shift); // set priority.
	        break;

	    case 42:
	        NVIC->IPR42 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR42 |= (priority << shift); // set priority.
	        break;

	    case 43:
	        NVIC->IPR43 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR43 |= (priority << shift); // set priority.
	        break;

	    case 44:
	        NVIC->IPR44 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR44 |= (priority << shift); // set priority.
	        break;

	    case 45:
	        NVIC->IPR45 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR45 |= (priority << shift); // set priority.
	        break;

	    case 46:
	        NVIC->IPR46 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR46 |= (priority << shift); // set priority.
	        break;

	    case 47:
	        NVIC->IPR47 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR47 |= (priority << shift); // set priority.
	        break;

	    case 48:
	        NVIC->IPR48 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR48 |= (priority << shift); // set priority.
	        break;

	    case 49:
	        NVIC->IPR49 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR49 |= (priority << shift); // set priority.
	        break;

	    case 50:
	        NVIC->IPR50 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR50 |= (priority << shift); // set priority.
	        break;

	    case 51:
	        NVIC->IPR51 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR51 |= (priority << shift); // set priority.
	        break;

	    case 52:
	        NVIC->IPR52 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR52 |= (priority << shift); // set priority.
	        break;

	    case 53:
	        NVIC->IPR53 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR53 |= (priority << shift); // set priority.
	        break;

	    case 54:
	        NVIC->IPR54 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR54 |= (priority << shift); // set priority.
	        break;

	    case 55:
	        NVIC->IPR55 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR55 |= (priority << shift); // set priority.
	        break;

	    case 56:
	        NVIC->IPR56 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR56 |= (priority << shift); // set priority.
	        break;

	    case 57:
	        NVIC->IPR57 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR57 |= (priority << shift); // set priority.
	        break;

	    case 58:
	        NVIC->IPR58 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR58 |= (priority << shift); // set priority.
	        break;

	    case 59:
	        NVIC->IPR59 &= (~(0xf0U) << shift); // clear current priority.
	        NVIC->IPR59 |= (priority << shift); // set priority.
	        break;
	    default:
	    	;
	}
}
