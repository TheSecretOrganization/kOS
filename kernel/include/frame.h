#pragma once

#include "paging.h"
#include <stdbool.h>
#include <stdint.h>

extern uintptr_t endkernel;

#define FREE 0x00
#define USED 0x01

#define FRAME_ALLOC_ERROR 0xFFFFFFFFu
#define STARTFRAME (((uintptr_t)&endkernel + 0xFFF) & (~0xFFF))

#define MEMORY_SIZE (PAGE_TABLE_ENTRIES * PAGE_DIR_ENTRIES * 1024)
#define TOTAL_PAGES (MEMORY_SIZE / PAGE_SIZE)
#define BITMAP_SIZE (TOTAL_PAGES / 32)

typedef uintptr_t page_frame_t;

page_frame_t alloc_frame(void);
void free_frame(page_frame_t frame);
