#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define TTY_MAX_SCREEN 4
#define VGA_MEMORY_BASE 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_BUFFER_SIZE 4000

typedef struct {
	size_t row;
	size_t column;
	uint8_t color;
	uint16_t buf[VGA_BUFFER_SIZE];
} tty_t;

typedef enum vga_color {
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
} vga_color_t;

static inline uint8_t vga_entry_color(vga_color_t fg, vga_color_t bg) {
	return fg | bg << 4;
}

static inline vga_color_t vga_get_color_fg(uint8_t entry_color) {
	return (vga_color_t)(entry_color & 0x0F);
}

static inline vga_color_t vga_get_color_bg(uint8_t entry_color) {
	return (vga_color_t)((entry_color >> 4) & 0x0F);
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t)uc | (uint16_t)color << 8;
}

static inline unsigned char vga_get_char(uint16_t entry) {
	return (unsigned char)entry & 0xFF;
}

static inline bool is_valid_tty(size_t n_tty) {
	return (n_tty >= 1 && n_tty <= TTY_MAX_SCREEN);
}

void tty_init();
void tty_set_color(vga_color_t fg, vga_color_t bg);
void tty_putchar_at(unsigned char c, size_t x, size_t y);
void tty_putchar(unsigned char c);
void tty_write(const char* data, size_t size);
void tty_putstr(const char* str);
void tty_clear();
void tty_backspace();
void tty_move_cursor(size_t x, size_t y);
void tty_change_screen(size_t screen_number);
void tty_print_prompt();
void tty_handle_entry(char c);
