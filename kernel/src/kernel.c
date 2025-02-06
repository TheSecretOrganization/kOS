#include "tty.h"

void kernel_main() {
	term_init();
	term_putstr("42\nHello World!\n");
}
