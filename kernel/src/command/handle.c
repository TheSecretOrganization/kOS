#include "command.h"
#include "io.h"
#include <string.h>

void cmd_handle(const char* command) {
	if (strcmp(command, "ping") == 0)
		cmd_ping();
	else if (strcmp(command, "crash") == 0)
		cmd_crash();
	else if (strcmp(command, "reboot") == 0)
		cmd_reboot();
	else if (strcmp(command, "halt") == 0)
		outw(0x604, 0x2000); // QEMU specific
}
