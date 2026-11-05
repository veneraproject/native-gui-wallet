
 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 
.text
.p2align 5
.globl _crypto_sign_ed25519_amd64_64_fe25519_mul
.globl crypto_sign_ed25519_amd64_64_fe25519_mul
_crypto_sign_ed25519_amd64_64_fe25519_mul:
crypto_sign_ed25519_amd64_64_fe25519_mul:
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

 
 
 
movq   0(%rsi),%r12

 
 
 
movq   0(%rcx),%rax

 
 
 
mul  %r12

 
 
 
mov  %rax,%r13

 
 
 
mov  %rdx,%r14

 
 
 
movq   8(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r14

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq   16(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r15

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   24(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%rbx

 
 
 
adc %rdx,%r8

 
 
 
movq   8(%rsi),%r12

 
 
 
movq   0(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r14

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   8(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r15

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r15

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   16(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%rbx

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%rbx

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   24(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq   16(%rsi),%r12

 
 
 
movq   0(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r15

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   8(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%rbx

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%rbx

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   16(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r8

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   24(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r9

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq   24(%rsi),%rsi

 
 
 
movq   0(%rcx),%rax

 
 
 
mul  %rsi

 
 
 
add  %rax,%rbx

 
 
 
mov  $0,%r12

 
 
 
adc %rdx,%r12

 
 
 
movq   8(%rcx),%rax

 
 
 
mul  %rsi

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %r12,%r8

 
 
 
mov  $0,%r12

 
 
 
adc %rdx,%r12

 
 
 
movq   16(%rcx),%rax

 
 
 
mul  %rsi

 
 
 
add  %rax,%r9

 
 
 
adc $0,%rdx

 
 
 
add  %r12,%r9

 
 
 
mov  $0,%r12

 
 
 
adc %rdx,%r12

 
 
 
movq   24(%rcx),%rax

 
 
 
mul  %rsi

 
 
 
add  %rax,%r10

 
 
 
adc $0,%rdx

 
 
 
add  %r12,%r10

 
 
 
adc %rdx,%r11

 
 
 
mov  %r8,%rax

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
mov  %rax,%rsi

 
 
 
mov  %r9,%rax

 
 
 
mov  %rdx,%rcx

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%rcx

 
 
 
mov  %r10,%rax

 
 
 
mov  $0,%r8

 
 
 
adc %rdx,%r8

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r8

 
 
 
mov  %r11,%rax

 
 
 
mov  $0,%r9

 
 
 
adc %rdx,%r9

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r9

 
 
 
mov  $0,%rax

 
 
 
adc %rdx,%rax

 
 
 
add  %rsi,%r13

 
 
 
adc %rcx,%r14

 
 
 
adc %r8,%r15

 
 
 
adc %r9,%rbx

 
 
 
mov  $0,%rsi

 
 
 
adc %rsi,%rax

 
 
 
imulq  $38,%rax,%rdx

 
 
 
add  %rdx,%r13

 
 
 
adc %rsi,%r14

 
 
 
adc %rsi,%r15

 
 
 
adc %rsi,%rbx

 
 
 
adc %rsi,%rsi

 
 
 
imulq  $38,%rsi,%rsi

 
 
 
add  %rsi,%r13

 
 
 
movq   %r14,8(%rdi)

 
 
 
movq   %r15,16(%rdi)

 
 
 
movq   %rbx,24(%rdi)

 
 
 
movq   %r13,0(%rdi)

 
 
 
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
