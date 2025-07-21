#include "paging.h"
#include "frame.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

extern void enable_paging(uint32_t pd);

void paging_setup() {
	uint32_t* pd = (uint32_t*)alloc_frame();
	uint32_t* first_page_table = (uint32_t*)alloc_frame();

	for (size_t i = 0; i < ENTRIES_PER_TABLE; i++) {
		first_page_table[i] = (i * PAGE_SIZE) | PAGE_PRESENT | PAGE_RW;
	}

	pd[0] = (uint32_t)first_page_table | PAGE_PRESENT | PAGE_RW;
	pd[1023] = (uint32_t)pd | PAGE_PRESENT | PAGE_RW;
	enable_paging((uint32_t)pd);
}
