#include "tty.h"
#include "io.h"
#include "string.h"
#include <stdint.h>

tty_t ttys[4];
tty_t* curr_tty = &ttys[0];
uint16_t* vga_buf = (uint16_t*)VGA_MEMORY_BASE;

static void tty_putnl_at(size_t y) {
	for (size_t x = 0; x < VGA_WIDTH; x++)
		tty_putchar_at(' ', x, y);
}

static inline void tty_putendnl() { tty_putnl_at(VGA_HEIGHT - 1); }

static inline char tty_get_current_char() {
	return vga_get_char(vga_buf[curr_tty->row * VGA_WIDTH + curr_tty->column]);
}

void tty_move_cursor(size_t x, size_t y) {
	uint16_t pos = y * VGA_WIDTH + x;
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t)(pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void tty_init() {
	curr_tty->color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	tty_clear();
	for (size_t i = 0; i < TTY_MAX_SCREEN; i++) {
		ttys[i].row = 0;
		ttys[i].column = 0;
		ttys[i].color = curr_tty->color;
		memcpy(ttys[i].buf, vga_buf, VGA_BUFFER_SIZE);
	}
}

void tty_scroll() {
	for (size_t y = 1; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			size_t index = y * VGA_WIDTH + x;
			vga_buf[index - VGA_WIDTH] = vga_buf[index];
		}
	}
	tty_putendnl();
}

void tty_set_color(enum vga_color front, enum vga_color back) {
	curr_tty->color = vga_entry_color(front, back);
}

void tty_putchar_at(unsigned char c, size_t x, size_t y) {
	size_t index = y * VGA_WIDTH + x;
	vga_buf[index] = vga_entry(c, curr_tty->color);
}

void tty_putchar(unsigned char c) {
	if (c != '\n')
		tty_putchar_at(c, curr_tty->column, curr_tty->row);
	if (++curr_tty->column == VGA_WIDTH || c == '\n') {
		curr_tty->column = 0;
		if (++curr_tty->row == VGA_HEIGHT) {
			tty_scroll();
			curr_tty->row -= 1;
		}
	}
	tty_move_cursor(curr_tty->column, curr_tty->row);
}

void tty_backspace() {
	if (curr_tty->column == 0) {
		if (curr_tty->row == 0)
			return;
		curr_tty->column = VGA_WIDTH - 1;
		curr_tty->row--;
		while (tty_get_current_char() == ' ' && curr_tty->column != 0)
			curr_tty->column--;
		if (curr_tty->column != 0 ||
			(curr_tty->column == 0 && tty_get_current_char() != ' '))
			curr_tty->column++;
	} else {
		curr_tty->column--;
	}
	tty_putchar_at(' ', curr_tty->column, curr_tty->row);
	tty_move_cursor(curr_tty->column, curr_tty->row);
}

void tty_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		tty_putchar(data[i]);
}

void tty_putstr(const char* str) { tty_write(str, strlen(str)); }

void tty_clear() {
	for (size_t y = 0; y < VGA_HEIGHT; y++)
		tty_putnl_at(y);
	curr_tty->row = 0;
	curr_tty->column = 0;
	tty_move_cursor(curr_tty->column, curr_tty->row);
}

void tty_change_screen(size_t screen_number) {
	if (!is_valid_tty(screen_number) || curr_tty == &ttys[screen_number - 1])
		return;
	memcpy(curr_tty->buf, vga_buf, VGA_BUFFER_SIZE);
	curr_tty = &ttys[screen_number - 1];
	memcpy(vga_buf, curr_tty->buf, VGA_BUFFER_SIZE);
	tty_move_cursor(curr_tty->column, curr_tty->row);
}
