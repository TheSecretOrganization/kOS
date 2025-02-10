#include "idt.h"
#include "isr.h"
#include <stdint.h>

idt_entry_t idt[IDT_MAX_ENTRY];
idtr_t idtr;

void idt_set_entry(idt_entry_t* entry, uint32_t offset, uint16_t segment,
				   uint16_t flags) {
	entry->offset_low = offset & 0xFFFF;
	entry->segment = segment;
	entry->reserved = 0;
	entry->flags = flags;
	entry->offset_high = (offset >> 16) & 0xFFFF;
}

void idt_setup() {
	for (uint8_t i = 0; i < 32; i++)
		idt_set_entry(&idt[i], (uint32_t)(uintptr_t)&isr_exception_handler,
					  0x08, IDT_PRESENT | IDT_GATE_INTERRUPT);
}

void idt_load() {
	idtr.size = (uint16_t)sizeof(idt) - 1;
	idtr.offset = (uint32_t)(uintptr_t)&idt;

	asm volatile("lidt %0" : : "m"(idtr));
}
