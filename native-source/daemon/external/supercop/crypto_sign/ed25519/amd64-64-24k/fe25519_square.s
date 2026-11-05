
 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 
.text
.p2align 5
.globl _crypto_sign_ed25519_amd64_64_fe25519_square
.globl crypto_sign_ed25519_amd64_64_fe25519_square
_crypto_sign_ed25519_amd64_64_fe25519_square:
crypto_sign_ed25519_amd64_64_fe25519_square:
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

 
 
 
movq   8(%rsi),%rax

 
 
 
mulq  0(%rsi)

 
 
 
mov  %rax,%r8

 
 
 
mov  %rdx,%r9

 
 
 
movq   16(%rsi),%rax

 
 
 
mulq  8(%rsi)

 
 
 
mov  %rax,%r10

 
 
 
mov  %rdx,%r11

 
 
 
movq   24(%rsi),%rax

 
 
 
mulq  16(%rsi)

 
 
 
mov  %rax,%r12

 
 
 
mov  %rdx,%r13

 
 
 
movq   16(%rsi),%rax

 
 
 
mulq  0(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
adc $0,%r11

 
 
 
movq   24(%rsi),%rax

 
 
 
mulq  8(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
adc $0,%r13

 
 
 
movq   24(%rsi),%rax

 
 
 
mulq  0(%rsi)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
adc $0,%r12

 
 
 
adc $0,%r13

 
 
 
adc $0,%rcx

 
 
 
add  %r8,%r8

 
 
 
adc %r9,%r9

 
 
 
adc %r10,%r10

 
 
 
adc %r11,%r11

 
 
 
adc %r12,%r12

 
 
 
adc %r13,%r13

 
 
 
adc %rcx,%rcx

 
 
 
movq   0(%rsi),%rax

 
 
 
mulq  0(%rsi)

 
 
 
mov  %rax,%r14

 
 
 
mov  %rdx,%r15

 
 
 
movq   8(%rsi),%rax

 
 
 
mulq  8(%rsi)

 
 
 
mov  %rax,%rbx

 
 
 
mov  %rdx,%rbp

 
 
 
movq   16(%rsi),%rax

 
 
 
mulq  16(%rsi)

 
 
 
add  %r15,%r8

 
 
 
adc %rbx,%r9

 
 
 
adc %rbp,%r10

 
 
 
adc %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
adc $0,%r13

 
 
 
adc $0,%rcx

 
 
 
movq   24(%rsi),%rax

 
 
 
mulq  24(%rsi)

 
 
 
add  %rax,%r13

 
 
 
adc %rdx,%rcx

 
 
 
mov  %r11,%rax

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
mov  %rax,%rsi

 
 
 
mov  %r12,%rax

 
 
 
mov  %rdx,%r11

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r11

 
 
 
mov  %r13,%rax

 
 
 
mov  $0,%r12

 
 
 
adc %rdx,%r12

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r12

 
 
 
mov  %rcx,%rax

 
 
 
mov  $0,%rcx

 
 
 
adc %rdx,%rcx

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%rcx

 
 
 
mov  $0,%rax

 
 
 
adc %rdx,%rax

 
 
 
add  %rsi,%r14

 
 
 
adc %r11,%r8

 
 
 
adc %r12,%r9

 
 
 
adc %rcx,%r10

 
 
 
mov  $0,%rsi

 
 
 
adc %rsi,%rax

 
 
 
imulq  $38,%rax,%rdx

 
 
 
add  %rdx,%r14

 
 
 
adc %rsi,%r8

 
 
 
adc %rsi,%r9

 
 
 
adc %rsi,%r10

 
 
 
adc %rsi,%rsi

 
 
 
imulq  $38,%rsi,%rsi

 
 
 
add  %rsi,%r14

 
 
 
movq   %r8,8(%rdi)

 
 
 
movq   %r9,16(%rdi)

 
 
 
movq   %r10,24(%rdi)

 
 
 
movq   %r14,0(%rdi)

 
 
 
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
