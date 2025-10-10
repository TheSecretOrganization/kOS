#pragma once

#include <stdint.h>

#define PRE_ALLOC_SIZE 20

typedef struct {
	uint8_t* addr;
	uint32_t size_in_bytes;
	uint32_t total_pages;
	uint32_t next_free_bit;
} frame_map_t;

typedef uint32_t pageframe_t;

uint32_t get_bitmap_end_addr();
void pmm_init(uint64_t total_mem);
pageframe_t pmm_alloc_frame(void);
void pmm_free_frame(pageframe_t frame);
