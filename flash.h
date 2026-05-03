#ifndef FLASH_H_
#define FLASH_H_

#include <stdint.h>
#include <stdbool.h>

struct flash {
	volatile uint32_t ACR, KEYR, OPTKEYR, CR, SR, CCR, OPTCR, OPTSR_CUR, OPTSR_PRG,
	OPTCCR, PRAR_CUR, PRAR_PRG, SCAR_CUR, SCAR_PRG, WPSN_CUR,
	WPSN_PRG, BOOT_CUR, BOOT_PRG, RSV1[2], CRCCR, CRCSADDR,
	 CRCEADDR, CRCDATAR, ECC_FAR, RSV2[3], OPTSR2_CUR, OPTSR2_PRG;
};

#define FLASH ((struct flash *)0x52002000)

// address macros
#define FLASH_ADDRESS_MAX 0x080FFFFF
#define FLASH_ADDRESS_MIN 0x08000000

// Set number of wait states
void set_wait_states(uint8_t num_states);
// set write frequency
void set_prog_delay(uint8_t delay);

// macros for write protection
#define WRITE_PROTECTED 1
#define NOT_WRITE_PROTECTED 0
#define BAD_SECTOR_SELECTION -1

// Set write protection of sector. 1 is write protected. 0 is not.
void set_sector_prot(uint8_t sector, bool prot_status);

// get write protection status of a sector
int32_t get_sector_prot(uint8_t sector);

// unlock flash control register.
// note that this requires a write sequence to FLASH_KEYR
void unlock_flash_cr();

// lock flash control register
void lock_flash_cr();

#define FLASH_KEY_ONE 0x45670123U
#define FLASH_KEY_TWO 0xCDEF89ABU

// write keys to unlock CR register
void write_keys();

// enable flash buffer
void enable_flash_buffer();

#define OPERATION_IN_QUEUE true
#define NO_OPERATION_IN_QUEUE false
// check wait queue
bool get_flash_queue_status();

// flash write macros
#define WRITE_FAILURE -1
#define WRITE_SUCCESS 0

// write to flash
int32_t write_flash(uint32_t address, const uint8_t buff[32]);

#endif
