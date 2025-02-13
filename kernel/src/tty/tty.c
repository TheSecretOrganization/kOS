#include "tty.h"
#include "io.h"
#include "string.h"
#include <stdint.h>

const size_t VGA_WIDTH = 80;
const size_t VGA_HEIGHT = 25;

size_t tty_row;
size_t tty_column;
uint8_t tty_color;
uint16_t* tty_buf;

void tty_move_cursor(size_t x, size_t y) {
	uint16_t pos = y * VGA_WIDTH + x;
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t)(pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void tty_init() {
	tty_row = 0;
	tty_column = 0;
	tty_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	tty_buf = (uint16_t*)0xB8000;

	size_t buf_size = VGA_WIDTH * VGA_HEIGHT;
	for (size_t i = 0; i < buf_size; i++)
		tty_buf[i] = vga_entry(' ', tty_color);
}

void tty_scroll() {
	for (size_t y = 1; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			size_t index = y * VGA_WIDTH + x;
			tty_buf[index - VGA_WIDTH] = tty_buf[index];
		}
	}
	for (size_t x = 0; x < VGA_WIDTH; x++) {
		size_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
		tty_buf[index] = vga_entry(' ', tty_color);
	}
}

void tty_set_color(enum vga_color front, enum vga_color back) {
	tty_color = vga_entry_color(front, back);
}

void tty_putchar_at(unsigned char c, size_t x, size_t y) {
	size_t index = y * VGA_WIDTH + x;
	tty_buf[index] = vga_entry(c, tty_color);
}

void tty_putchar(unsigned char c) {
	if (c != '\n')
		tty_putchar_at(c, tty_column, tty_row);
	if (++tty_column == VGA_WIDTH || c == '\n') {
		tty_column = 0;
		if (++tty_row == VGA_HEIGHT) {
			tty_scroll();
			tty_row -= 1;
		}
	}
	tty_move_cursor(tty_column, tty_row);
}

void tty_backspace() {
	if (tty_column == 0) {
		if (tty_row == 0)
			return;
		tty_column = VGA_WIDTH - 1;
		tty_row--;
		while (vga_get_char(tty_buf[tty_row * VGA_WIDTH + tty_column]) == ' ' &&
			   tty_column != 0)
			tty_column--;
		if (tty_column != 0 ||
			(tty_column == 0 &&
			 vga_get_char(tty_buf[tty_row * VGA_WIDTH + tty_column]) != ' '))
			tty_column++;
	} else {
		tty_column--;
	}
	tty_putchar_at(' ', tty_column, tty_row);
	tty_move_cursor(tty_column, tty_row);
}

void tty_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		tty_putchar(data[i]);
}

void tty_putstr(char* str) { tty_write(str, strlen(str)); }
