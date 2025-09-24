#include "command.h"
#include <stdio.h>
#include <string.h>

void cmd_handle(const char* command) {
	if (strcmp(command, "ping") == 0) {
		cmd_ping();
	} else if (strcmp(command, "crash") == 0) {
		cmd_crash();
	} else if (strcmp(command, "clear") == 0) {
		cmd_clear();
	} else {
		if (strlen(command) > 0)
			printf("%s: command not found\n", command);
	}
}
