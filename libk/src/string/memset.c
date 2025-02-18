#include <string.h>

void* memset(void* s, int c, size_t n) {
	if (!s)
		return NULL;
	for (size_t i = 0; i < n; i++)
		((unsigned char*)s)[i] = c;
	return s;
}
