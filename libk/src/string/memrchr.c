#include <string.h>

void* memrchr(const void* s, int c, size_t n) {
	if (!s)
		return NULL;
	for (size_t i = n; i > 0; i--)
		if ((unsigned char)c == ((const unsigned char*)s)[i - 1])
			return (void*)((const unsigned char*)s + i - 1);
	return NULL;
}
