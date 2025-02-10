#pragma once

struct interrupt_frame;

void isr_exception_handler(struct interrupt_frame* frame);
