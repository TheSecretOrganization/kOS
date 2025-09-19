#include "utils.h"
#include <stddef.h>
#include <stdio.h>

void print_stack_trace() {
	unsigned int* ebp;
	asm volatile("movl %%ebp, %0" : "=r"(ebp));

	printf("--- STACK ---\n");
	unsigned int* bot = ebp + 2;
	unsigned int* top = (unsigned int*)*ebp;

	printf("esp: %p ebp: %p frame size: %d\n\n", ebp + 2, *ebp, top - ebp);

	size_t i = 0;
	while (bot != top) {
		if (i == 0)
			printf("%p: ", bot);
		printf("%x ", *bot);
		bot++;
		i++;

		if (i == 4 && bot != top) {
			printf("\n");
			i = 0;
		}
	}

	ebp = (unsigned int*)*ebp;
	printf("\n\n--- STACK TRACE ---\n");
	while (ebp) {
		printf("return address: %p\n", *(ebp + 1));
		ebp = (unsigned int*)*ebp;
	}
}
