#include "string.h"

void* memmove(void* dest, const void* src, size_t n) {
	if (!dest || !src)
		return (NULL);
	if (dest > src) {
		size_t i = n;
		while (i-- > 0)
			((unsigned char*)dest)[i] = ((const unsigned char*)src)[i];
	} else
		memcpy(dest, src, n);
	return (dest);
}
