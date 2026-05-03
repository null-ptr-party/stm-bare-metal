#include <stdint.h>
#include <stdbool.h>
#include "flash.h"
#include "shared_tools.h"

// Set number of wait states
void set_wait_states(uint8_t num_states)
{
	uint32_t word = num_states & MASK(3); // ensure correct range
	FLASH->ACR = (FLASH->ACR & ~MASK(3)) | word;
}
// set write frequency
void set_prog_delay(uint8_t delay)
{
	uint32_t word = ((delay & MASK(1)) << 4U);
	FLASH->ACR = ((FLASH->ACR & ~(MASK(1) << 4U)) | word);
}


// macros to set protection
#define ENABLE_WRITE_PROTECTION true
#define DISABLE_WRITE_PROTECTION false
// Set write protection of sector. 1 is write protected. 0 is not.
void set_sector_prot(uint8_t sector, bool prot_status)
{	// valid sectors are 0-7
	if (sector > 7) return; // verify sector selection

	FLASH->WPSN_PRG = ((FLASH->WPSN_PRG & ~BIT(sector)) | ((uint32_t)prot_status * BIT(sector)));

}

// get write protection status of a sector
int32_t get_sector_prot(uint8_t sector)
{
	// valid sectors are 0-7
	if (sector > 7) return BAD_SECTOR_SELECTION;

	if ((FLASH->WPSN_CUR & BIT(sector)) != 0)
	{
		return NOT_WRITE_PROTECTED;
	}
	else
	{
		return WRITE_PROTECTED;
	}
}

 // unlock flash control register.
 // note that this requires a write sequence to FLASH_KEYR
void unlock_flash_cr()
{
	 // Need to add write sequence to KEYR register
	 // before zeroing configuration lock bit.
	 FLASH->CR = (FLASH->CR & ~BIT(0));
}

 // lock flash control register
void lock_flash_cr()
{
	FLASH->CR |= BIT(0);
}

// check wait queue
bool get_flash_queue_status()
{
	if ((FLASH->SR & BIT(2)) == 0)
	{
		return OPERATION_IN_QUEUE;
	}
	else
	{
		return NO_OPERATION_IN_QUEUE;
	}
}

// write to flash
int32_t write_flash(uint32_t address, const uint8_t buff[32])
{
	// ensure address is aligned on 32 byte address
	if ((address % 32) != 0) return -1;
	// check address range. Since we write 32 bytes at a time,
	// we must ensure that all 32 bytes are within addressable range
	if ((address + 31) > FLASH_ADDRESS_MAX) return -1;
	if (address < FLASH_ADDRESS_MIN) return -1;

	uint8_t *addr_ptr = (uint8_t *)address;

	for (uint8_t byte_idx = 0; byte_idx < 32; byte_idx++, addr_ptr++)
	{
		*addr_ptr = buff[byte_idx];
	}

	while(get_flash_queue_status() == OPERATION_IN_QUEUE)
	{
		; // do nothing until operation completes
	}
	return 0;
}
