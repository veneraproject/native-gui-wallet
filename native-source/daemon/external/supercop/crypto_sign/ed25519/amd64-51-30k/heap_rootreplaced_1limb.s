
 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 
.text
.p2align 5
.globl _crypto_sign_ed25519_amd64_51_30k_batch_heap_rootreplaced_1limb
.globl crypto_sign_ed25519_amd64_51_30k_batch_heap_rootreplaced_1limb
_crypto_sign_ed25519_amd64_51_30k_batch_heap_rootreplaced_1limb:
crypto_sign_ed25519_amd64_51_30k_batch_heap_rootreplaced_1limb:
mov %rsp,%r11
and $31,%r11
add $64,%r11
sub %r11,%rsp

 
 
 
movq %r11,0(%rsp)

 
 
 
movq %r12,8(%rsp)

 
 
 
movq %r13,16(%rsp)

 
 
 
movq %r14,24(%rsp)

 
 
 
movq %r15,32(%rsp)

 
 
 
movq %rbx,40(%rsp)

 
 
 
movq %rbp,48(%rsp)

 
 
 
mov  $0,%rcx

 
._siftdownloop:

 
 
 
mov  %rcx,%r8

 
 
 
imulq  $2,%r8,%r8

 
 
 
mov  %r8,%r9

 
 
 
add  $2,%r8

 
 
 
add  $1,%r9

 
 
 
cmp  %r8,%rsi
 

 
jbe ._siftuploop

 
 
 
movq   (%rdi,%r8,8),%rax

 
 
 
shl  $5,%rax

 
 
 
add  %rdx,%rax

 
 
 
movq   (%rdi,%r9,8),%r10

 
 
 
shl  $5,%r10

 
 
 
add  %rdx,%r10

 
 
 
movq   0(%r10),%r11

 
 
 
subq 0(%rax),%r11

 
 
 
cmovc %r8,%r9

 
 
 
cmovc %rax,%r10

 
 
 
sub  %rdx,%r10

 
 
 
shr  $5,%r10

 
 
 
movq   (%rdi,%rcx,8),%r8

 
 
 
movq  %r10,(%rdi,%rcx,8)

 
 
 
movq  %r8,(%rdi,%r9,8)

 
 
 
mov  %r9,%rcx
 

 
jmp ._siftdownloop

 
._siftuploop:

 
 
 
mov  %rcx,%rsi

 
 
 
sub  $1,%rcx

 
 
 
shr  $1,%rcx

 
 
 
cmp  $0,%rsi
 

 
jbe ._end

 
 
 
movq   (%rdi,%rcx,8),%r8

 
 
 
movq   (%rdi,%rsi,8),%r9

 
 
 
shl  $5,%r8

 
 
 
shl  $5,%r9

 
 
 
add  %rdx,%r9

 
 
 
add  %rdx,%r8

 
 
 
movq   0(%r9),%rax

 
 
 
subq 0(%r8),%rax
 

 
jc ._end

 
 
 
sub  %rdx,%r9

 
 
 
shr  $5,%r9

 
 
 
sub  %rdx,%r8

 
 
 
shr  $5,%r8

 
 
 
movq  %r9,(%rdi,%rcx,8)

 
 
 
movq  %r8,(%rdi,%rsi,8)
 

 
jmp ._siftuploop

 
._end:

 
 
 
movq 0(%rsp),%r11

 
 
 
movq 8(%rsp),%r12

 
 
 
movq 16(%rsp),%r13

 
 
 
movq 24(%rsp),%r14

 
 
 
movq 32(%rsp),%r15

 
 
 
movq 40(%rsp),%rbx

 
 
 
movq 48(%rsp),%rbp

 
add %r11,%rsp
mov %rdi,%rax
mov %rsi,%rdx
ret
