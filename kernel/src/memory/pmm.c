#include "pmm.h"
#include "kpanic.h"
#include "paging.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

extern uint32_t _kernel_start;
extern uint32_t _kernel_end;

static frame_map_t map = {NULL, 0, 0, 0};
static pageframe_t pre_frames[PRE_ALLOC_SIZE];

static inline uint32_t page_align_up(uint32_t x) {
	return (x + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
}

static inline void set_bit(uint32_t bit) {
	map.addr[bit / 8] |= (1 << (bit % 8));
}

static inline void clear_bit(uint32_t bit) {
	map.addr[bit / 8] &= ~(1 << (bit % 8));
}

static inline bool test_bit(uint32_t bit) {
	return (map.addr[bit / 8] >> (bit % 8)) & 1;
}

static uint32_t find_next_free() {
	for (uint32_t i = map.next_free_bit; i < map.total_pages; i++) {
		if (!test_bit(i)) {
			return i;
		}
	}

	return map.total_pages;
}

inline uint32_t get_bitmap_end_addr() {
	return (uint32_t)map.addr + map.size_in_bytes;
}

void pmm_init(uint64_t total_mem) {
	map.addr = (uint8_t*)page_align_up((uint32_t)&_kernel_end);
	map.total_pages = total_mem / PAGE_SIZE;
	map.size_in_bytes = (map.total_pages + 7) / 8; // Ceiling division
	map.size_in_bytes = page_align_up(map.size_in_bytes);

	for (uint32_t i = 0; i < map.size_in_bytes; i++) {
		map.addr[i] = 0xFF;
	}

	uint32_t bitmap_end_addr = get_bitmap_end_addr();
	uint32_t bitmap_end_page = (bitmap_end_addr + PAGE_SIZE - 1) / PAGE_SIZE;

	for (uint32_t i = bitmap_end_page; i < map.total_pages; i++) {
		clear_bit(i);
	}

	map.next_free_bit = find_next_free();
}

static pageframe_t intern_alloc() {
	if (map.next_free_bit >= map.total_pages) {
		KPANIC("frame alloc error")
	}

	pageframe_t frame = map.next_free_bit * PAGE_SIZE;
	set_bit(map.next_free_bit);
	map.next_free_bit = find_next_free();

	memset((void*)frame, 0, PAGE_SIZE);

	return frame;
}

pageframe_t pmm_alloc_frame() {
	static bool allocate = true;
	static uint8_t pframe = 0;

	if (pframe == PRE_ALLOC_SIZE) {
		allocate = true;
	}

	if (allocate) {
		for (uint8_t i = 0; i < PRE_ALLOC_SIZE; i++) {
			pre_frames[i] = intern_alloc();
		}

		pframe = 0;
		allocate = false;
	}

	pageframe_t ret = pre_frames[pframe];
	++pframe;
	return ret;
}

void pmm_free_frame(pageframe_t frame) {
	uint32_t bit = frame / PAGE_SIZE;

	if (!test_bit(bit)) {
		return;
	}

	clear_bit(bit);

	if (bit < map.next_free_bit) {
		map.next_free_bit = bit;
	}
}
