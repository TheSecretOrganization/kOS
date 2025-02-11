#include "isr.h"
#include "tty.h"

__attribute__((interrupt)) void
isr_exception_handler(__attribute__((unused)) struct interrupt_frame* frame) {
	tty_putstr("Exception occurred\n");
	asm volatile("cli; hlt");
}
