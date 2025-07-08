#include "tty.h"
#include "io.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

tty_t ttys[4];
tty_t* curr_tty = &ttys[0];
uint16_t* vga_buf = (uint16_t*)VGA_MEMORY_BASE;
static uint8_t screen_color = VGA_COLOR_LIGHT_GREY | VGA_COLOR_BLACK << 4;
static uint8_t hidden_color = VGA_COLOR_BLACK | VGA_COLOR_BLACK << 4;

static void set_vga_entry(unsigned char c, uint8_t color, size_t x, size_t y) {
	size_t index = y * VGA_WIDTH + x;
	vga_buf[index] = vga_entry(c, color);
}

static void putendl(size_t y) {
	for (size_t x = 0; x < VGA_WIDTH; x++)
		set_vga_entry(' ', screen_color, x, y);
}

static inline char get_current_char() {
	return vga_get_char(vga_buf[curr_tty->row * VGA_WIDTH + curr_tty->column]);
}

static void move_cursor(size_t x, size_t y) {
	uint16_t pos = y * VGA_WIDTH + x;
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t)(pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

static void scroll() {
	for (size_t y = 1; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			size_t index = y * VGA_WIDTH + x;
			vga_buf[index - VGA_WIDTH] = vga_buf[index];
		}
	}
	putendl(VGA_HEIGHT - 1);
}

void tty_init() {
	curr_tty->color = screen_color;
	tty_clear();
	for (size_t i = 0; i < TTY_MAX_SCREEN; i++) {
		ttys[i].row = 0;
		ttys[i].column = 0;
		ttys[i].color = curr_tty->color;
		memcpy(ttys[i].buf, vga_buf, VGA_BUFFER_SIZE);
	}
}

void tty_set_writer_color(vga_color_t fg, vga_color_t bg) {
	curr_tty->color = vga_entry_color(fg, bg);
}

void tty_set_screen_color(vga_color_t fg, vga_color_t bg) {
	screen_color = vga_entry_color(fg, bg);
	hidden_color = vga_entry_color(bg, bg);
	tty_set_writer_color(fg, bg);
}

void tty_putchar_at(unsigned char c, size_t x, size_t y) {
	set_vga_entry(c, curr_tty->color, x, y);
}

void tty_putchar(unsigned char c) {
	if (c == ' ' && curr_tty->column == VGA_WIDTH - 1)
		c = '\n';
	if (c == '\n')
		set_vga_entry(c, hidden_color, curr_tty->column, curr_tty->row);
	else
		tty_putchar_at(c, curr_tty->column, curr_tty->row);
	if (++curr_tty->column == VGA_WIDTH || c == '\n') {
		curr_tty->column = 0;
		if (++curr_tty->row == VGA_HEIGHT) {
			scroll();
			curr_tty->row -= 1;
		}
	}
	move_cursor(curr_tty->column, curr_tty->row);
}

void tty_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		tty_putchar(data[i]);
}

void tty_putstr(const char* str) { tty_write(str, strlen(str)); }

void tty_clear() {
	for (size_t y = 0; y < VGA_HEIGHT; y++)
		putendl(y);
	curr_tty->row = 0;
	curr_tty->column = 0;
	move_cursor(curr_tty->column, curr_tty->row);
}

void tty_backspace() {
	if (curr_tty->column == 0) {
		if (curr_tty->row == 0)
			return;
		curr_tty->column = VGA_WIDTH - 1;
		curr_tty->row--;
		while (get_current_char() == ' ' && curr_tty->column != 0)
			curr_tty->column--;
	} else {
		curr_tty->column--;
	}
	set_vga_entry(' ', screen_color, curr_tty->column, curr_tty->row);
	move_cursor(curr_tty->column, curr_tty->row);
}

void tty_change_screen(size_t screen_number) {
	if (!is_valid_tty(screen_number) || curr_tty == &ttys[screen_number - 1])
		return;
	memcpy(curr_tty->buf, vga_buf, VGA_BUFFER_SIZE);
	curr_tty = &ttys[screen_number - 1];
	memcpy(vga_buf, curr_tty->buf, VGA_BUFFER_SIZE);
	move_cursor(curr_tty->column, curr_tty->row);
}
