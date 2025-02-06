#include "tty.h"
#include "string.h"

const size_t VGA_WIDTH = 80;
const size_t VGA_HEIGHT = 25;

size_t term_row;
size_t term_column;
uint16_t* term_buf;

void term_move_cursor(size_t x, size_t y) {
	uint16_t pos = y * VGA_WIDTH + x;
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t)(pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void term_init() {
	term_row = 0;
	term_column = 0;
	term_buf = (uint16_t*)0xB8000;

	size_t buf_size = VGA_WIDTH * VGA_HEIGHT;
	for (size_t i = 0; i < buf_size; i++)
		term_buf[i] = vga_value(' ', VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
}

void term_scroll() {
	for (size_t y = 1; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			size_t index = y * VGA_WIDTH + x;
			term_buf[index - VGA_WIDTH] = term_buf[index];
		}
	}
	for (size_t x = 0; x < VGA_WIDTH; x++) {
		size_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
		term_buf[index] = vga_value(' ', VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	}
}

void term_putchar_at(unsigned char c, size_t x, size_t y) {
	size_t index = y * VGA_WIDTH + x;
	term_buf[index] = vga_value(c, VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
}

void term_putchar(unsigned char c) {
	if (c != '\n')
		term_putchar_at(c, term_column, term_row);
	if (++term_column == VGA_WIDTH || c == '\n') {
		term_column = 0;
		if (++term_row == VGA_HEIGHT) {
			term_scroll();
			term_row -= 1;
		}
	}
	term_move_cursor(term_column, term_row);
}

void term_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		term_putchar(data[i]);
}

void term_putstr(char* str) { term_write(str, strlen(str)); }
