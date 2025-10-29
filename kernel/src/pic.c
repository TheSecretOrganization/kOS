#include "pic.h"
#include "io.h"
#include <stdbool.h>

void outb_wait(uint16_t port, uint8_t val) {
	outb(port, val);
	io_wait();
}

void pic_eoi(uint8_t irq) {
	if (irq >= 8)
		outb(PIC_CHILD_COMMAND, PIC_EOI);
	outb(PIC_PARENT_COMMAND, PIC_EOI);
}

static void pic_set_irq_mask(uint8_t irq, bool enable) {
	uint16_t port;
	uint8_t mask;

	if (irq < 8) {
		port = PIC_PARENT_DATA;
	} else {
		port = PIC_CHILD_DATA;
		irq -= 8;
	}

	mask = inb(port);
	if (enable) {
		mask &= ~(1 << irq);
	} else {
		mask |= (1 << irq);
	}
	outb(port, mask);
}

void pic_enable_irq(uint8_t irq) { pic_set_irq_mask(irq, true); }

void pic_disable_irq(uint8_t irq) { pic_set_irq_mask(irq, false); }

void pic_remap(int parent_offset, int child_offset) {
	outb_wait(PIC_PARENT_COMMAND, PIC_ICW1_INIT);
	outb_wait(PIC_CHILD_COMMAND, PIC_ICW1_INIT);
	outb_wait(PIC_PARENT_DATA, parent_offset);
	outb_wait(PIC_CHILD_DATA, child_offset);
	outb_wait(PIC_PARENT_DATA, 4);
	outb_wait(PIC_CHILD_DATA, 2);
	outb_wait(PIC_PARENT_DATA, PIC_ICW4_8086);
	outb_wait(PIC_CHILD_DATA, PIC_ICW4_8086);

	// Disable all IRQs
	outb_wait(PIC_PARENT_DATA, 255);
	outb_wait(PIC_CHILD_DATA, 255);
}
