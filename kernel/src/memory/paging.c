#include "paging.h"
#include "pmm.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

extern void enable_paging(uintptr_t pd_phys);

static pde_t* page_directory = NULL;

static inline uint32_t pd_index(uintptr_t virt_addr) {
	return (virt_addr >> 22) & 0x3FF;
}

static inline uint32_t pt_index(uintptr_t virt_addr) {
	return (virt_addr >> 12) & 0x3FF;
}

static inline pte_t* pt_virt_for_index(uint32_t pd_idx) {
	return (pte_t*)(RECURSIVE_PT_BASE + (pd_idx << 12));
}

static inline bool recursive_active() {
	return (uintptr_t)page_directory == (uintptr_t)RECURSIVE_PD_ADDR;
}

static uintptr_t get_page_table_phys(uintptr_t virt_addr, bool create) {
	uint32_t pd_idx = pd_index(virt_addr);

	uintptr_t pd_entry = (uintptr_t)page_directory[pd_idx];
	if (pd_entry & PAGE_PRESENT) {
		return page_align_down(pd_entry);
	}

	if (!create) {
		return 0;
	}

	uintptr_t pt_phys = pmm_alloc_frame();
	page_directory[pd_idx] = pt_phys | PAGE_PRESENT | PAGE_RW;

	return pt_phys;
}

void map_page(uintptr_t virt_addr, uintptr_t phys_addr, uint32_t flags) {
	virt_addr = page_align_down(virt_addr);
	phys_addr = page_align_down(phys_addr);

	uintptr_t pt_phys = get_page_table_phys(virt_addr, true);
	uint32_t pd_idx = pd_index(virt_addr);
	uint32_t pt_idx = pt_index(virt_addr);

	pte_t* pt;
	if (recursive_active()) {
		pt = pt_virt_for_index(pd_idx);
	} else {
		pt = (pte_t*)pt_phys;
	}

	pt[pt_idx] = phys_addr | flags;
	invlpg(virt_addr);
}

void unmap_page(uintptr_t virt_addr) {
	virt_addr = page_align_down(virt_addr);

	uint32_t pd_idx = pd_index(virt_addr);
	uint32_t pt_idx = pt_index(virt_addr);

	if (!(page_directory[pd_idx] & PAGE_PRESENT))
		return;

	pte_t* pt;
	if (recursive_active()) {
		pt = pt_virt_for_index(pd_idx);
	} else {
		uintptr_t pt_phys = page_align_down(page_directory[pd_idx]);
		pt = (pte_t*)pt_phys;
	}

	pt[pt_idx] = 0;
	invlpg(virt_addr);
}

void paging_init(void) {
	uintptr_t pd_phys = pmm_alloc_frame();
	page_directory = (pde_t*)pd_phys;
	page_directory[RECURSIVE_INDEX] = pd_phys | PAGE_PRESENT | PAGE_RW;

	uint32_t bitmap_end_addr = get_bitmap_end_addr();
	for (uintptr_t addr = 0; addr < bitmap_end_addr; addr += PAGE_SIZE) {
		map_page(addr, addr, PAGE_PRESENT | PAGE_RW);
	}

	enable_paging(pd_phys);
	page_directory = RECURSIVE_PD_ADDR;
}

uintptr_t get_phys_addr(uintptr_t virt_addr) {
	uint32_t pd_idx = pd_index(virt_addr);
	uint32_t pt_idx = pt_index(virt_addr);

	if (!(page_directory[pd_idx] & PAGE_PRESENT)) {
		return 0;
	}

	pte_t* pt;
	if (recursive_active()) {
		pt = pt_virt_for_index(pd_idx);
	} else {
		uintptr_t pt_phys = page_align_down(page_directory[pd_idx]);
		pt = (pte_t*)pt_phys;
	}

	if (!(pt[pt_idx] & PAGE_PRESENT)) {
		return 0;
	}

	uintptr_t phys_page = page_align_down(pt[pt_idx]);
	uintptr_t offset = page_offset(virt_addr);

	return phys_page + offset;
}
