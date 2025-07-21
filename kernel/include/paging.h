#pragma once

#include <stdint.h>

extern uint32_t _kernel_start;
extern uint32_t _kernel_end;

#define ENTRIES_PER_TABLE 1024u
#define PAGE_SIZE 0x1000u

void paging_setup();
