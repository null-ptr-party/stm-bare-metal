#ifndef _DEBUG_H
#include <stdint.h>
#define _DEBUG_H
#define MEM_ADDR_MAX  0xDFFFFFFF

// converts contents of addr to string and writes to buffer
void addr_contents_to_str(uint32_t addr, char buff[], uint32_t buffsize);
// dumps contents of memory in range addr_start:add_stop (inclusive), converts to string and sends over serial
void ser_memdump_range(uint32_t addr_start, uint32_t addr_stop, char buff[], uint32_t buffsize);
// returns user requested memrange over serial
void ser_memdump_range_req(char buff[], uint32_t buffsize);
// returns the contents of the supplied address.
uint32_t get_addr_contents(uint32_t addr);

#endif