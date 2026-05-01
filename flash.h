#ifndef FLASH_H_
#define FLASH_H_

#include <stdint.h>

struct flash {
	volatile uint32_t ACR, KEYR, CR, SR, CCR, OPTCR, OPTSR_CUR, OPTSR_PRG,
	OPTCCR, PRAR_CUR, PRAR_PRG, SCAR_CUR, SCAR_PRG, WPSN_CUR,
	WPSN_PRG, BOOT_CUR, BOOT_PRG, RSV1[2], CRCCR, CRCSADDR,
	 CRCEADDR, CRCDATAR, ECC_FAR, RSV2[3], OPTSR2_CUR, OPTSR2_PRG;
};

// Set number of wait states
void set_wait_states(uint8_t num_states);
// set write frequency
void set_prog_delay(uint8_t delay);



#define FLASH ((struct flash *)0x52002000)

#endif
