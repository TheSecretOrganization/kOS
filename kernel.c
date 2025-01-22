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

const size_t VGA_WIDTH = 80;
const size_t VGA_HEIGHT = 25;

size_t term_row;
size_t term_column;
uint16_t *term_buf;

static inline void outb(uint16_t port, uint8_t val) {
	__asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

uint16_t vga_value(unsigned char c, enum vga_color fg, enum vga_color bg) {
	return (uint16_t) c | (uint16_t) (fg | bg << 4) << 8;
}

void cursor_move(size_t x, size_t y) {
	uint16_t pos = y * VGA_WIDTH + x;
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void init_term() {
	term_row = 0;
	term_column = 0;
	term_buf = (uint16_t*) 0xB8000;

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
	cursor_move(term_column, term_row);
}

void term_putstr(char *str) {
	size_t i = -1;

	while (str[++i])
		term_putchar(str[i]);
}

void kernel_main() {
	init_term();
	term_putstr("42\nHello World!\n");
}
