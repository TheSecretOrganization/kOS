#include "utils.h"
#include "tty.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define MAX_STACK_LINES 9
#define MAX_TRACE_LINES 9

void print_stack_lines(uint32_t* start, const uint32_t* end, size_t n,
					   size_t lines) {
	size_t i = 0;
	size_t l = 0;

	while (start <= end && l < lines) {
		if (i == 0)
			printf("%p: ", start);
		printf("%x ", *start);
		start++;
		i++;

		if (i == n && start != end) {
			printf("\n");
			i = 0;
			l++;
		}
	}

	if (l == lines)
		printf("...\n");
}

void print_stack(uint32_t* start, uint32_t* end, size_t n) {
	print_stack_lines(start, end, n, VGA_HEIGHT - 2);
}

void print_stack_frame_lines(uint32_t* ebp, size_t lines) {
	print_stack_lines(ebp + 2, (uint32_t*)*ebp, 4, lines);
}

void print_stack_frame(uint32_t* ebp) {
	print_stack_frame_lines(ebp, VGA_HEIGHT - 2);
}

void print_stack_trace() {
	uint32_t* ebp;
	asm volatile("mov %0, ebp" : "=r"(ebp));

	printf("--- STACK ---\n");
	const uint32_t* top = (uint32_t*)*ebp;

	printf("esp: %p ebp: %p frame size: %d\n\n", ebp + 2, *ebp, top - ebp);

	print_stack_frame_lines(ebp, MAX_STACK_LINES);

	uint32_t lines = 0;
	ebp = (uint32_t*)*ebp;
	printf("\n\n--- STACK TRACE ---\n");
	lines = 0;
	while (ebp && lines <= MAX_TRACE_LINES) {
		printf("return address: %p\n", *(ebp + 1));
		lines++;
		ebp = (uint32_t*)*ebp;
	}
}
