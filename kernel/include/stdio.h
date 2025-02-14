#pragma once

#include <stdarg.h>
#include <stddef.h>

int vprintf(const char* restrict format, va_list ap);
int printf(const char* restrict format, ...);
