#pragma once

#include <stdbool.h>
#include <stdint.h>

extern uintptr_t endkernel;

#define FREE 0x00
#define USED 0x01

#define FRAME_ALLOC_ERROR 0xFFFFFFFFu
#define STARTFRAME (((uintptr_t) &endkernel + 0xFFF) & (~0xFFF))

#define PAGE_SIZE 0x1000u
#define MEMORY_SIZE (1024 * 1024 * 1024)
#define TOTAL_PAGES (MEMORY_SIZE / PAGE_SIZE)
#define BITMAP_SIZE (TOTAL_PAGES / 32)

typedef uintptr_t pageframe_t;

pageframe_t alloc_frame(void);
void free_frame(pageframe_t frame);
