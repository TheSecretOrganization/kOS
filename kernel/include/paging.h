#pragma once

#include <stdint.h>

extern uint32_t _kernel_start;
extern uint32_t _kernel_end;

#define KERNEL_PHYS_BASE _kernel_start
#define KERNEL_VIRT_BASE 0xC0000000
#define KERNEL_SIZE (_kernel_end - _kernel_start)

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

#define PD_VIRTUAL_ADDR 0xFFFFF000UL
#define PT_RECURSIVE_BASE 0xFFC00000UL

#define IS_PAGE_ALIGNED(addr) (!((uint32_t)(addr) & 0xFFF))
#define GET_PD_INDEX(virtualaddr) (virtualaddr >> 22)
#define GET_PT_INDEX(virtualaddr) (virtualaddr >> 12 & 0x03FF)
#define GET_FRAME_ADDR(entry) ((entry) & 0xFFFFF000)
#define GET_PAGE_OFFSET(addr) ((unsigned long)(addr) & 0xFFF)
#define GET_PT(pdindex) (PT_RECURSIVE_BASE + (ENTRIES_PER_TABLE * pdindex))

void paging_setup();
