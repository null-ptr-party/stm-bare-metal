#ifndef _DEBUG_H
#include <stdint.h>
#define _DEBUG_H
#define MEM_ADDR_MAX  0x0000ffff

void memdump(uint32_t addr, char buff[], uint32_t buffsize);
void memdump_range(uint32_t addr_start, uint32_t addr_stop, char buff[], uint32_t buffsize);
#endif