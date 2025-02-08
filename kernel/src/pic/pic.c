#include "pic.h"
#include "io.h"

void outb_wait(uint16_t port, uint8_t val) {
	outb(port, val);
	io_wait();
}

void pic_eoi(uint8_t irq) {
	if (irq >= 8)
		outb(PIC_CHILD_COMMAND, PIC_EOI);
	outb(PIC_PARENT_COMMAND, PIC_EOI);
}

void pic_remap(int parent_offset, int child_offset) {
	uint8_t parent_mask = inb(PIC_PARENT_DATA);
	uint8_t child_mask = inb(PIC_CHILD_DATA);

	outb_wait(PIC_PARENT_COMMAND, PIC_ICW1_INIT);
	outb_wait(PIC_CHILD_COMMAND, PIC_ICW1_INIT);
	outb_wait(PIC_PARENT_DATA, parent_offset);
	outb_wait(PIC_CHILD_DATA, child_offset);
	outb_wait(PIC_PARENT_DATA, 4);
	outb_wait(PIC_CHILD_DATA, 2);
	outb_wait(PIC_PARENT_DATA, PIC_ICW4_8086);
	outb_wait(PIC_CHILD_DATA, PIC_ICW4_8086);

	outb_wait(PIC_PARENT_DATA, parent_mask);
	outb_wait(PIC_CHILD_DATA, child_mask);
}
