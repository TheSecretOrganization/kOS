#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct block_header {
	size_t size;
	bool is_free;
	struct block_header* next;
	struct block_header* prev;
	uint32_t magic;
} block_header_t;

#define BLOCK_MAGIC 0xDEADBEEF
#define HEADER_SIZE sizeof(block_header_t)

void* kmalloc(size_t size);
void kfree(const void* objp);
size_t ksize(const void* objp);
void* kbrk(size_t increment);
