#include "tim.h"
#include "shared_tools.h"

// Advanced timer functions.
// Todo: some of these functions are bloaty. Once we get a feel for the timer
// we could get rid of them and put their code in a "setup function"

// enable counter
void atim_ctr_enbl(struct adv_tim* atim_ptr)
{
	atim_ptr->CR1 |= BIT(0);
}
// set counter mode
void set_atim_ctmode(struct adv_tim* atim_ptr, uint8_t mode)
{
	atim_ptr->CR1 |= (mode * BIT(4U)); // set to downcounter when modemacro = 1
}

// Macros should be used for channels and mode selection
void set_atim_capmode(struct adv_tim* atim_ptr, uint8_t ch, uint8_t mode)
{ // set atim capture mode.
	uint32_t mode_shifted = (mode << 8*ch); // mode with shift applied for ch.

	if (ch <= 1)
	{
		atim_ptr->CCMR1 = ((atim_ptr->CCMR1 & ~(mode_shifted)) | (mode_shifted));
	}
	else if (ch <= 3)
	{
		atim_ptr->CCMR2 = ((atim_ptr->CCMR2 & ~(mode_shifted)) | (mode_shifted));
	}
}

void set_atim_filter(struct adv_tim* atim_ptr, uint8_t ch, uint8_t fset)
{ // sets atim filter setting. Must use macro for channel.
	uint32_t fset_shifted = (fset << (ch*8 + 4)); // mode with shift applied for ch.

	if (ch <= 1)
	{
		atim_ptr->CCMR1 = ((atim_ptr->CCMR1 & ~(fset_shifted)) | (fset_shifted));
	}
	else if (ch <= 3)
	{
		atim_ptr->CCMR2 = ((atim_ptr->CCMR2 & ~(fset_shifted)) | (fset_shifted));
	}
}

void set_atim_prescl(struct adv_tim* atim_ptr, uint8_t ch, uint8_t prscl)
{	// set atim prescaler.
	uint32_t prscl_shifted = (prscl << (8*ch + 2)); // mode with shift applied for ch.

	if (ch <= 1)
	{
		atim_ptr->CCMR1 = ((atim_ptr->CCMR1 & ~(prscl_shifted)) | (prscl_shifted));
	}
	else if (ch <= 3)
	{
		atim_ptr->CCMR2 = ((atim_ptr->CCMR2 & ~(prscl_shifted)) | (prscl_shifted));
	}

}

void set_atim_clk_prscl(struct adv_tim* atim_ptr, uint16_t clk_prscl)
{
	atim_ptr->PSC = ((atim_ptr->PSC & ~0xFFFFU) | clk_prscl);
}

// enable capture compare and set polarity. Recommend using macro
void enable_atim_ch(struct adv_tim* atim_ptr, uint8_t ch)
{	// Note macros should be used.
	atim_ptr->CCER |= BIT((2*ch));
}

void set_atim_polarity(struct adv_tim* atim_ptr, uint8_t ch, uint8_t polarity)
{	// Note macros should be used.
	atim_ptr->CCER |= polarity*BIT((2*ch + 1));
}

void set_atim_rep_cnt(struct adv_tim* atim_ptr, uint16_t cnt)
{
	atim_ptr->RCR = ((atim_ptr->RCR & 0xFFFF0000) | cnt); // only write to bottom 16 bits.
}

uint16_t get_atim_capval(struct adv_tim* atim_ptr, uint8_t reg)
{	// gets the capture value of the requested register for the specified atim.
	// each advanced tim has 6 capture/compare registers.

	switch(reg)
	{
		case ATIM_CC_REG_1:
			return (uint16_t)atim_ptr->CCR1;
			break;

		case ATIM_CC_REG_2:
			return (uint16_t)atim_ptr->CCR2;
			break;

		case ATIM_CC_REG_3:

			return (uint16_t)atim_ptr->CCR3;
			break;

		case ATIM_CC_REG_4:
			return (uint16_t)atim_ptr->CCR4;
			break;

		case ATIM_CC_REG_5:
			return (uint16_t)atim_ptr->CCR5;
			break;

		case ATIM_CC_REG_6:
			return (uint16_t)atim_ptr->CCR6;
			break;

		default:
			return (uint16_t)atim_ptr->CCR1;
			break;
	}
	

}