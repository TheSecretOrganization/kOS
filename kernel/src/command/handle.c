#include "command.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

typedef struct {
	const char* name;
	void (*handler)(void);
} command_t;

static const command_t commands[] = {
	{"ping", &cmd_ping}, {"crash", &cmd_crash}, {"reboot", &cmd_reboot},
	{"halt", &cmd_halt}, {"clear", &cmd_clear}, {"stack", &cmd_stack},
	{NULL, NULL} // sentinel
};

void cmd_handle(const char* command) {
	for (size_t i = 0; commands[i].name != NULL; i++) {
		if (strcmp(command, commands[i].name) == 0) {
			commands[i].handler();
			return;
		}
	}

	if (strlen(command) > 0)
		printf("%s: command not found\n", command);
}
