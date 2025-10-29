#include "command.h"
#include "io.h"

void cmd_halt() {
	outw(0x604, 0x2000); // QEMU specific
}
