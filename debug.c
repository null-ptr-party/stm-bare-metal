#include "debug.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "usart.h"

void addr_contents_to_str(uint32_t addr, char buff[], uint32_t buffsize)
{
	// below returns if any checks are violated
	if ((addr + 4) > MEM_ADDR_MAX) return;
	uint32_t* ptr = (uint32_t*)addr;

	snprintf(buff, (size_t)buffsize, "0x%08lx: 0x%08lx 0x%08lx 0x%08lx 0x%08lx\n\r",
		(uint32_t)ptr, ptr[0], ptr[1], ptr[2], ptr[3]);
}

uint32_t get_addr_contents(uint32_t addr, uint32_t* result)
{
	// below returns 1 if address is out of range.
	if (addr > MEM_ADDR_MAX) return 1;
	
	*result = *(uint32_t*)addr; // cast integer as pointer and dereference.
	return 0;
}

void ser_memdump_range(uint32_t addr_start, uint32_t addr_stop, char buff[], uint32_t buffsize)
{
	for (uint32_t* addr_ptr = (uint32_t*)addr_start; (uint32_t)addr_ptr <= addr_stop; addr_ptr += 4)
	{
		addr_contents_to_str((uint32_t)addr_ptr, buff, buffsize);
		usart_transmit_bytes(USART_DEBUG, buff, 57U, '\0'); // snprintf null terminates output
	}
}

void ser_memdump_range_req(char buff[], uint32_t buffsize)
{
	uint32_t addr_start = 0, addr_stop = 0; //used to store addresses.
	// requests range of memaddresses from user
	usart_transmit_bytes(USART_DEBUG, "enter address in format: 0xstart 0xend\n\r\0", 100, '\0');
	usart_read_with_echo(USART_DEBUG, buff, buffsize);
	sscanf(buff, "0x%08lx 0x%08lx", &addr_start, &addr_stop);
	ser_memdump_range(addr_start, addr_stop, buff, buffsize);
	usart_transmit_bytes(USART_DEBUG, "\n\r\0", 50, '\0');
}