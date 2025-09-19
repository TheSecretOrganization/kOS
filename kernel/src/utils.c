#include "utils.h"
#include <stddef.h>
#include <stdio.h>

#define MAX_STACK_LINES 1
#define MAX_TRACE_LINES 9

void print_stack_trace() {
	unsigned int* ebp;
	asm volatile("movl %%ebp, %0" : "=r"(ebp));

	printf("--- STACK ---\n");
	unsigned int* bot = ebp + 2;
	const unsigned int* top = (unsigned int*)*ebp;

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

	ebp = (unsigned int*)*ebp;
	printf("\n\n--- STACK TRACE ---\n");
	lines = 0;
	while (ebp && lines <= MAX_TRACE_LINES) {
		printf("return address: %p\n", *(ebp + 1));
		lines++;
		ebp = (unsigned int*)*ebp;
	}
}
