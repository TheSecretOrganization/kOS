#include "command.h"
#include <string.h>

void cmd_handle(const char *command) {
	if (strcmp(command, "ping") == 0)
		cmd_ping();
}
