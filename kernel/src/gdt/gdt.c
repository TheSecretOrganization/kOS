#include "gdt.h"

gdt_entry_t gdt[GDT_MAX_ENTRY];
gdtr_t* gdtr = (gdtr_t*)GDT_ADDRESS;

void gdt_set_entry(gdt_entry_t* entry, uint32_t limit, uint32_t base,
				   uint8_t access, uint8_t flags) {
	entry->limit_low = limit & 0xFFFF;
	entry->base_low = base & 0xFFFF;
	entry->base_middle = (base >> 16) & 0xFF;
	entry->access = access;
	entry->flag_limit = (limit >> 16) & 0xF;
	entry->flag_limit |= flags << 4;
	entry->base_high = (base >> 24) & 0xFF;
}

void gdt_setup() {
	gdt_set_entry(&gdt[0], 0, 0, 0, 0);
	gdt_set_entry(&gdt[1], 0xFFFFF, 0, GDT_ACCESS_BASE | GDT_ACCESS_CODE,
				  GDT_FLAG_BASE);
	gdt_set_entry(&gdt[2], 0xFFFFF, 0, GDT_ACCESS_BASE | GDT_ACCESS_DATA,
				  GDT_FLAG_BASE);
	gdt_set_entry(&gdt[3], 0xFFFFF, 0,
				  GDT_ACCESS_BASE | GDT_ACCESS_DATA | GDT_ACCESS_DC,
				  GDT_FLAG_BASE);
	gdt_set_entry(&gdt[4], 0xFFFFF, 0,
				  GDT_ACCESS_BASE | GDT_ACCESS_DPL_USER | GDT_ACCESS_CODE,
				  GDT_FLAG_BASE);
	gdt_set_entry(&gdt[5], 0xFFFFF, 0,
				  GDT_ACCESS_BASE | GDT_ACCESS_DPL_USER | GDT_ACCESS_DATA,
				  GDT_FLAG_BASE);
	gdt_set_entry(&gdt[6], 0xFFFFF, 0,
				  GDT_ACCESS_BASE | GDT_ACCESS_DPL_USER | GDT_ACCESS_DATA |
					  GDT_ACCESS_DC,
				  GDT_FLAG_BASE);
}

void gdt_load() {
	gdtr->size = (uint16_t)sizeof(gdt) - 1;
	gdtr->offset = (uint32_t)(uintptr_t)&gdt;

	asm volatile("lgdt %0" : : "m"(*gdtr));
}
