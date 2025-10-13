#pragma once

#include "multiboot.h"
#include <stddef.h>
#include <stdint.h>

void print_stack(uint32_t* start, const uint32_t* end, size_t n);
void print_stack_trace();
uint64_t detect_memory(const multiboot_info_t* mbd, uint32_t magic);
