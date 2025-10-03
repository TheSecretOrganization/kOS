#include "command.h"
#include "kpanic.h"

void cmd_crash() { asm volatile("jmp 0xFFFF"); }
