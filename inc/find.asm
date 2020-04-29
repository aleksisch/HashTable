SECTION .text
global hash5
extern strcmp
hash5:
    mov rax, 0           ;int tmp = 0;
    .Loop:
        xor al, [rdi]
        rol rax, 1
        inc rdi
        cmp byte [rdi], 0
        jne .Loop
    ret

global FindListAsm
FindListAsm:
	mov rbx, [rdi + 8]			;start
	mov rdx, [rdi + 16]			;end
	mov rbx, [rbx]				;tmp = start.next
	.Loop:
		cmp rbx, rdx
			je .End
		mov rdi, [rbx + 16]		;rdi = tmp->data
		call strcmp 		;strcmp(tmp->data, rsi = value)
		cmp ax, 0
			je .End
		mov rbx, [rbx]

	.End:

	mov eax, 0
	cmp rbx, rdx
		je .Ret
	mov eax, 1

	.Ret:
		ret
