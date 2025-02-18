#include <string.h>

int strncmp(const char* s1, const char* s2, size_t n) {
	if (!s1 || !s2 || n == 0)
		return (0);
	return memcmp(s1, s2, n);
}
