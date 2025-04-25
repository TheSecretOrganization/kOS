#include "kalloc.h"
#include <stdint.h>

pageframe_t frame_map[BITMAP_SIZE] = {0};
pageframe_t pre_frames[20] = {0};

static pageframe_t kalloc_frame_int() {
	uint32_t i = 0;
	while (frame_map[i] != FREE) {
		i++;
		if (i == BITMAP_SIZE)
			return ERROR;
	}
	frame_map[i] = USED;
	return (STARTFRAME + (i * 0x1000));
}

pageframe_t kalloc_frame() {
	static bool allocate = true;
	static uint8_t pframe = 0;
	pageframe_t ret;

	if (pframe == 20)
		allocate = true;

	if (allocate) {
		allocate = false;
		pframe = 0;
		for (int i = 0; i < 20; i++)
			pre_frames[i] = kalloc_frame_int();
	}

	ret = pre_frames[pframe];
	pframe++;
	return ret;
}

void kfree_frame(pageframe_t frame) {
	uint32_t offset = frame - STARTFRAME;
	uint32_t index = offset == 0 ? offset : offset / 0x1000;
	frame_map[index] = FREE;
}
