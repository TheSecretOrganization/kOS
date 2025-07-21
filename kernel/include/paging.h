#pragma once

#include <stdint.h>

extern uint32_t _kernel_start;
extern uint32_t _kernel_end;

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

void paging_setup();
