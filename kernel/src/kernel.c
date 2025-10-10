#include "gdt.h"
#include "idt.h"
#include "kpanic.h"
#include "multiboot.h"
#include "paging.h"
#include "pic.h"
#include "pmm.h"
#include "tty.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

static uint64_t detect_memory(const multiboot_info_t* mbd, uint32_t magic) {
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		KPANIC("invalid magic number")
	}

	if (!(mbd->flags >> 6 & 0x1)) {
		KPANIC("invalid memory map given by GRUB bootloader")
	}

	uint64_t total_mem = 0;

	for (size_t i = 0; i < mbd->mmap_length;
		 i += sizeof(multiboot_memory_map_t)) {
		const multiboot_memory_map_t* mmmt =
			(multiboot_memory_map_t*)(mbd->mmap_addr + i);

		uint64_t len = ((uint64_t)mmmt->len_high << 32) | mmmt->len_low;

		if (mmmt->type == MULTIBOOT_MEMORY_AVAILABLE) {
			total_mem += len;
		}
	}

	if (total_mem > (uint64_t)UINT32_MAX + 1) {
		total_mem = (uint64_t)UINT32_MAX + 1;
	}

	return total_mem;
}

void kernel_main(const multiboot_info_t* mbd, uint32_t magic) {
	tty_init();

	uint64_t total_mem = detect_memory(mbd, magic);
	pmm_init(total_mem);
	paging_init();

	pic_remap(0x20, 0x28);

	gdt_setup();
	gdt_load();
	gdt_reload();

	idt_setup();
	idt_load();

	tty_set_color(VGA_COLOR_WHITE, VGA_COLOR_LIGHT_BLUE);
	printf("%d\n%s\n", 42, "Hello World!");
	tty_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	tty_print_prompt();
}
