MAGIC		equ 0x1BADB002
FLAGS		equ 1<<0 | 1<<1
CHECKSUM	equ -(MAGIC + FLAGS)

section .multiboot.data
align 4
dd MAGIC
dd FLAGS
dd CHECKSUM

section .stack nobits
align 16
stack_bottom:
	resb 16384
stack_top:

section .bss nobits
align 4096
boot_page_directory:
	resb 4096
boot_page_table:
	resb 4096

section .multiboot.text
global _start
extern kernel_main
extern _kernel_start
extern _kernel_end

_start:
	mov edi, boot_page_table - 0xC0000000
	mov esi, 0
	mov ecx, 1023

one:
	cmp esi, _kernel_start
	jl two
	cmp esi, _kernel_end - 0xC0000000
	jge three

	mov edx, esi
	or edx, 0x003
	mov [edi], edx

two:
	add esi, 4096
	add edi, 4
	loop one

three:
	mov dword [boot_page_table - 0xC0000000 + 1023 * 4], 0x000B8000 | 0x003

	mov dword [boot_page_directory - 0xC0000000 + 0], boot_page_table - 0xC0000000 + 0x003
	mov dword [boot_page_directory - 0xC0000000 + 768 * 4], boot_page_table - 0xC0000000 + 0x003

	mov ecx, boot_page_directory - 0xC0000000
	mov cr3, ecx

	mov ecx, cr0
	or ecx, 0x80010000
	mov cr0, ecx

	lea ecx, [four]
	jmp ecx


section .text
four:
	mov dword [boot_page_directory + 0], 0

	mov ecx, cr3
	mov cr3, ecx

	mov esp, stack_top
	call kernel_main
	sti
loop:
	hlt
	jmp loop
