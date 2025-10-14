#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define HEAP_VIRTUAL_START 0xD0000000
#define HEAP_VIRTUAL_END 0xE0000000
#define BLOCK_MAGIC 0xDEADBEEF
#define BLOCK_MINIMUM_SIZE 1u

typedef struct block_header {
	size_t size;
	bool is_free;
	struct block_header* next;
	struct block_header* prev;
	uint32_t magic;
} block_header_t;

typedef struct {
	block_header_t* first;
	block_header_t* last;
	uintptr_t brk;
} heap_state_t;

void* kmalloc(size_t size);
void kfree(const void* objp);
size_t ksize(const void* objp);
