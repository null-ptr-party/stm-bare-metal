#ifndef _SYSCFG_H
#define _SYSCFG_H

#include <stdint.h>

// define syscfg struct
// 08/27/25. Note there seem to be some discrepancies between the syscfg register map
// and the STM32 programmer tool. Which shows does not show some of the registers
// outlined in the reference manual.

struct syscfg {
	volatile uint32_t RSVD0, PMCR, EXTICR1, EXTICR2, EXTICR3, EXTICR4,
	RSVD1[2], CCCSR, CCVR, CCCR, RSVD2[62], PKGR, RSVD3[118], UR0, RSVD4,
	UR2, UR3, UR4, UR5, UR6, UR7, RSVD5[3], UR11, UR12, UR13, UR14, UR15, UR16, UR17;
};

// define syscfg struct
#define SYSCFG ((struct syscfg *)0x58000400)

#define EXTI_PORT_A 0UL
#define EXTI_PORT_B 1UL
#define EXTI_PORT_C 2UL
#define EXTI_PORT_D 3UL
#define EXTI_PORT_E 4UL
#define EXTI_PORT_F 5UL
#define EXTI_PORT_G 6UL
#define EXTI_PORT_H 7UL
#define EXTI_PORT_I 8UL

void cfg_exti_interrupt(uint32_t port, uint32_t pin);

#endif