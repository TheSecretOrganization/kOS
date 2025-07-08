#include "kalloc.h"
#include <stdbool.h>
#include <stdint.h>

static uint32_t frame_bitmap[BITMAP_SIZE] = {0};

static inline void set_frame_used(uint32_t frame_number) {
	uint32_t index = frame_number / 32;
	uint32_t bit = frame_number % 32;
	frame_bitmap[index] |= (1 << bit);
}

static inline void set_frame_free(uint32_t frame_number) {
	uint32_t index = frame_number / 32;
	uint32_t bit = frame_number % 32;
	frame_bitmap[index] &= ~(1 << bit);
}

static uint32_t find_free_frame(void) {
	for (uint32_t i = 0; i < BITMAP_SIZE; i++) {
		if (frame_bitmap[i] != 0xFFFFFFFF) {
			for (uint32_t bit = 0; bit < 32; bit++) {
				if (!(frame_bitmap[i] & (1 << bit))) {
					return i * 32 + bit;
				}
			}
		}
	}
	return FRAME_ALLOC_ERROR;
}

pageframe_t kalloc_frame(void) {
	uint32_t frame_num = find_free_frame();
	if (frame_num == FRAME_ALLOC_ERROR)
		return FRAME_ALLOC_ERROR;

	set_frame_used(frame_num);
	return STARTFRAME + (frame_num * PAGE_SIZE);
}

void kfree_frame(pageframe_t frame) {
	if (frame < STARTFRAME)
		return;

	uint32_t frame_num = (frame - STARTFRAME) / PAGE_SIZE;
	if (frame_num >= TOTAL_PAGES)
		return;

	set_frame_free(frame_num);
}
