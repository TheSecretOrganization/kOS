#pragma once

#include <stdarg.h>
#include <stddef.h>

#ifndef LOGLEVEL
#define LOGLEVEL 6
#endif

#define KERN_EMERG "0"
#define KERN_ALERT "1"
#define KERN_CRIT "2"
#define KERN_ERR "3"
#define KERN_WARNING "4"
#define KERN_NOTICE "5"
#define KERN_INFO "6"
#define KERN_DEBUG "7"
#define KERN_DEFAULT ""
#define KERN_CONT "c"

extern int console_loglevel;

int vprintf(const char* restrict format, va_list ap);
int printf(const char* restrict format, ...);
int printk(const char* restrict format, ...);
