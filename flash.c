#include <stdint.h>
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

