#pragma once

#include <stddef.h>
#include <stdint.h>

void print_stack(uint32_t* start, const uint32_t* end, size_t n);
void print_stack_trace();
