#include <string.h>

char* strrchr(const char* s, int c) {
	return s ? memrchr(s, c, strlen(s) + 1) : NULL;
}
