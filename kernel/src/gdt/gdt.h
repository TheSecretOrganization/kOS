#pragma once

#include <stdint.h>

#define GDT_MAX_ENTRY 65535

#define GDT_ACCESS_PRESENT 0x80
#define GDT_ACCESS_DPL_USER 0x60
#define GDT_ACCESS_DATA 0x10
#define GDT_ACCESS_CODE 0x18
#define GDT_ACCESS_DC 0x04
#define GDT_ACCESS_RW 0x02
#define GDT_ACCESS_A 0x01

#define GDT_FLAG_PAGE_GRAN 0x08
#define GDT_FLAG_32P 0x04
#define GDT_FLAG_LONG 0x02

#define GDT_ACCESS_BASE (GDT_ACCESS_PRESENT | GDT_ACCESS_RW)
#define GDT_FLAG_BASE (GDT_FLAG_PAGE_GRAN | GDT_FLAG_32P)

typedef struct {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_middle;
	uint8_t access;
	uint8_t flag_limit;
	uint8_t base_high;
} __attribute__((packed)) gdt_entry_t;

typedef struct {
	uint16_t size;
	uint32_t offset;
} __attribute__((packed)) gdtr_t;

extern void gdt_reload();
void gdt_set_entry(gdt_entry_t* entry, uint32_t limit, uint32_t base,
				   uint8_t access, uint8_t flags);
void gdt_setup();
void gdt_load();
