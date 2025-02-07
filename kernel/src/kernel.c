#include "tty.h"

void kernel_main() {
	tty_init();
	tty_putstr("42\nHello World!\n");
}
