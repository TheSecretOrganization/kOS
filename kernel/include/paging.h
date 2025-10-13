#pragma once

#include <stdbool.h>
#include <stdint.h>

extern uintptr_t _kernel_start;
extern uintptr_t _kernel_end;

#define KERNEL_VIRTUAL_BASE 0xC0000000

#define ENTRIES_PER_TABLE 1024u
#define PAGE_SIZE 0x1000u

#define PAGE_PRESENT 0x1
#define PAGE_RW 0x2
#define PAGE_USER 0x4
#define PAGE_PWT 0x8
#define PAGE_PCD 0x10
#define PAGE_ACCESSED 0x20
#define PAGE_DIRTY 0x40
#define PAGE_4MB 0x80
#define PAGE_GLOBAL 0x100

typedef uint32_t pde_t;
typedef uint32_t pte_t;

static inline uintptr_t page_align_up(uintptr_t addr) {
	return ((addr + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1));
}

static inline uintptr_t page_align_down(uintptr_t addr) {
	return (addr & ~(PAGE_SIZE - 1));
}

static inline uint32_t page_offset(uintptr_t addr) {
	return addr & (PAGE_SIZE - 1);
}

static inline void invlpg(uintptr_t virt_addr) {
	asm volatile("invlpg [%0]" ::"r"(virt_addr) : "memory");
}

void paging_init();
uintptr_t get_phys_addr(uintptr_t virt_addr);
void map_page(uintptr_t virt_addr, uintptr_t phys_addr, uint32_t flags);
void unmap_page(uintptr_t virt_addr);
