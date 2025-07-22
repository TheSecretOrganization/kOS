MAGIC           equ 0x1BADB002
FLAGS           equ 1<<0 | 1<<1
CHECKSUM        equ -(MAGIC + FLAGS)
VIRTUAL_BASE    equ 0xC0000000

section .multiboot.data
align 4
dd MAGIC
dd FLAGS
dd CHECKSUM

section .stack nobits
align 16
stack_bottom:
	resb 0x4000
stack_top:

section .bss nobits
align 0x1000
page_directory:
	resb 0x1000
page_table:
	resb 0x1000

section .multiboot.text
extern _kernel_start
extern _kernel_end
extern kernel_main

global _start
_start:
    cli
	mov edi, page_table - VIRTUAL_BASE
	mov esi, 0
	mov ecx, 1023

map_kernel:
	cmp esi, _kernel_start
	jl next_page
	cmp esi, _kernel_end - VIRTUAL_BASE
	jge final_mapping

	mov edx, esi
	or edx, 0x003
	mov [edi], edx

next_page:
	add esi, 0x1000
	add edi, 4
	loop map_kernel

final_mapping:
	mov dword [page_table - VIRTUAL_BASE + 1023 * 4], 0x000B8000 | 0x003

	mov dword [page_directory - VIRTUAL_BASE + 0], page_table - VIRTUAL_BASE + 0x003
	mov dword [page_directory - VIRTUAL_BASE + 768 * 4], page_table - VIRTUAL_BASE + 0x003

	mov dword [page_directory - VIRTUAL_BASE + 1023 * 4], page_directory - VIRTUAL_BASE + 0x003

	mov ecx, page_directory - VIRTUAL_BASE
	mov cr3, ecx

	mov ecx, cr0
	or ecx, 0x80010000
	mov cr0, ecx

	lea ecx, [enter_virtual_mode]
	jmp ecx


section .text
enter_virtual_mode:
	mov dword [page_directory + 0], 0

	mov ecx, cr3
	mov cr3, ecx

	mov esp, stack_top
	call kernel_main

hang:
	hlt
	jmp hang
