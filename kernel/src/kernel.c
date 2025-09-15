#include "gdt.h"
#include "idt.h"
#include "paging.h"
#include "pic.h"
#include "tty.h"
#include <stdio.h>

void kernel_main() {
	tty_init();
	pic_remap(0x20, 0x28);
	gdt_setup();
	gdt_load();
	gdt_reload();
	idt_setup();
	idt_load();
	paging_setup();
	tty_set_color(VGA_COLOR_WHITE, VGA_COLOR_LIGHT_BLUE);
	printf("%d\n%s\n", 42, "Hello World!");
	tty_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	tty_print_prompt();
}
