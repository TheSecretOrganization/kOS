#include "kheap.h"

static block_header_t* head;

void* kmalloc(size_t size);
void kfree(const void* objp);
size_t ksize(const void* objp);
void* kbrk(size_t increment);

void kheap_init() {}
