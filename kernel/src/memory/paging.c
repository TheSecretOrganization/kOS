#include "paging.h"
#include "frame.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

extern void enable_paging(uint32_t* pd);

static uint32_t alloc_page(unsigned int flags) {
	uint32_t page = alloc_frame();
	if (page == FRAME_ALLOC_ERROR)
		return 0; // TODO: Handle this error

	return (page | (flags & 0xFFF) | PAGE_PRESENT);
}

static void flush_tlb() {
	uint32_t cr3;
	asm volatile("mov %%cr3, %0" : "=r"(cr3));
	asm volatile("mov %0, %%cr3" ::"r"(cr3));
}

void paging_setup() {
	uint32_t* pd = (uint32_t*)alloc_page(PAGE_RW);
	// pd[1023] = (uint32_t)pd;

	uint32_t* first_page_table = (uint32_t*)alloc_page(PAGE_RW);

	for (size_t i = _kernel_start; i < ENTRIES_PER_TABLE; i++) {
		first_page_table[i] = (i * PAGE_SIZE) | PAGE_PRESENT | PAGE_RW;
	}

	pd[0] = (uint32_t)first_page_table;

	enable_paging(pd);
}

void* get_physaddr(void* virtualaddr) {
	uint32_t pdindex = GET_PD_INDEX((uint32_t)virtualaddr);
	uint32_t ptindex = GET_PT_INDEX((uint32_t)virtualaddr);

	uint32_t* pd = (uint32_t*)PD_VIRTUAL_ADDR;
	if (!(pd[pdindex] & PAGE_PRESENT))
		return NULL;

	uint32_t* pt = (uint32_t*)GET_PT(pdindex);
	if (!(pt[ptindex] & PAGE_PRESENT))
		return NULL;

	return (void*)(GET_FRAME_ADDR(pt[ptindex]) + GET_PAGE_OFFSET(virtualaddr));
}

void map_page(void* physaddr, void* virtualaddr, unsigned int flags) {
	if (!IS_PAGE_ALIGNED((uint32_t)physaddr) ||
		!IS_PAGE_ALIGNED((uint32_t)virtualaddr))
		return; // TODO: Page unaligned

	uint32_t pdindex = GET_PD_INDEX((uint32_t)virtualaddr);
	uint32_t ptindex = GET_PT_INDEX((uint32_t)virtualaddr);

	uint32_t* pd = (uint32_t*)PD_VIRTUAL_ADDR;
	if (!(pd[pdindex] & PAGE_PRESENT))
		pd[pdindex] = alloc_page(flags);

	uint32_t* pt = (uint32_t*)GET_PT(pdindex);
	if (pt[ptindex] & PAGE_PRESENT)
		return; // TODO: This page is already mapped.

	pt[ptindex] = ((uint32_t)physaddr) | (flags & 0xFFF) | PAGE_PRESENT;
	flush_tlb();
}

void map_kernel_higher_half() {
	for (uint32_t offset = 0; offset < KERNEL_SIZE; offset += PAGE_SIZE) {
		void* phys = (void*)(KERNEL_PHYS_BASE + offset);
		void* virt = (void*)(KERNEL_VIRT_BASE + offset);

		map_page(phys, virt,
				 PAGE_PRESENT |
					 PAGE_RW); // use whatever flags your kernel needs
	}
}
