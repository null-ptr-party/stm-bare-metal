#include "exti.h"
#include "shared_tools.h"

void cfg_interrupt(uint8_t line, uint8_t edgetype)
{
	/* This function configures the exti interrupt, by
	unmasking it, and setting the desired edge trigger type
	*/

	// unmask exti
	if (line <= 31)
	{
		EXTI->CPUIMR1 |= BIT(line);

		switch (edgetype)
		{	// configure exti interrupt to desired edgetype.
			case EDGETYPE_BOTH:
				EXTI->RTSR1 |= BIT(line);
				EXTI->FTSR1 |= BIT(line);
				break;
			
			case EDGETYPE_RISING:
				EXTI->RTSR1 |= BIT(line);
				break;

			case EDGETYPE_FALLING:
				EXTI->FTSR1 |= BIT(line);
				break;

			default:
				;
		}
	}
	else if (line <= 63)
	{
		EXTI->CPUIMR2 |= BIT((line + 32U));

		switch (edgetype)
		{
			case EDGETYPE_BOTH:
				EXTI->RTSR2 |= BIT((line + 32U));
				EXTI->FTSR2 |= BIT((line + 32U));
				break;

			case EDGETYPE_RISING:
				EXTI->RTSR2 |= BIT((line + 32U));
				break;

			case EDGETYPE_FALLING:
				EXTI->FTSR2 |= BIT((line + 32U));
				break;

			default:
				;
		}
	}
	else if (line <= 91)
	{
		EXTI->CPUIMR3 |= BIT((line + 64U));

		switch (edgetype)
		{
			case EDGETYPE_BOTH:
				EXTI->RTSR3 |= BIT((line + 64U));
				EXTI->FTSR3 |= BIT((line + 64U));
				break;

			case EDGETYPE_RISING:
				EXTI->RTSR3 |= BIT((line + 64U));
				break;

			case EDGETYPE_FALLING:
				EXTI->FTSR3 |= BIT((line + 64U));
				break;

			default:
				;
		}
	}
}

