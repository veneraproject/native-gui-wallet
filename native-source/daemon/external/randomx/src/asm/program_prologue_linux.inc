	 
	push rbx
	push rbp
	push r12
	push r13
	push r14
	push r15

	 
	mov rbx, rcx                 
	push rdi                     
	mov rcx, rdi
	mov rbp, qword ptr [rsi]     
	mov rdi, qword ptr [rsi+8]   
	mov rsi, rdx                 

	mov rax, rbp
	ror rbp, 32

	 
	xor r8, r8
	xor r9, r9
	xor r10, r10
	xor r11, r11
	xor r12, r12
	xor r13, r13
	xor r14, r14
	xor r15, r15

	 
	lea rcx, [rcx+120]
	movapd xmm8, xmmword ptr [rcx+72]
	movapd xmm9, xmmword ptr [rcx+88]
	movapd xmm10, xmmword ptr [rcx+104]
	movapd xmm11, xmmword ptr [rcx+120]
