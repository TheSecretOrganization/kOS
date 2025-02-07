
section .text
global outb
global inb

outb:
	mov edx, [esp + 4]
	mov al, [esp + 8]
	out dx, al
	ret

inb:
	mov edx, [esp + 4]
	in al, dx
	movzx eax, al
	ret
