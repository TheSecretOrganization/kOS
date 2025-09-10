#include "tty.h"
#include "command.h"
#include "io.h"
#include "keyboard.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define TTY_PROMPT ">"

static tty_t ttys[4];
static tty_t* curr_tty = &ttys[0];
static uint16_t* vga_buf = (uint16_t*)VGA_MEMORY_BASE;

static void putendl(size_t y) {
	for (size_t x = 0; x < VGA_WIDTH; x++)
		tty_putchar_at(' ', x, y);
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
	curr_tty->color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	tty_clear();
	for (size_t i = 0; i < TTY_MAX_SCREEN; i++) {
		ttys[i].row = 0;
		ttys[i].column = 0;
		ttys[i].color = curr_tty->color;
		memcpy(ttys[i].buf, vga_buf, VGA_BUFFER_SIZE);
	}
}

void tty_set_color(vga_color_t fg, vga_color_t bg) {
	curr_tty->color = vga_entry_color(fg, bg);
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
			scroll();
			curr_tty->row -= 1;
		}
	}
	tty_move_cursor(curr_tty->column, curr_tty->row);
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
	tty_move_cursor(curr_tty->column, curr_tty->row);
}

void tty_backspace() {
	if (curr_tty->column < strlen(TTY_PROMPT)) {
		return;
	} else {
		curr_tty->column--;
	}
	tty_putchar_at(' ', curr_tty->column, curr_tty->row);
	tty_move_cursor(curr_tty->column, curr_tty->row);
}

void tty_move_cursor(size_t x, size_t y) {
	uint16_t pos = y * VGA_WIDTH + x;
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t)(pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void tty_change_screen(size_t screen_number) {
	if (!is_valid_tty(screen_number) || curr_tty == &ttys[screen_number - 1])
		return;
	memcpy(curr_tty->buf, vga_buf, VGA_BUFFER_SIZE);
	curr_tty = &ttys[screen_number - 1];
	memcpy(vga_buf, curr_tty->buf, VGA_BUFFER_SIZE);
	tty_move_cursor(curr_tty->column, curr_tty->row);
}

void tty_print_prompt() { tty_putstr(TTY_PROMPT); }

static void build_command(char* buf) {
	size_t prompt_len = strlen(TTY_PROMPT);
	const uint16_t* command = &vga_buf[curr_tty->row * VGA_WIDTH + prompt_len];
	for (size_t i = 0; i < curr_tty->column - prompt_len; i++)
		buf[i] = vga_get_char(command[i]);
}

void tty_handle_entry(char c) {
	if (c == KC_ENTER) {
		char buf[VGA_WIDTH] = {0};
		build_command(buf);
		tty_putchar('\n');
		cmd_handle(buf);
		tty_print_prompt();
		return;
	}

	tty_putchar(c);
}
