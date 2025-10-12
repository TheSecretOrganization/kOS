#include "gdt.h"
#include "idt.h"
#include "memory.h"
#include "multiboot.h"
#include "pic.h"
#include "pmm.h"
#include "tty.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

void kernel_main(const multiboot_info_t* mbd, uint32_t magic) {
	tty_init();

	pic_remap(0x20, 0x28);
	gdt_setup();
	gdt_load();
	gdt_reload();
	idt_setup();
	idt_load();

	uint64_t total_mem = detect_memory(mbd, magic);
	pmm_init(total_mem);
	paging_init();

	tty_set_color(VGA_COLOR_WHITE, VGA_COLOR_LIGHT_BLUE);
	printf("%d\n%s\n", 42, "Hello World!");
	tty_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	tty_print_prompt();
}
