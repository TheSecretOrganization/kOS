#pragma once

#include "paging.h"
#include <stdbool.h>
#include <stdint.h>

#define FREE 0x00
#define USED 0x01
#define FRAME_ALLOC_ERROR 0xFFFFFFFFu
#define START_FRAME (((uintptr_t)&_kernel_end + 0xFFF) & (~0xFFF))

#define TOTAL_FRAMES (ENTRIES_PER_TABLE * ENTRIES_PER_TABLE)
#define BITMAP_WORDS ((TOTAL_FRAMES + 31) / 32)

uint32_t alloc_frame(void);
void free_frame(uint32_t frame);
