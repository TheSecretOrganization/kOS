#pragma once

#include <stdint.h>

#define PIC_PARENT 0x20
#define PIC_CHILD 0xA0
#define PIC_PARENT_COMMAND PIC_PARENT
#define PIC_PARENT_DATA (PIC_PARENT + 1)
#define PIC_CHILD_COMMAND PIC_CHILD
#define PIC_CHILD_DATA (PIC_CHILD + 1)

#define PIC_EOI 0x20
#define PIC_ICW1_INIT 0x11
#define PIC_ICW4_8086 0x01

void pic_eoi(uint8_t irq);
void pic_remap(int parent_offset, int child_offset);
