#include "command.h"

void cmd_crash() { asm volatile("jmp 0xFFFF"); }
