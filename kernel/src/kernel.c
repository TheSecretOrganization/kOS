#include "gdt/gdt.h"
#include "tty.h"

void kernel_main() {
	tty_init();
	gdt_setup();
	gdt_load();
	tty_putstr("42\nHello World!\n");
}
