#include "paging.h"
#include "pmm.h"
#include <stdint.h>

extern void enable_paging(uint32_t pd);

void paging_init() {
	pageframe_t pd_phys_addr = pmm_alloc_frame();
	pageframe_t first_pt_phys_addr = pmm_alloc_frame();

	uint32_t* pd = (uint32_t*)pd_phys_addr;
	uint32_t* first_pt = (uint32_t*)first_pt_phys_addr;

	uint32_t bitmap_end_addr = get_bitmap_end_addr();

	for (uint32_t addr = 0; addr < bitmap_end_addr; addr += PAGE_SIZE) {
		first_pt[addr / PAGE_SIZE] = addr | PAGE_PRESENT | PAGE_RW;
	}

	pd[0] = first_pt_phys_addr | PAGE_PRESENT | PAGE_RW;
	pd[1023] = pd_phys_addr | PAGE_PRESENT | PAGE_RW;

	enable_paging(pd_phys_addr);
}
