#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void bzero(void* s, size_t n);
void* memchr(const void* s, int c, size_t n);
int memcmp(const void* s1, const void* s2, size_t n);
void* memcpy(void* dest, const void* src, size_t n);
void* memmove(void* dest, const void* src, size_t n);
void* memrchr(const void* s, int c, size_t n);
void* memset(void* s, int c, size_t n);
size_t strlen(const char*);

#ifdef __cplusplus
}
#endif
