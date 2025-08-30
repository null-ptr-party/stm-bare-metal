#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>
#include "shared_tools.h"

// HSI x1 is 64MHz 1f3f value
//============datatypes===================================
// systick struct for registers
struct systick {
	volatile uint32_t CSR, RVR, CVR, CALIB;
};

// define register macro
#define SYSTICK ((struct systick *)(0xE000E010))

// systick setup struct
struct systick_setup
{
	uint32_t ctr_enbl, excpt_enbl, clksrc, rld_val;
};

//============functions===================================
#define CTR_ENABLE 1UL // Enable systick counter
#define CTR_DISABLE 0UL // disable systick counter

// enables systick counter
void set_systick_ctr(uint32_t ctr_enbl);

#define CLKSRC_EXT 0UL // Use external clock
#define CLKSRC_PRC 1UL // Use processor clock

// sets systick clocksource
void set_systick_clksrc(uint32_t clksrc);

#define EXCPT_ENBL 1UL
#define EXCPT_DSBL 0UL

// set whether systick exception is triggered
// on counter rollover
void set_systick_excpt(uint32_t excpt_enbl);

void set_rld_val(uint32_t rld_val);

// used to call the above functions
void init_systick(struct systick_setup *setup);

#endif