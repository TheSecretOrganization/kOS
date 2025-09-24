#include "command.h"
#include "io.h"
#include <stdint.h>

void cmd_reboot() {
	uint8_t good = 0x02;
	while (good & 0x02)
		good = inb(0x64);
	outb(0x64, 0xFE);
	asm volatile("hlt");
}
