
section .text
global gdt_reload

gdt_reload:
	jmp 0x08:.reload
.reload:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ax, 0x18
	mov ss, ax
	ret
