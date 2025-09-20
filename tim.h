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
#define TIM1 ((struct adv_tim*)0x40010000)
#define TIM8 ((struct adv_tim*)0x40010400)

// advances timer macros
// mode macros
#define TIMMODE_UPCOUNTER 0U // count up
#define TIMMODE_DOWNCOUNTER 1U // count down
// capture compare mode macros
// CC1 defines
#define CC1_MODE_OUTPUT 0U
#define CC1_MODE_INPUT_TI1 1U
#define CC1_MODE_INPUT_TI2 2U
#define CC1_MODE_INPUT_TRC 3U
// CC2 defines
#define CC2_MODE_OUTPUT 0U
#define CC2_MODE_INPUT_TI2 1U
#define CC2_MODE_INPUT_TI1 2U
#define CC2_MODE_INPUT_TRC 3U
// CC3 defines
#define CC3_MODE_OUTPUT 0U
#define CC3_MODE_INPUT_TI3 1U
#define CC3_MODE_INPUT_TI4 2U
#define CC3_MODE_INPUT_TRC 3U
// CC4 defines
#define CC4_MODE_OUTPUT 0U
#define CC4_MODE_INPUT_TI4 1U
#define CC4_MODE_INPUT_TI3 2U
#define CC4_MODE_INPUT_TRC 3U

// channel macros
#define ATIM_CH1 0U
#define ATIM_CH2 1U

// capture compare ch macros
#define ATIM_CC1P 0U
#define ATIM_CC1N 1U
#define ATIM_CC2P 2U
#define ATIM_CC2N 3U
#define ATIM_CC3P 4U
#define ATIM_CC3N 5U
#define ATIM_CC4P 6U
#define ATIM_CC4N 7U
#define ATIM_CC5P 8U
#define ATIM_CC5N 9U
#define ATIM_CC6P 10U
#define ATIM_CC6N 11U

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