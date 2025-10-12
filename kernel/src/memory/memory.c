#include "memory.h"
#include "kpanic.h"
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

uint64_t detect_memory(const multiboot_info_t* mbd, uint32_t magic) {
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		KPANIC("invalid magic number")
	}

	if (!(mbd->flags >> 6 & 0x1)) {
		KPANIC("invalid memory map given by GRUB bootloader")
	}

	uint64_t total_mem = 0;

	for (size_t i = 0; i < mbd->mmap_length;
		 i += sizeof(multiboot_memory_map_t)) {
		const multiboot_memory_map_t* mmmt =
			(multiboot_memory_map_t*)(mbd->mmap_addr + i);

		uint64_t len = ((uint64_t)mmmt->len_high << 32) | mmmt->len_low;

		if (mmmt->type == MULTIBOOT_MEMORY_AVAILABLE) {
			total_mem += len;
		}
	}

	if (total_mem > (uint64_t)UINT32_MAX + 1) {
		total_mem = (uint64_t)UINT32_MAX + 1;
	}

	return total_mem;
}
