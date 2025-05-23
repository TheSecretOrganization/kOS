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
char* strchr(const char* s, int c);
int strcmp(const char* s1, const char* s2);
size_t strlcpy(char* dst, const char* src, size_t size);
size_t strlen(const char*);
int strncmp(const char* s1, const char* s2, size_t n);
char* strrchr(const char* s, int c);

#ifdef __cplusplus
}
#endif
