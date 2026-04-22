#include "debug.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "usart.h"

void addr_contents_to_str(uint32_t addr, char buff[], uint32_t buffsize)
{
	// below returns if any checks are violated
	if ((addr + 4) > MEM_ADDR_MAX) return;
	const uint32_t* ptr = (const uint32_t*)addr;

	snprintf(buff, (size_t)buffsize, "0x%08lx: 0x%08lx 0x%08lx 0x%08lx 0x%08lx\n\r",
		(uint32_t)ptr, ptr[0], ptr[1], ptr[2], ptr[3]);
}

uint32_t get_addr_contents(uint32_t addr, uint32_t* result)
{
	// below returns 1 if address is out of range.
	if (addr > MEM_ADDR_MAX) return 1;
	
	*result = *(const uint32_t*)addr; // cast integer as pointer and dereference.
	return 0;
}

void ser_memdump_range(uint32_t addr_start, uint32_t addr_stop, char buff[], uint32_t buffsize)
{
	for (const uint32_t* addr_ptr = (const uint32_t*)addr_start; (uint32_t)addr_ptr <= addr_stop; addr_ptr += 4)
	{
		addr_contents_to_str((uint32_t)addr_ptr, buff, buffsize);
		usart_transmit_bytes(USART_DEBUG, buff, 57U, '\0'); // snprintf null terminates output
	}
}

void ser_memdump_range_req(char buff[], uint32_t buffsize)
{
	uint32_t addr_start = 0, addr_stop = 0; //used to store addresses.
	int32_t status = -1;
	// requests range of memaddresses from user
	usart_transmit_bytes(USART_DEBUG, "enter address in format: 0xstart 0xend\n\r\0", 100, '\0');
	usart_read_with_echo(USART_DEBUG, buff, buffsize);
	status = sscanf(buff, "0x%08lx 0x%08lx", &addr_start, &addr_stop);
	if ((status == 2) &&
		(memrange_valid(addr_start, addr_stop)))
	{
		ser_memdump_range(addr_start, addr_stop, buff, buffsize);

	}
	else
	{
		usart_transmit_bytes(USART_DEBUG, "Incorrectly formatted command, "
				"please try again", 50, '\0');
	}

	usart_transmit_bytes(USART_DEBUG, "\n\r\0", 50, '\0');
}

// reads option passed by user and returns inter value
int32_t read_option(const char buff[])
{
	int option = -1;
	int32_t status = sscanf(buff, "%*[^0-9]%d", &option);

	if (status != 1)
	{
		return -1;
	}

	return (int32_t)option;
}
// verify memory range is valid based on macro defs
bool memrange_valid(uint32_t addr_start, uint32_t addr_stop)
{
	if ((addr_start <= MEM_ADDR_MAX) &&
		(addr_stop <= MEM_ADDR_MAX) &&
		(addr_stop >= addr_start))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
