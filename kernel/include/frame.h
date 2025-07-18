#pragma once

#include "paging.h"
#include <stdbool.h>
#include <stdint.h>

#define FREE 0x00
#define USED 0x01
#define FRAME_ALLOC_ERROR 0xFFFFFFFFu
#define START_FRAME (((uintptr_t) & _kernel_end + 0xFFF) & (~0xFFF))

#define TOTAL_FRAMES (MEMORY_SIZE / PAGE_SIZE)
#define BITMAP_SIZE (TOTAL_FRAMES / 8)

uint32_t alloc_frame(void);
void free_frame(uint32_t frame);
