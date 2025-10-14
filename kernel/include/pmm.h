#pragma once

#include <stdint.h>

#define PRE_ALLOC_SIZE 20

typedef struct {
	uint8_t* addr;
	uint32_t size_in_bytes;
	uint32_t total_pages;
	uint32_t next_free_bit;
} frame_map_t;

uintptr_t get_bitmap_end_addr();
void pmm_init(uint64_t total_mem);
uintptr_t pmm_alloc_frame(void);
void pmm_free_frame(uintptr_t frame);
