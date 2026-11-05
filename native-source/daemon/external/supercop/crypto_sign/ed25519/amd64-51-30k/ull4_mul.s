
 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 
.text
.p2align 5
.globl _crypto_sign_ed25519_amd64_51_30k_batch_ull4_mul
.globl crypto_sign_ed25519_amd64_51_30k_batch_ull4_mul
_crypto_sign_ed25519_amd64_51_30k_batch_ull4_mul:
crypto_sign_ed25519_amd64_51_30k_batch_ull4_mul:
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

 
 
 
mov  %rdx,%rcx

 
 
 
mov  $0,%r8

 
 
 
mov  $0,%r9

 
 
 
mov  $0,%r10

 
 
 
mov  $0,%r11

 
 
 
mov  $0,%r12

 
 
 
movq   0(%rsi),%rax

 
 
 
mulq  0(%rcx)

 
 
 
mov  %rax,%r13

 
 
 
mov  %rdx,%r14

 
 
 
movq   0(%rsi),%rax

 
 
 
mulq  8(%rcx)

 
 
 
mov  %rax,%r15

 
 
 
add  %r14,%r15

 
 
 
mov  $0,%r14

 
 
 
adc %rdx,%r14

 
 
 
movq   0(%rsi),%rax

 
 
 
mulq  16(%rcx)

 
 
 
mov  %rax,%rbx

 
 
 
add  %r14,%rbx

 
 
 
mov  $0,%r14

 
 
 
adc %rdx,%r14

 
 
 
movq   0(%rsi),%rax

 
 
 
mulq  24(%rcx)

 
 
 
mov  %rax,%rbp

 
 
 
add  %r14,%rbp

 
 
 
adc %rdx,%r8

 
 
 
movq   8(%rsi),%rax

 
 
 
mulq  0(%rcx)

 
 
 
add  %rax,%r15

 
 
 
mov  $0,%r14

 
 
 
adc %rdx,%r14

 
 
 
movq   8(%rsi),%rax

 
 
 
mulq  8(%rcx)

 
 
 
add  %rax,%rbx

 
 
 
adc %r12,%rdx

 
 
 
add  %r14,%rbx

 
 
 
mov  $0,%r14

 
 
 
adc %rdx,%r14

 
 
 
movq   8(%rsi),%rax

 
 
 
mulq  16(%rcx)

 
 
 
add  %rax,%rbp

 
 
 
adc %r12,%rdx

 
 
 
add  %r14,%rbp

 
 
 
mov  $0,%r14

 
 
 
adc %rdx,%r14

 
 
 
movq   8(%rsi),%rax

 
 
 
mulq  24(%rcx)

 
 
 
add  %rax,%r8

 
 
 
adc %r12,%rdx

 
 
 
add  %r14,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq   16(%rsi),%rax

 
 
 
mulq  0(%rcx)

 
 
 
add  %rax,%rbx

 
 
 
mov  $0,%r14

 
 
 
adc %rdx,%r14

 
 
 
movq   16(%rsi),%rax

 
 
 
mulq  8(%rcx)

 
 
 
add  %rax,%rbp

 
 
 
adc %r12,%rdx

 
 
 
add  %r14,%rbp

 
 
 
mov  $0,%r14

 
 
 
adc %rdx,%r14

 
 
 
movq   16(%rsi),%rax

 
 
 
mulq  16(%rcx)

 
 
 
add  %rax,%r8

 
 
 
adc %r12,%rdx

 
 
 
add  %r14,%r8

 
 
 
mov  $0,%r14

 
 
 
adc %rdx,%r14

 
 
 
movq   16(%rsi),%rax

 
 
 
mulq  24(%rcx)

 
 
 
add  %rax,%r9

 
 
 
adc %r12,%rdx

 
 
 
add  %r14,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq   24(%rsi),%rax

 
 
 
mulq  0(%rcx)

 
 
 
add  %rax,%rbp

 
 
 
mov  $0,%r14

 
 
 
adc %rdx,%r14

 
 
 
movq   24(%rsi),%rax

 
 
 
mulq  8(%rcx)

 
 
 
add  %rax,%r8

 
 
 
adc %r12,%rdx

 
 
 
add  %r14,%r8

 
 
 
mov  $0,%r14

 
 
 
adc %rdx,%r14

 
 
 
movq   24(%rsi),%rax

 
 
 
mulq  16(%rcx)

 
 
 
add  %rax,%r9

 
 
 
adc %r12,%rdx

 
 
 
add  %r14,%r9

 
 
 
mov  $0,%r14

 
 
 
adc %rdx,%r14

 
 
 
movq   24(%rsi),%rax

 
 
 
mulq  24(%rcx)

 
 
 
add  %rax,%r10

 
 
 
adc %r12,%rdx

 
 
 
add  %r14,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq   %r13,0(%rdi)

 
 
 
movq   %r15,8(%rdi)

 
 
 
movq   %rbx,16(%rdi)

 
 
 
movq   %rbp,24(%rdi)

 
 
 
movq   %r8,32(%rdi)

 
 
 
movq   %r9,40(%rdi)

 
 
 
movq   %r10,48(%rdi)

 
 
 
movq   %r11,56(%rdi)

 
 
 
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
