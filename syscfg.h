#ifndef _SYSCFG_H
#def _SYSCFG_H

#include <stdint.h>

// define syscfg struct
// 08/27/25. Note there seem to be some discrepancies between the syscfg register map
// and the STM32 programmer tool. Which shows does not show some of the registers
// outlined in the reference manual.

struct SYSCFG {volatile uint32_t RSVD0, PMCR, EXTICR1, EXTICR2, EXTICR3, EXTICR4,
RSVD1[2] ,CCCSR, CCVR, CCCR, RSVD1[62], PKGR, RSVD2[118], UR0, RSVD3,
UR2, UR3, UR4, UR5, UR6, UR7, RSVD[3], UR11, UR12, UR13, UR14, UR15, UR16, UR17};