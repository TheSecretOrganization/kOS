#include <string.h>

char* strchr(const char* s, int c) {
	return s ? (char*)memchr(s, c, strlen(s) + 1) : NULL;
}
