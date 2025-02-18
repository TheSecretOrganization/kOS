#include <string.h>

int strcmp(const char* s1, const char* s2) {
	if (!s1 || !s2)
		return (0);
	size_t len1 = strlen(s1);
	size_t len2 = strlen(s2);
	return strncmp(s1, s2, len1 > len2 ? len1 : len2);
}
