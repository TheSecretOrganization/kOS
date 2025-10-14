#include "kheap.h"
#include "paging.h"
#include "pmm.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static heap_state_t heap = {NULL, NULL, HEAP_VIRTUAL_START};

static block_header_t* block_header_init(block_header_t* block,
										 block_header_t* next,
										 block_header_t* prev, size_t size) {
	if (!block) {
		return NULL;
	}

	block->magic = BLOCK_MAGIC;
	block->is_free = true;
	block->next = next;
	block->prev = prev;
	block->size = size;

	return block;
}

size_t ksize(const void* objp) {
	if (!objp) {
		return 0;
	}

	const block_header_t* header = (const block_header_t*)objp - 1;

	if (header->magic != BLOCK_MAGIC) {
		return 0;
	}

	return header->size;
}

static block_header_t* kbrk(size_t increment) {
	if (increment == 0) {
		return (void*)heap.brk;
	}

	size_t pages_needed = (increment + PAGE_SIZE - 1) / PAGE_SIZE;
	size_t actual_size = pages_needed * PAGE_SIZE;

	if (heap.brk + actual_size >= HEAP_VIRTUAL_END) {
		return NULL;
	}

	for (size_t i = 0; i < pages_needed; i++) {
		uintptr_t phys_frame = pmm_alloc_frame();
		uintptr_t virt_addr = heap.brk + (i * PAGE_SIZE);
		map_page(virt_addr, phys_frame, PAGE_PRESENT | PAGE_RW);
	}

	size_t data_size = actual_size - sizeof(block_header_t);
	block_header_t* new_block = (block_header_t*)heap.brk;
	block_header_init(new_block, NULL, heap.last, data_size);

	if (!heap.first) {
		heap.first = new_block;
	}

	if (heap.last) {
		heap.last->next = new_block;
	}

	heap.last = new_block;
	heap.brk += actual_size;

	return new_block;
}

static block_header_t* try_split_block(block_header_t* block, size_t new_size) {
	if (!block || !new_size) {
		return NULL;
	}

	size_t min_size = new_size + sizeof(block_header_t) + BLOCK_MINIMUM_SIZE;
	if (block->size < min_size) {
		return NULL;
	}

	size_t b1_total_size = new_size + sizeof(block_header_t);
	size_t b2_total_size = block->size + sizeof(block_header_t) - b1_total_size;

	block_header_t* new_block =
		(block_header_t*)((uint8_t*)block + b1_total_size);

	block_header_init(new_block, block->next, block,
					  b2_total_size - sizeof(block_header_t));

	if (block->next) {
		block->next->prev = new_block;
	} else {
		heap.last = new_block;
	}

	block->size = new_size;
	block->next = new_block;

	return new_block;
}

void* kmalloc(size_t size) {
	block_header_t* curr = heap.first;
	while (curr) {
		if (curr->is_free && curr->size >= size) {
			try_split_block(curr, size);
			curr->is_free = false;

			return (void*)(curr + 1);
		}

		curr = curr->next;
	}

	curr = kbrk(size + sizeof(block_header_t));
	if (!curr) {
		return NULL;
	}

	try_split_block(curr, size);
	curr->is_free = false;

	return (void*)(curr + 1);
}

static block_header_t* coalesce_next(block_header_t* block) {
	if (!block || !block->next || !block->is_free || !block->next->is_free) {
		return NULL;
	}

	block_header_t* next = block->next;

	block->size += sizeof(block_header_t) + next->size;
	block->next = next->next;

	if (next->next) {
		next->next->prev = block;
	} else {
		heap.last = block;
	}

	memset((void*)next, 0, sizeof(block_header_t));

	return block;
}

void kfree(void* objp) {
	if (!objp) {
		return;
	}

	block_header_t* block = (block_header_t*)objp - 1;
	if (block->magic != BLOCK_MAGIC) {
		return;
	}

	block->is_free = true;
	coalesce_next(block);
	coalesce_next(block->prev);
}
