#include "string.h"

int memcmp(const void* s1, const void* s2, size_t n) {
	if (!s1 || !s2 || n == 0)
		return (0);
	size_t i = 0;
	while (i + 1 < n && ((unsigned char*)s1)[i] == ((unsigned char*)s2)[i])
		i++;
	return (((unsigned char*)s1)[i] - ((unsigned char*)s2)[i]);
}
