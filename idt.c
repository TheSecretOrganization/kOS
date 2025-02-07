#include <stdint.h>
#include <stddef.h>

#define IDT_MAX_ENTRY		256

#define IDT_GATE_INTERRUPT	0x0E
#define IDT_GATE_TRAP		0x0F
#define IDT_DPL_USER		0x60
#define IDT_PRESENT			0x80

extern uint8_t inb(uint16_t port);
void term_putstr(char *str);
void term_putchar(unsigned char c);
void pic_eoi(uint8_t irq);

typedef struct __attribute__((packed)) {
	uint16_t offset_low;
	uint16_t segment;
	uint8_t reserved;
	uint8_t flags;
	uint16_t offset_high;
} idt_entry_t;

typedef struct __attribute__((packed)) {
	uint16_t size;
	uint32_t offset;
} idtr_t;

#define KC_RELEASE	0x80
#define KC_ENTER	0x1C
#define KC_LSHIFT	0x2A
#define KC_RSHIFT	0x36

char keymap_qwerty[0x54] = {
      0,   0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',   0,'\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0  , 0  , 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';','\'', '`',   0,'\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/',   0, '*',   0, ' ',   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0, '7', '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.'
};

char keymap_qwerty_shift[0x54] = {
      0,   0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',   0,'\t',
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 0  , 0  , 'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',   0, '|', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', '<', '>', '?',   0, '*',   0, ' ',   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0, '7', '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.'
};

idt_entry_t idt[IDT_MAX_ENTRY];
idtr_t idtr;

struct interrupt_frame;

__attribute__((interrupt)) 
void keyboard_handler(__attribute__((unused)) struct interrupt_frame *frame) {
	static int shift = 0;

	uint8_t key = inb(0x60);

	if (key & KC_RELEASE) {
		switch(key & ~KC_RELEASE) {
			case KC_LSHIFT:
			case KC_RSHIFT:
				shift = 0;
				break;
		}
	} else {
		char c = keymap_qwerty[key];

		if (c != 0) {
			if (shift)
				term_putchar(keymap_qwerty_shift[key]);
			else
				term_putchar(keymap_qwerty[key]);
		}

		switch(key) {
			case KC_ENTER:
				term_putchar('\n');
				break;
			case KC_LSHIFT:
			case KC_RSHIFT:
				shift = 1;
				break;
		}
	}

	pic_eoi(1);
}

__attribute__((interrupt))
void irq_handler(__attribute__((unused)) struct interrupt_frame *frame) {
	pic_eoi(0);
}

__attribute__((interrupt))
void exception_handler(__attribute__((unused)) struct interrupt_frame *frame) {
	term_putstr("exception!\n");
}

void idt_set_entry(idt_entry_t *entry, uint32_t offset, uint16_t segment, uint16_t flags) {
	entry->offset_low = offset & 0xFFFF;
	entry->segment = segment;
	entry->reserved = 0;
	entry->flags = flags;
	entry->offset_high = (offset >> 16) & 0xFFFF;
}

void idt_setup() {
	for (size_t i = 0; i < 32; i++)
		idt_set_entry(&idt[i], (uint32_t) &exception_handler, 0x08, IDT_PRESENT | IDT_GATE_INTERRUPT);
	idt_set_entry(&idt[32], (uint32_t) &irq_handler, 0x08, IDT_PRESENT | IDT_GATE_INTERRUPT);
	idt_set_entry(&idt[33], (uint32_t) &keyboard_handler, 0x08, IDT_PRESENT | IDT_GATE_INTERRUPT);
}

void idt_load() {
	idtr.size = (uint16_t) sizeof(idt) - 1;
	idtr.offset = (uint32_t) &idt;

	asm volatile ("lidt %0" : : "m"(idtr));

	term_putstr("idt loaded\n");
}
