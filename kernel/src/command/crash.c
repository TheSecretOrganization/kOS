#include "command.h"
#include "kpanic.h"

void cmd_crash() { KPANIC("CRASH!") }
