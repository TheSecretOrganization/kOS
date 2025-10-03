#pragma once

#include "utils.h"
#include <stdio.h>
#include <tty.h>

#define CLEAN_REGISTERS()                                                      \
	asm volatile(                                                              \
		"xor %eax, %eax; xor %ebx, %ebx; xor %ecx, %ecx; xor %edx, %edx");

#define KPANIC(s)                                                              \
	tty_set_color(VGA_COLOR_WHITE, VGA_COLOR_LIGHT_BLUE);                      \
	tty_clear();                                                               \
	printf("[ kOS - KERNEL PANIC at (%s:%d) ] %s\n", __FILE__, __LINE__, s);   \
	print_stack_trace();                                                       \
	tty_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);                      \
	CLEAN_REGISTERS();                                                         \
	asm volatile("cli");                                                       \
	while (42)                                                                 \
		asm volatile("hlt");
