#include <string.h>

size_t strlcpy(char* dst, const char* src, size_t size) {
	if (!dst || !src)
		return 0;
	size_t len = strlen(src);
	size_t i = 0;
	if (size > 0) {
		while (i < size - 1 && i < len) {
			dst[i] = src[i];
			++i;
		}
		dst[i] = '\0';
	}
	return len;
}
