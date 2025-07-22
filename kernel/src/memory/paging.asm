section .text
global enable_paging
enable_paging:
    push ebp
    mov ebp, esp

    mov eax, [ebp + 8]
    mov cr3, eax

    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    mov esp, ebp
    pop ebp
    ret

global flush_tlb
flush_tlb:
    push ebp
    mov ebp, esp

    mov eax, cr3
    mov cr3, eax

    mov esp, ebp
    pop ebp
    ret

extern hh_entrypoint
global hh_reload
hh_reload:
	lea ecx, [hh_entrypoint]
	jmp ecx
