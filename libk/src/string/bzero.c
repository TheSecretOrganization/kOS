#include <string.h>

void bzero(void* s, size_t n) {
	if (!s)
		return;
	memset(s, 0, n);
}
