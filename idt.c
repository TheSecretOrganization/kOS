#include <stdint.h>
#include <stddef.h>

#define IDT_MAX_ENTRY		256

#define IDT_GATE_INTERRUPT	0x0E
#define IDT_GATE_TRAP		0x0F
#define IDT_DPL_USER		0x60
#define IDT_PRESENT			0x80

void term_putstr(char *str);

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

idt_entry_t idt[IDT_MAX_ENTRY];
idtr_t idtr;

struct interrupt_frame;

__attribute__((interrupt)) 
void keyboard_handler(__attribute__((unused)) struct interrupt_frame *frame) {
	term_putstr("interrupt!");
}

__attribute__((interrupt))
void exception_handler(__attribute__((unused)) struct interrupt_frame *frame) {
	term_putstr("exception!");
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
	idt_set_entry(&idt[33], (uint32_t) &keyboard_handler, 0x08, IDT_PRESENT | IDT_GATE_INTERRUPT);
}

void idt_load() {
	idtr.size = (uint16_t) sizeof(idt) - 1;
	idtr.offset = (uint32_t) &idt;

	asm volatile ("lidt %0" : : "m"(idtr));

	term_putstr("idt loaded\n");
}
