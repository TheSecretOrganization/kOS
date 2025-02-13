#pragma once

#define KC_RELEASE 0x80
#define KC_ENTER 0x1C
#define KC_LSHIFT 0x2A
#define KC_RSHIFT 0x36
#define KC_BACKSPACE 0x0E
#define KC_LALT 0x38

struct interrupt_frame;

void isr_exception_handler(struct interrupt_frame* frame);
void isr_keyboard_handler(struct interrupt_frame* frame);
