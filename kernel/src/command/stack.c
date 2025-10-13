#include "command.h"
#include "utils.h"
#include <stdint.h>

extern uint32_t stack_top;
extern uint32_t stack_bottom;

void cmd_stack() {
	*(&stack_bottom + 15) = 0x42; // debug value
	print_stack(&stack_bottom, &stack_top, 24);
}
