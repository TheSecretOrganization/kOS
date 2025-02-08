#include "pic/pic.h"
#include "tty.h"

void kernel_main() {
	tty_init();
	pic_remap(0x20, 0x28);
	tty_putstr("42\nHello World!\n");
}
