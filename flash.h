#ifndef FLASH_H_
#define FLASH_H_

#include <stdint.h>

struct flash {
	volatile uint32_t ACR, KEYR, CR, SR, CCR, OPTCR, OPTSR_CUR, OPTSR_PRG,
	OPTCCR, PRAR_CUR, PRAR_PRG, SCAR_CUR, SCAR_PRG, WPSN_CUR,
	WPSN_PRG, BOOT_CUR, BOOT_PRG, RSV1[2], CRCCR, CRCSADDR,
	 CRCEADDR, CRCDATAR, ECC_FAR, RSV2[3], OPTSR2_CUR, OPTSR2_PRG;
};

#define FLASH ((struct flash *)0x52002000)

#endif
