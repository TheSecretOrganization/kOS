#include <stddef.h>
#include <stdint.h>

const size_t VGA_WIDTH = 80;
const size_t VGA_HEIGHT = 25;

uint16_t *term_buf;

void init_term()
{
	term_buf = (uint16_t*) 0xB8000;

	size_t buf_size = VGA_WIDTH * VGA_HEIGHT;
	for (size_t i = 0; i < buf_size; i++)
		term_buf[i] = (uint16_t) ' ' | (uint16_t) (7 | 0 << 4) << 8;
}

void kernel_main()
{
	init_term();
	term_buf[0] = (uint16_t) '4' | (uint16_t) (7 | 0 << 4) << 8;
	term_buf[1] = (uint16_t) '2' | (uint16_t) (7 | 0 << 4) << 8;
}
