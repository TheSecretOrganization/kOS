MAGIC		equ 0x1BADB002
FLAGS		equ 1<<0 | 1<<1
CHECKSUM	equ -(MAGIC + FLAGS)

section .multiboot
align 4
dd MAGIC
dd FLAGS
dd CHECKSUM

section .bss
align 16
stack_bottom:
	resb 16384
stack_top:

section .text
extern tty_init
extern paging_setup
extern kernel_main

global _start
_start:
    cli
    call tty_init
    call paging_setup

global hh_entrypoint
hh_entrypoint:
	mov esp, stack_top
	call kernel_main
    sti

.hang:
	hlt
	jmp .hang
