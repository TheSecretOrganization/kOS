#pragma once

#include <stdint.h>

#define IDT_MAX_ENTRY 256

#define IDT_GATE_INTERRUPT 0x0E
#define IDT_GATE_TRAP 0x0F
#define IDT_DPL_USER 0x60
#define IDT_PRESENT 0x80

typedef struct {
	uint16_t offset_low;
	uint16_t segment;
	uint8_t reserved;
	uint8_t flags;
	uint16_t offset_high;
} __attribute__((packed)) idt_entry_t;

typedef struct {
	uint16_t size;
	uint32_t offset;
} __attribute__((packed)) idtr_t;

void idt_set_entry(idt_entry_t* entry, uint32_t offset, uint16_t segment,
				   uint16_t flags);
void idt_setup();
void idt_load();
