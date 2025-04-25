#include "kalloc.h"
#include <stdbool.h>
#include <stdint.h>

pageframe_t frame_map[BITMAP_SIZE] = {0};
pageframe_t pre_frames[PRE_FRAME_COUNT] = {0};

static pageframe_t get_next_frame_addr() {
	uint32_t i = 0;
	while (frame_map[i] != FREE) {
		i++;
		if (i == BITMAP_SIZE)
			return FRAME_ALLOC_ERROR;
	}
	frame_map[i] = USED;
	return (STARTFRAME + (i * 0x1000));
}

static void clear_pre_frames() {
	for (uint32_t i = 0; i < PRE_FRAME_COUNT; i++)
		pre_frames[i] = 0;
}

pageframe_t kalloc_frame() {
	static bool allocate = true;
	static uint8_t pframe = 0;
	pageframe_t ret;

	if (pframe == PRE_FRAME_COUNT)
		allocate = true;

	if (allocate) {
		pframe = 0;
		allocate = false;

		for (uint32_t i = 0; i < PRE_FRAME_COUNT; i++) {
			pre_frames[i] = get_next_frame_addr();

			if (pre_frames[i] == FRAME_ALLOC_ERROR) {
				allocate = true;
				clear_pre_frames();

				return FRAME_ALLOC_ERROR;
			}
		}
	}

	ret = pre_frames[pframe];
	pframe++;
	return ret;
}

void kfree_frame(pageframe_t frame) {
	uint32_t offset = frame - STARTFRAME;
	uint32_t index = offset == 0 ? offset : offset / PAGE_SIZE;
	frame_map[index] = FREE;
}
