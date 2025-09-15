#include "frame.h"
#include "kpanic.h"
#include "paging.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

static uint32_t frame_bitmap[BITMAP_WORDS] = {0};

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
	for (uint32_t i = 0; i < BITMAP_WORDS; i++) {
		if (frame_bitmap[i] != 0xFFFFFFFF) {
			uint32_t bit = __builtin_ctz(~frame_bitmap[i]);
			if (bit < 32)
				return i * 32 + bit;
		}
	}
	KPANIC("Frame alloc error")
}

uint32_t alloc_frame(void) {
	uint32_t frame_num = find_free_frame();
	set_frame_used(frame_num);

	uint32_t frame = START_FRAME + (frame_num * PAGE_SIZE);
	memset((void*)frame, 0, PAGE_SIZE);

	return frame;
}

void free_frame(uint32_t frame) {
	if (frame < START_FRAME)
		return;

	uint32_t frame_num = (frame - START_FRAME) / PAGE_SIZE;
	if (frame_num >= TOTAL_FRAMES)
		return;

	memset((void*)frame, 0, PAGE_SIZE);
	set_frame_free(frame_num);
}
