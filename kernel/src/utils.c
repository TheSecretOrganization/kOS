#include "utils.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define MAX_STACK_LINES 1
#define MAX_TRACE_LINES 9

void print_stack_trace() {
	uint32_t* ebp;
	asm volatile("mov %0, ebp" : "=r"(ebp));

	printf("--- STACK ---\n");
	uint32_t* bot = ebp + 2;
	const uint32_t* top = (uint32_t*)*ebp;

	printf("esp: %p ebp: %p frame size: %d\n\n", ebp + 2, *ebp, top - ebp);

	size_t i = 0;
	size_t lines = 0;
	while (bot != top && lines <= MAX_STACK_LINES) {
		if (i == 0)
			printf("%p: ", bot);
		printf("%x ", *bot);
		bot++;
		i++;

		if (i == 4 && bot != top) {
			printf("\n");
			i = 0;
			lines++;
		}
	}

	ebp = (uint32_t*)*ebp;
	printf("\n\n--- STACK TRACE ---\n");
	lines = 0;
	while (ebp && lines <= MAX_TRACE_LINES) {
		printf("return address: %p\n", *(ebp + 1));
		lines++;
		ebp = (uint32_t*)*ebp;
	}
}
