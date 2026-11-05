	sub rsp, 72
	mov qword ptr [rsp+64], rbx
	mov qword ptr [rsp+56], r8
	mov qword ptr [rsp+48], r9
	mov qword ptr [rsp+40], r10
	mov qword ptr [rsp+32], r11
	mov qword ptr [rsp+24], r12
	mov qword ptr [rsp+16], r13
	mov qword ptr [rsp+8], r14
	mov qword ptr [rsp+0], r15
	ror rbp, 32                         
	xor rbp, rax                        
	mov rbx, rbp                        
	shr rbx, 38
	and ebx, RANDOMX_DATASET_BASE_MASK / 64  
	 
	 