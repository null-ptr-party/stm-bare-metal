#ifndef _TIM_H
#define _TIM_H

#include <stdint.h>

/*Timer note: timers 1:8, 12:17 do not have kernel clocks, but instead
utilize the bus clock frequency*/

// define adv_tim struct
struct adv_tim {
	volatile uint32_t CR1, CR2, SMCR, DIER, SR, EGR, CCMR1, CCMR2, CCER, CNT, PSC, ARR,
	RCR, CCR1, CCR2, CCR3, CCR4, BDTR, DCR, DMAR, CCMR3, CCR5, CCR6,
	AF1, AF2, TISEL;
};

// define register macros for advanced tims 1/8
#define ATIM1 ((struct adv_tim*)0x40010000)
#define ATIM8 ((struct adv_tim*)0x40010400)

// advances timer macros
// mode macros
#define TIMMODE_UPCOUNTER 0U // count up
#define TIMMODE_DOWNCOUNTER 1U // count down
// capture compare mode macros
#define CC_MODE_OUTPUT 0U
#define CC_MODE_INPUT_TI1 1U
#define CC_MODE_INPUT_TI2 2U
#define CC_MODE_INPUT_TRC 3U

// channel macros
#define ATIM_CH1 0U
#define ATIM_CH2 1U

// capture compare ch macros
#define TIM_CAPCOMP_CH1 0U
#define TIM_CAPCOMP_CH1COMP 1U
#define TIM_CAPCOMP_CH2 2U
#define TIM_CAPCOMP_CH2COMP 3U
#define TIM_CAPCOMP_CH3_EN 4U
#define TIM_CAPCOMP_CH3COMP 5U
#define TIM_CAPCOMP_CH4 6U
#define TIM_CAPCOMP_CH4COMP 7U
#define TIM_CAPCOMP_CH5 8U
#define TIM_CAPCOMP_CH5COMP 9U
#define TIM_CAPCOMP_CH6 10U
#define TIM_CAPCOMP_CH6COMP 11U

// polarity macro
#define TIM_POLARITY_NORMAL 0U
#define TIM_POLARITY_INV 1U

// Advanced timer functions. Macros should be used for channels
// enable atim counter
void atim_ctr_enbl(struct adv_tim* atim_ptr);
// set counter mode
void set_atim_ctmode(struct adv_tim* atim_ptr, uint8_t mode);
// set capture mode of advanced timer
void set_atim_capmode(struct adv_tim* atim_ptr, uint8_t ch, uint8_t mode);
// apply filter setting for timer
void set_atim_filter(struct adv_tim* atim_ptr, uint8_t ch, uint8_t fset);
// set capture signal prescaler
void set_atim_prescl(struct adv_tim* atim_ptr, uint8_t ch, uint8_t prscl);
// enable capture compare. Recommend using macro
void enable_atim_ch(struct adv_tim* atim_ptr, uint8_t ch);
// set timer polarity
void set_atim_polarity(struct adv_tim* atim_ptr, uint8_t ch, uint8_t polarity);

#endif