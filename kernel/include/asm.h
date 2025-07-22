#pragma once

#define CLI() __asm__ __volatile__("cli" ::: "memory")
#define STI() __asm__ __volatile__("sti" ::: "memory")
#define NOP() __asm__ __volatile__("nop" ::: "memory")
#define HLT() __asm__ __volatile__("hlt" ::: "memory")
