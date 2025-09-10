#include "isr.h"
#include "io.h"
#include "keyboard.h"
#include "pic.h"
#include "tty.h"
#include <stdbool.h>
#include <stdint.h>

char keymap_qwerty[0x54] = {
	0, 0,	 '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-',	 '=',
	0, '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[',	 ']',
	0, 0,	 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
	0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,	 '*',
	0, ' ',	 0,	  0,   0,	0,	 0,	  0,   0,	0,	 0,	  0,   0,	 0,
	0, '7',	 '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0',	 '.'};

char keymap_qwerty_shift[0x54] = {
	0, 0,	 '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',
	0, '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}',
	0, 0,	 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
	0, '|',	 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,	'*',
	0, ' ',	 0,	  0,   0,	0,	 0,	  0,   0,	0,	 0,	  0,   0,	0,
	0, '7',	 '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.'};

bool shift = false;
bool alt = false;

__attribute__((interrupt)) void
isr_exception_handler(__attribute__((unused)) struct interrupt_frame* frame) {
	tty_set_color(VGA_COLOR_WHITE, VGA_COLOR_LIGHT_BLUE);
	tty_clear();
	tty_putstr("Exception occurred\n");
	tty_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	asm volatile("cli; hlt");
}

__attribute__((interrupt)) void
isr_keyboard_handler(__attribute__((unused)) struct interrupt_frame* frame) {
	uint8_t keycode = inb(IO_KEYBOARD_DATA);

	if (keycode & KC_RELEASE) {
		keycode &= ~KC_RELEASE;
		switch (keycode) {
		case KC_LSHIFT:
		case KC_RSHIFT:
			shift = false;
			break;
		case KC_LALT:
			alt = false;
			break;
		}
	} else {
		char c = keymap_qwerty[keycode];

		switch (keycode) {
		case KC_LSHIFT:
		case KC_RSHIFT:
			shift = true;
			break;
		case KC_LALT:
			alt = true;
			break;
		case KC_BACKSPACE:
			tty_backspace();
			break;
		default:
			if (keycode == KC_ENTER) {
				tty_handle_entry(KC_ENTER);
				break;
			}

			if (c == 0)
				break;
			if (alt) {
				size_t alt_n = c - '0';
				if (is_valid_tty(alt_n))
					tty_change_screen(alt_n);
			} else if (shift)
				tty_handle_entry(keymap_qwerty_shift[keycode]);
			else
				tty_handle_entry(keymap_qwerty[keycode]);
			break;
		}
	}

	pic_eoi(1);
}
