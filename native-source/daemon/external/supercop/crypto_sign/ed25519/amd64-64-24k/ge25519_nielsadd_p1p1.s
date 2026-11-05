
 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 
.text
.p2align 5
.globl _crypto_sign_ed25519_amd64_64_ge25519_nielsadd_p1p1
.globl crypto_sign_ed25519_amd64_64_ge25519_nielsadd_p1p1
_crypto_sign_ed25519_amd64_64_ge25519_nielsadd_p1p1:
crypto_sign_ed25519_amd64_64_ge25519_nielsadd_p1p1:
mov %rsp,%r11
and $31,%r11
add $128,%r11
sub %r11,%rsp

 
 
 
movq %r11,0(%rsp)

 
 
 
movq %r12,8(%rsp)

 
 
 
movq %r13,16(%rsp)

 
 
 
movq %r14,24(%rsp)

 
 
 
movq %r15,32(%rsp)

 
 
 
movq %rbx,40(%rsp)

 
 
 
movq %rbp,48(%rsp)

 
 
 
mov  %rdx,%rcx

 
 
 
movq   32(%rsi),%rdx

 
 
 
movq   40(%rsi),%r8

 
 
 
movq   48(%rsi),%r9

 
 
 
movq   56(%rsi),%rax

 
 
 
mov  %rdx,%r10

 
 
 
mov  %r8,%r11

 
 
 
mov  %r9,%r12

 
 
 
mov  %rax,%r13

 
 
 
subq 0(%rsi),%rdx

 
 
 
sbbq 8(%rsi),%r8

 
 
 
sbbq 16(%rsi),%r9

 
 
 
sbbq 24(%rsi),%rax

 
 
 
mov  $0,%r14

 
 
 
mov  $38,%r15

 
 
 
cmovae %r14,%r15

 
 
 
sub  %r15,%rdx

 
 
 
sbb  %r14,%r8

 
 
 
sbb  %r14,%r9

 
 
 
sbb  %r14,%rax

 
 
 
cmovc %r15,%r14

 
 
 
sub  %r14,%rdx

 
 
 
addq 0(%rsi),%r10

 
 
 
adcq 8(%rsi),%r11

 
 
 
adcq 16(%rsi),%r12

 
 
 
adcq 24(%rsi),%r13

 
 
 
mov  $0,%r14

 
 
 
mov  $38,%r15

 
 
 
cmovae %r14,%r15

 
 
 
add  %r15,%r10

 
 
 
adc %r14,%r11

 
 
 
adc %r14,%r12

 
 
 
adc %r14,%r13

 
 
 
cmovc %r15,%r14

 
 
 
add  %r14,%r10

 
 
 
movq %rdx,56(%rsp)

 
 
 
movq %r8,64(%rsp)

 
 
 
movq %r9,72(%rsp)

 
 
 
movq %rax,80(%rsp)

 
 
 
movq %r10,88(%rsp)

 
 
 
movq %r11,96(%rsp)

 
 
 
movq %r12,104(%rsp)

 
 
 
movq %r13,112(%rsp)

 
 
 
mov  $0,%r8

 
 
 
mov  $0,%r9

 
 
 
mov  $0,%r10

 
 
 
mov  $0,%r11

 
 
 
movq 56(%rsp),%r12

 
 
 
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

 
 
 
movq 64(%rsp),%r12

 
 
 
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

 
 
 
movq 72(%rsp),%r12

 
 
 
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

 
 
 
movq 80(%rsp),%r12

 
 
 
movq   0(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%rbx

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   8(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r8

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   16(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r9

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r9

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   24(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r10

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r10

 
 
 
adc %rdx,%r11

 
 
 
mov  %r8,%rax

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
mov  %rax,%r8

 
 
 
mov  %r9,%rax

 
 
 
mov  %rdx,%r9

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r9

 
 
 
mov  %r10,%rax

 
 
 
mov  $0,%r10

 
 
 
adc %rdx,%r10

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r10

 
 
 
mov  %r11,%rax

 
 
 
mov  $0,%r11

 
 
 
adc %rdx,%r11

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r11

 
 
 
mov  $0,%rax

 
 
 
adc %rdx,%rax

 
 
 
add  %r8,%r13

 
 
 
adc %r9,%r14

 
 
 
adc %r10,%r15

 
 
 
adc %r11,%rbx

 
 
 
mov  $0,%rdx

 
 
 
adc %rdx,%rax

 
 
 
imulq  $38,%rax,%r8

 
 
 
add  %r8,%r13

 
 
 
adc %rdx,%r14

 
 
 
adc %rdx,%r15

 
 
 
adc %rdx,%rbx

 
 
 
adc %rdx,%rdx

 
 
 
imulq  $38,%rdx,%rdx

 
 
 
add  %rdx,%r13

 
 
 
movq %r13,56(%rsp)

 
 
 
movq %r14,64(%rsp)

 
 
 
movq %r15,72(%rsp)

 
 
 
movq %rbx,80(%rsp)

 
 
 
mov  $0,%r8

 
 
 
mov  $0,%r9

 
 
 
mov  $0,%r10

 
 
 
mov  $0,%r11

 
 
 
movq 88(%rsp),%r12

 
 
 
movq   32(%rcx),%rax

 
 
 
mul  %r12

 
 
 
mov  %rax,%r13

 
 
 
mov  %rdx,%r14

 
 
 
movq   40(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r14

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq   48(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r15

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   56(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%rbx

 
 
 
adc %rdx,%r8

 
 
 
movq 96(%rsp),%r12

 
 
 
movq   32(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r14

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   40(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r15

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r15

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   48(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%rbx

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%rbx

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   56(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 104(%rsp),%r12

 
 
 
movq   32(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r15

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   40(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%rbx

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%rbx

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   48(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r8

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   56(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r9

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq 112(%rsp),%r12

 
 
 
movq   32(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%rbx

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   40(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r8

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   48(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r9

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r9

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   56(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r10

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r10

 
 
 
adc %rdx,%r11

 
 
 
mov  %r8,%rax

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
mov  %rax,%r8

 
 
 
mov  %r9,%rax

 
 
 
mov  %rdx,%r9

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r9

 
 
 
mov  %r10,%rax

 
 
 
mov  $0,%r10

 
 
 
adc %rdx,%r10

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r10

 
 
 
mov  %r11,%rax

 
 
 
mov  $0,%r11

 
 
 
adc %rdx,%r11

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r11

 
 
 
mov  $0,%rax

 
 
 
adc %rdx,%rax

 
 
 
add  %r8,%r13

 
 
 
adc %r9,%r14

 
 
 
adc %r10,%r15

 
 
 
adc %r11,%rbx

 
 
 
mov  $0,%rdx

 
 
 
adc %rdx,%rax

 
 
 
imulq  $38,%rax,%r8

 
 
 
add  %r8,%r13

 
 
 
adc %rdx,%r14

 
 
 
adc %rdx,%r15

 
 
 
adc %rdx,%rbx

 
 
 
adc %rdx,%rdx

 
 
 
imulq  $38,%rdx,%rdx

 
 
 
add  %rdx,%r13

 
 
 
mov  %r13,%rdx

 
 
 
mov  %r14,%r8

 
 
 
mov  %r15,%r9

 
 
 
mov  %rbx,%rax

 
 
 
subq 56(%rsp),%r13

 
 
 
sbbq 64(%rsp),%r14

 
 
 
sbbq 72(%rsp),%r15

 
 
 
sbbq 80(%rsp),%rbx

 
 
 
mov  $0,%r10

 
 
 
mov  $38,%r11

 
 
 
cmovae %r10,%r11

 
 
 
sub  %r11,%r13

 
 
 
sbb  %r10,%r14

 
 
 
sbb  %r10,%r15

 
 
 
sbb  %r10,%rbx

 
 
 
cmovc %r11,%r10

 
 
 
sub  %r10,%r13

 
 
 
addq 56(%rsp),%rdx

 
 
 
adcq 64(%rsp),%r8

 
 
 
adcq 72(%rsp),%r9

 
 
 
adcq 80(%rsp),%rax

 
 
 
mov  $0,%r10

 
 
 
mov  $38,%r11

 
 
 
cmovae %r10,%r11

 
 
 
add  %r11,%rdx

 
 
 
adc %r10,%r8

 
 
 
adc %r10,%r9

 
 
 
adc %r10,%rax

 
 
 
cmovc %r11,%r10

 
 
 
add  %r10,%rdx

 
 
 
movq   %rdx,64(%rdi)

 
 
 
movq   %r8,72(%rdi)

 
 
 
movq   %r9,80(%rdi)

 
 
 
movq   %rax,88(%rdi)

 
 
 
movq   %r13,0(%rdi)

 
 
 
movq   %r14,8(%rdi)

 
 
 
movq   %r15,16(%rdi)

 
 
 
movq   %rbx,24(%rdi)

 
 
 
mov  $0,%r8

 
 
 
mov  $0,%r9

 
 
 
mov  $0,%r10

 
 
 
mov  $0,%r11

 
 
 
movq   96(%rsi),%r12

 
 
 
movq   64(%rcx),%rax

 
 
 
mul  %r12

 
 
 
mov  %rax,%r13

 
 
 
mov  %rdx,%r14

 
 
 
movq   72(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r14

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq   80(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r15

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   88(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%rbx

 
 
 
adc %rdx,%r8

 
 
 
movq   104(%rsi),%r12

 
 
 
movq   64(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r14

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   72(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r15

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r15

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   80(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%rbx

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%rbx

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   88(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq   112(%rsi),%r12

 
 
 
movq   64(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r15

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   72(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%rbx

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%rbx

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   80(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r8

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   88(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r9

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq   120(%rsi),%r12

 
 
 
movq   64(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%rbx

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   72(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r8

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   80(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r9

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r9

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   88(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r10

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r10

 
 
 
adc %rdx,%r11

 
 
 
mov  %r8,%rax

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
mov  %rax,%rcx

 
 
 
mov  %r9,%rax

 
 
 
mov  %rdx,%r8

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r8

 
 
 
mov  %r10,%rax

 
 
 
mov  $0,%r9

 
 
 
adc %rdx,%r9

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r9

 
 
 
mov  %r11,%rax

 
 
 
mov  $0,%r10

 
 
 
adc %rdx,%r10

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r10

 
 
 
mov  $0,%rax

 
 
 
adc %rdx,%rax

 
 
 
add  %rcx,%r13

 
 
 
adc %r8,%r14

 
 
 
adc %r9,%r15

 
 
 
adc %r10,%rbx

 
 
 
mov  $0,%rdx

 
 
 
adc %rdx,%rax

 
 
 
imulq  $38,%rax,%rcx

 
 
 
add  %rcx,%r13

 
 
 
adc %rdx,%r14

 
 
 
adc %rdx,%r15

 
 
 
adc %rdx,%rbx

 
 
 
adc %rdx,%rdx

 
 
 
imulq  $38,%rdx,%rdx

 
 
 
add  %rdx,%r13

 
 
 
movq   64(%rsi),%rdx

 
 
 
movq   72(%rsi),%rcx

 
 
 
movq   80(%rsi),%r8

 
 
 
movq   88(%rsi),%rsi

 
 
 
add  %rdx,%rdx

 
 
 
adc %rcx,%rcx

 
 
 
adc %r8,%r8

 
 
 
adc %rsi,%rsi

 
 
 
mov  $0,%r9

 
 
 
mov  $38,%rax

 
 
 
cmovae %r9,%rax

 
 
 
add  %rax,%rdx

 
 
 
adc %r9,%rcx

 
 
 
adc %r9,%r8

 
 
 
adc %r9,%rsi

 
 
 
cmovc %rax,%r9

 
 
 
add  %r9,%rdx

 
 
 
mov  %rdx,%r9

 
 
 
mov  %rcx,%rax

 
 
 
mov  %r8,%r10

 
 
 
mov  %rsi,%r11

 
 
 
sub  %r13,%rdx

 
 
 
sbb  %r14,%rcx

 
 
 
sbb  %r15,%r8

 
 
 
sbb  %rbx,%rsi

 
 
 
mov  $0,%r12

 
 
 
mov  $38,%rbp

 
 
 
cmovae %r12,%rbp

 
 
 
sub  %rbp,%rdx

 
 
 
sbb  %r12,%rcx

 
 
 
sbb  %r12,%r8

 
 
 
sbb  %r12,%rsi

 
 
 
cmovc %rbp,%r12

 
 
 
sub  %r12,%rdx

 
 
 
add  %r13,%r9

 
 
 
adc %r14,%rax

 
 
 
adc %r15,%r10

 
 
 
adc %rbx,%r11

 
 
 
mov  $0,%r12

 
 
 
mov  $38,%r13

 
 
 
cmovae %r12,%r13

 
 
 
add  %r13,%r9

 
 
 
adc %r12,%rax

 
 
 
adc %r12,%r10

 
 
 
adc %r12,%r11

 
 
 
cmovc %r13,%r12

 
 
 
add  %r12,%r9

 
 
 
movq   %r9,32(%rdi)

 
 
 
movq   %rax,40(%rdi)

 
 
 
movq   %r10,48(%rdi)

 
 
 
movq   %r11,56(%rdi)

 
 
 
movq   %rdx,96(%rdi)

 
 
 
movq   %rcx,104(%rdi)

 
 
 
movq   %r8,112(%rdi)

 
 
 
movq   %rsi,120(%rdi)

 
 
 
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
