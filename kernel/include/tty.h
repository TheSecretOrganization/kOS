#pragma once

#include <stddef.h>
#include <stdint.h>

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t)uc | (uint16_t)color << 8;
}

static inline unsigned char vga_get_char(uint16_t entry) {
	return (unsigned char)entry & 0xFF;
}

void tty_init();
void tty_scroll();
void tty_move_cursor(size_t x, size_t y);
void tty_putchar_at(unsigned char c, size_t x, size_t y);
void tty_set_color(enum vga_color front, enum vga_color back);
void tty_putchar(unsigned char c);
void tty_backspace();
void tty_write(const char* data, size_t size);
void tty_putstr(char* str);
