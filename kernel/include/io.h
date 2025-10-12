#pragma once

#include <stdint.h>

#define IO_KEYBOARD_DATA 0x60

static inline void outb(uint16_t port, uint8_t val) {
	__asm__ volatile("out dx, al" : : "a"(val), "d"(port) : "memory");
}

static inline void outw(uint16_t port, uint16_t val) {
	__asm__ volatile("out dx, ax" : : "a"(val), "d"(port) : "memory");
}

static inline uint8_t inb(uint16_t port) {
	uint8_t ret;
	__asm__ volatile("in al, dx" : "=a"(ret) : "d"(port) : "memory");
	return ret;
}

static inline void io_wait() { outb(0x80, 0); }
