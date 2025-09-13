#ifndef _TIM_H
#define _TIM_H

#include <stdint.h>

// define adv_tim struct
struct adv_tim {
	volatile uint32_t CR1, CR2, SMCR, DIER, SR, EGR, CCMR1, CCMR2, CCER, CNT, PSC, ARR,
	RCR, CCR1, CCR2, CCR3, CCR4, BDTR, DCR, DMAR, CCMR3, CCR5, CCR6,
	AF1, AF2, TISEL;
};

// define register macros for advanced tims 1/8
#define TIM1 ((struct adv_tim*)0x40010000)
#define TIM2 ((struct adv_tim*)0x40010400)

#endif