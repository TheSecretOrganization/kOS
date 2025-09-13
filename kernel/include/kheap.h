#pragma once

#include <stddef.h>

typedef unsigned int gfp_t;

void* kmalloc(size_t size, gfp_t flags);
void kfree();
size_t ksize(const void* obj);
void kbrk();
