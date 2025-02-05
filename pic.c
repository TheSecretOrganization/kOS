#include <stdint.h>

#define PIC_PARENT			0x20
#define PIC_CHILD			0xA0
#define PIC_PARENT_COMMAND	PIC_PARENT
#define PIC_PARENT_DATA		(PIC_PARENT + 1)
#define PIC_CHILD_COMMAND	PIC_CHILD
#define PIC_CHILD_DATA		(PIC_CHILD + 1)

#define PIC_EOI				0x20
#define PIC_ICW1_INIT		0x11
#define PIC_ICW4_8086		0x01

extern void outb(uint16_t port, uint8_t val);
extern uint8_t inb(uint16_t port);

void pic_wait() {
	outb(0x80, 0);
}

void outb_wait(uint16_t port, uint8_t val) {
	outb(port, val);
	pic_wait();
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
