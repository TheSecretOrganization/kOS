#include "command.h"
#include "utils.h"
#include <stdint.h>
#include <stdio.h>

extern uint32_t stack_top;
extern uint32_t stack_bottom;

void cmd_stack() {
	printf("bot %x top %x\n", (void*)&stack_bottom, (void*)&stack_top);
	*((uint32_t*)&stack_bottom + 15) = 0x42; // debug value
	print_stack((uint32_t*)&stack_bottom, (uint32_t*)&stack_top, 24);
}
