#ifndef _PWR_H
#define _PWR_H
#include <stdint.h>

struct pwr
{
	volatile uint32_t PWR_CR1, PWR_CSR1, PWR_CR2, PWR_CR3,
		PWR_CPUCR, RSVD0, PWR_D3CR, RSVD1, PWR_WKUPCR, PWR_WKUPFR, PWR_WKUPEPR;
};

#define PWR ((struct pwr*) 0x58024800)

#define VOS_SCALE_0 0U
#define VOS_SCALE_1 3U
#define VOS_SCALE_2 2U
#define VOS_SCALE_3 1U

// used for accessing vos without bit clear
#define VOS_MASK 0xC000UL 
// for accesing power registers
#define PWR_CFG_MASK 0x03UL

// this function is used to set core
// scaling. This is requried when increasing
// preformance (increasing freqency with PLL.
void set_core_scl(uint8_t scale);

// block until VOS is ready
void blk_til_vos_rdy(void);

// used to set the VDO config
void cfg_pwr_input(void);

// blocks until actvos reg is read (1);
void blk_til_actvos_rdy(void);

#endif
