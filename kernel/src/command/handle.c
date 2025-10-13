#include "command.h"
#include "io.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

void cmd_handle(const char* command) {
	if (strcmp(command, "ping") == 0) {
		cmd_ping();
	} else if (strcmp(command, "crash") == 0) {
		cmd_crash();
	} else if (strcmp(command, "reboot") == 0) {
		cmd_reboot();
	} else if (strcmp(command, "halt") == 0) {
		outw(0x604, 0x2000); // QEMU specific
	} else if (strcmp(command, "clear") == 0) {
		cmd_clear();
	} else if (strcmp(command, "stack") == 0) {
		cmd_stack();
	} else {
		if (strlen(command) > 0)
			printf("%s: command not found\n", command);
	}
}
