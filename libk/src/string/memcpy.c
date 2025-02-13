#include "string.h"

void* memcpy(void* dest, const void* src, size_t n) {
	if (!dest || !src)
		return NULL;
	for (size_t i = 0; i < n; i++)
		((unsigned char*)dest)[i] = ((const unsigned char*)src)[i];
	return dest;
}
