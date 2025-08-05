#include "debug.h"
#include <stdint.h>
#include <stdio.h>
#include "usart.h"

void memdump(uint32_t addr, char buff[], uint32_t buffsize)
{
	// dumps contents of memory in range addr_start:add_stop (inclusive)
	// psuedocode
	// validate addresses provided
	// Note if buffsize is not big enough, it will just truncate.

	// below returns if any checks are violated
	if ((addr + 4) > MEM_ADDR_MAX) return;
	uint32_t* ptr = (uint32_t*)addr;

	snprintf(buff, (size_t)buffsize, "0x%08lx: 0x%08lx 0x%08lx 0x%08lx 0x%08lx\n\r",
		(uint32_t)ptr, ptr[0], ptr[1], ptr[2], ptr[3]);
}


void memdump_range(uint32_t addr_start, uint32_t addr_stop, char buff[], uint32_t buffsize)
{
	for (uint32_t* addr_ptr = (uint32_t*)addr_start; (uint32_t)addr_ptr <= addr_stop; addr_ptr += 4)
	{
		memdump((uint32_t)addr_ptr, buff, buffsize);
		usart_transmit_bytes(USART_DEBUG, buff, 57);
	}
}