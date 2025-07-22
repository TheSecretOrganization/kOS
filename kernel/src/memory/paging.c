#include "paging.h"
#include "frame.h"
#include "gdt.h"
#include "tty.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

extern void enable_paging(uint32_t pd);
extern void flush_tlb();
extern void hh_reload();

static void map_page(void* phys_addr, void* virt_addr, unsigned int flags) {
	if (!IS_PAGE_ALIGNED((uint32_t)phys_addr) ||
		!IS_PAGE_ALIGNED((uint32_t)virt_addr))
		return; // TODO: Page unaligned

	uint32_t pdindex = GET_PD_INDEX((uint32_t)virt_addr);
	uint32_t ptindex = GET_PT_INDEX((uint32_t)virt_addr);

	uint32_t* pd = (uint32_t*)PD_VIRTUAL_ADDR;
	if (!(pd[pdindex] & PAGE_PRESENT))
		pd[pdindex] = alloc_frame() | PAGE_PRESENT | PAGE_RW;

	uint32_t* pt = (uint32_t*)GET_PT(pdindex);
	pt[ptindex] = ((uint32_t)phys_addr) | (flags & 0xFFF) | PAGE_PRESENT;
}

static void map_kernel_higher_half() {
	uint32_t size = _kernel_end - KERNEL_PHYS_BASE;
	uint32_t pages = (size + 0xFFF) / PAGE_SIZE;

	for (uint32_t i = 0; i < pages; i++) {
		uint32_t offset = i * PAGE_SIZE;
		void* phys_addr = (void*)(KERNEL_PHYS_BASE + offset);
		void* virt_addr = (void*)(KERNEL_VIRT_BASE + offset);
		map_page(phys_addr, virt_addr, PAGE_RW);
	}

	// map_page((void*)GDT_ADDRESS, (void*)(GDT_ADDRESS +
	// KERNEL_VIRT_BASE),PAGE_RW);
	map_page((void*)VGA_MEMORY_BASE,
			 (void*)(VGA_MEMORY_BASE + KERNEL_VIRT_BASE), PAGE_RW);

	flush_tlb();
}

void paging_setup() {
	uint32_t* pd = (uint32_t*)alloc_frame();
	pd[1023] = (uint32_t)pd | PAGE_PRESENT | PAGE_RW;

	uint32_t* identity_map = (uint32_t*)alloc_frame();

	for (size_t i = 0; i < ENTRIES_PER_TABLE; i++) {
		identity_map[i] = (i * PAGE_SIZE) | PAGE_PRESENT | PAGE_RW;
	}

	pd[0] = (uint32_t)identity_map | PAGE_PRESENT | PAGE_RW;
	pd[GET_PD_INDEX(KERNEL_VIRT_BASE)] =
		(uint32_t)identity_map | PAGE_PRESENT | PAGE_RW;

	enable_paging((uint32_t)pd);
	map_kernel_higher_half();
	hh_reload();
}

void* get_phys_addr(void* virt_addr) {
	uint32_t pdindex = GET_PD_INDEX((uint32_t)virt_addr);
	uint32_t ptindex = GET_PT_INDEX((uint32_t)virt_addr);

	uint32_t* pd = (uint32_t*)PD_VIRTUAL_ADDR;
	if (!(pd[pdindex] & PAGE_PRESENT))
		return NULL;

	uint32_t* pt = (uint32_t*)GET_PT(pdindex);
	if (!(pt[ptindex] & PAGE_PRESENT))
		return NULL;

	return (void*)(GET_FRAME_ADDR(pt[ptindex]) + GET_PAGE_OFFSET(virt_addr));
}
