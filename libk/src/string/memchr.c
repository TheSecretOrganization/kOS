#include "string.h"

void* memchr(const void* s, int c, size_t n) {
	if (!s)
		return NULL;
	for (size_t i = 0; i < n; i++)
		if ((unsigned char)c == ((const unsigned char*)s)[i])
			return (void*)s + i;
	return NULL;
}
