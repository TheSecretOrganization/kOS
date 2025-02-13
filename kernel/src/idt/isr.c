#include "isr.h"
#include "io.h"
#include "pic.h"
#include "tty.h"
#include <stdint.h>
#include <stdbool.h>

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

__attribute__((interrupt)) void
isr_exception_handler(__attribute__((unused)) struct interrupt_frame* frame) {
	tty_putstr("Exception occurred\n");
	asm volatile("cli; hlt");
}

__attribute__((interrupt)) void
isr_keyboard_handler(__attribute__((unused)) struct interrupt_frame* frame) {
	uint8_t keycode = inb(IO_KEYBOARD_DATA);

	if (keycode & KC_RELEASE) {
		keycode &= ~KC_RELEASE;
		if (keycode == KC_LSHIFT || keycode == KC_RSHIFT)
			shift = false;
	} else {
		char c = keymap_qwerty[keycode];

		if (c != 0) {
			if (shift)
				tty_putchar(keymap_qwerty_shift[keycode]);
			else
				tty_putchar(keymap_qwerty[keycode]);
		}

		switch (keycode) {
		case KC_ENTER:
			tty_putchar('\n');
			break;
		case KC_LSHIFT:
		case KC_RSHIFT:
			shift = true;
			break;
		case KC_BACKSPACE:
			tty_backspace();
			break;
		}
	}

	pic_eoi(1);
}
