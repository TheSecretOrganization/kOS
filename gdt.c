#include <stdint.h>

#define MAX_GDT_ENTRY 65535

#define GDT_ACCESS_PRESENT	0x80
#define GDT_ACCESS_DPL_USER	0x60
#define GDT_ACCESS_DATA		0x10
#define GDT_ACCESS_CODE		0x18
#define GDT_ACCESS_DC		0x04
#define GDT_ACCESS_RW		0x02
#define GDT_ACCESS_A		0x01

#define GDT_FLAG_PAGE_GRAN	0x08
#define GDT_FLAG_32P		0x04
#define GDT_FLAG_LONG		0x02

#define GDT_ACCESS_BASE		(GDT_ACCESS_PRESENT | GDT_ACCESS_RW)
#define GDT_FLAG_BASE		(GDT_FLAG_PAGE_GRAN | GDT_FLAG_32P)

void term_putstr(char *str);

typedef struct __attribute__((packed)) {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_mid;
	uint8_t access;
	uint8_t	flag_limit;
	uint8_t base_high;
} gdt_entry_t;

typedef struct __attribute__((packed)) {
	uint16_t size;
	uint32_t offset;
} gdtr_t;

gdt_entry_t gdt[MAX_GDT_ENTRY];
gdtr_t gdtr;

void gdt_set_entry(gdt_entry_t *entry, uint32_t limit, uint32_t base, uint8_t access, uint8_t flags) {
	entry->limit_low = limit & 0xFFFF;
	entry->base_low = base & 0xFFFF;
	entry->base_mid = (base >> 16) & 0xFF;
	entry->access = access;
	entry->flag_limit = (limit >> 16) & 0xF;
	entry->flag_limit |= flags << 4;
	entry->base_high = (base >> 24) & 0xFF;
}

void gdt_setup() {
	gdt_set_entry(&gdt[0], 0, 0, 0, 0);
	gdt_set_entry(&gdt[1], 0xFFFFF, 0, GDT_ACCESS_BASE | GDT_ACCESS_CODE, GDT_FLAG_BASE);
	gdt_set_entry(&gdt[2], 0xFFFFF, 0, GDT_ACCESS_BASE | GDT_ACCESS_DATA, GDT_FLAG_BASE);
	gdt_set_entry(&gdt[3], 0xFFFFF, 0, GDT_ACCESS_BASE | GDT_ACCESS_DPL_USER | GDT_ACCESS_CODE, GDT_FLAG_BASE);
	gdt_set_entry(&gdt[4], 0xFFFFF, 0, GDT_ACCESS_BASE | GDT_ACCESS_DPL_USER | GDT_ACCESS_DATA, GDT_FLAG_BASE);
}

void gdt_load() {
	gdtr.size = sizeof(gdt) - 1;
	gdtr.offset = (uint32_t) &gdt;
	
	asm volatile ("lgdt %0" : : "m"(gdtr));

	term_putstr("gdt loaded\n");
}
