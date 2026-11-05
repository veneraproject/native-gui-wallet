
 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 
.text
.p2align 5
.globl _crypto_sign_ed25519_amd64_64_ge25519_add_p1p1
.globl crypto_sign_ed25519_amd64_64_ge25519_add_p1p1
_crypto_sign_ed25519_amd64_64_ge25519_add_p1p1:
crypto_sign_ed25519_amd64_64_ge25519_add_p1p1:
mov %rsp,%r11
and $31,%r11
add $192,%r11
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

 
 
 
movq   32(%rcx),%rdx

 
 
 
movq   40(%rcx),%r8

 
 
 
movq   48(%rcx),%r9

 
 
 
movq   56(%rcx),%rax

 
 
 
mov  %rdx,%r10

 
 
 
mov  %r8,%r11

 
 
 
mov  %r9,%r12

 
 
 
mov  %rax,%r13

 
 
 
subq 0(%rcx),%rdx

 
 
 
sbbq 8(%rcx),%r8

 
 
 
sbbq 16(%rcx),%r9

 
 
 
sbbq 24(%rcx),%rax

 
 
 
mov  $0,%r14

 
 
 
mov  $38,%r15

 
 
 
cmovae %r14,%r15

 
 
 
sub  %r15,%rdx

 
 
 
sbb  %r14,%r8

 
 
 
sbb  %r14,%r9

 
 
 
sbb  %r14,%rax

 
 
 
cmovc %r15,%r14

 
 
 
sub  %r14,%rdx

 
 
 
addq 0(%rcx),%r10

 
 
 
adcq 8(%rcx),%r11

 
 
 
adcq 16(%rcx),%r12

 
 
 
adcq 24(%rcx),%r13

 
 
 
mov  $0,%r14

 
 
 
mov  $38,%r15

 
 
 
cmovae %r14,%r15

 
 
 
add  %r15,%r10

 
 
 
adc %r14,%r11

 
 
 
adc %r14,%r12

 
 
 
adc %r14,%r13

 
 
 
cmovc %r15,%r14

 
 
 
add  %r14,%r10

 
 
 
movq %rdx,120(%rsp)

 
 
 
movq %r8,128(%rsp)

 
 
 
movq %r9,136(%rsp)

 
 
 
movq %rax,144(%rsp)

 
 
 
movq %r10,152(%rsp)

 
 
 
movq %r11,160(%rsp)

 
 
 
movq %r12,168(%rsp)

 
 
 
movq %r13,176(%rsp)

 
 
 
mov  $0,%r8

 
 
 
mov  $0,%r9

 
 
 
mov  $0,%r10

 
 
 
mov  $0,%r11

 
 
 
movq 56(%rsp),%r12

 
 
 
movq 120(%rsp),%rax

 
 
 
mul  %r12

 
 
 
mov  %rax,%r13

 
 
 
mov  %rdx,%r14

 
 
 
movq 128(%rsp),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r14

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 136(%rsp),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r15

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq 144(%rsp),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%rbx

 
 
 
adc %rdx,%r8

 
 
 
movq 64(%rsp),%r12

 
 
 
movq 120(%rsp),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r14

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq 128(%rsp),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r15

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r15

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq 136(%rsp),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%rbx

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%rbx

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq 144(%rsp),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 72(%rsp),%r12

 
 
 
movq 120(%rsp),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r15

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq 128(%rsp),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%rbx

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%rbx

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq 136(%rsp),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r8

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq 144(%rsp),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r9

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq 80(%rsp),%r12

 
 
 
movq 120(%rsp),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%rbx

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq 128(%rsp),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r8

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq 136(%rsp),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r9

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r9

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq 144(%rsp),%rax

 
 
 
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

 
 
 
movq 152(%rsp),%rax

 
 
 
mul  %r12

 
 
 
mov  %rax,%r13

 
 
 
mov  %rdx,%r14

 
 
 
movq 160(%rsp),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r14

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 168(%rsp),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r15

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq 176(%rsp),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%rbx

 
 
 
adc %rdx,%r8

 
 
 
movq 96(%rsp),%r12

 
 
 
movq 152(%rsp),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r14

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq 160(%rsp),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r15

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r15

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq 168(%rsp),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%rbx

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%rbx

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq 176(%rsp),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 104(%rsp),%r12

 
 
 
movq 152(%rsp),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r15

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq 160(%rsp),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%rbx

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%rbx

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq 168(%rsp),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r8

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq 176(%rsp),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r9

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq 112(%rsp),%r12

 
 
 
movq 152(%rsp),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%rbx

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq 160(%rsp),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r8

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq 168(%rsp),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r9

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r9

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq 176(%rsp),%rax

 
 
 
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

 
 
 
movq   %r13,0(%rdi)

 
 
 
movq   %r14,8(%rdi)

 
 
 
movq   %r15,16(%rdi)

 
 
 
movq   %rbx,24(%rdi)

 
 
 
movq   %rdx,64(%rdi)

 
 
 
movq   %r8,72(%rdi)

 
 
 
movq   %r9,80(%rdi)

 
 
 
movq   %rax,88(%rdi)

 
 
 
mov  $0,%r8

 
 
 
mov  $0,%r9

 
 
 
mov  $0,%r10

 
 
 
mov  $0,%r11

 
 
 
movq   96(%rsi),%r12

 
 
 
movq   96(%rcx),%rax

 
 
 
mul  %r12

 
 
 
mov  %rax,%r13

 
 
 
mov  %rdx,%r14

 
 
 
movq   104(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r14

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq   112(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r15

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   120(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%rbx

 
 
 
adc %rdx,%r8

 
 
 
movq   104(%rsi),%r12

 
 
 
movq   96(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r14

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   104(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r15

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r15

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   112(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%rbx

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%rbx

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   120(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq   112(%rsi),%r12

 
 
 
movq   96(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r15

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   104(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%rbx

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%rbx

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   112(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r8

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   120(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r9

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq   120(%rsi),%r12

 
 
 
movq   96(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%rbx

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   104(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r8

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   112(%rcx),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r9

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r9

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq   120(%rcx),%rax

 
 
 
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

 
 
 
movq 56(%rsp),%r12

 
 
 
movq crypto_sign_ed25519_amd64_64_EC2D0(%rip),%rax

 
 
 
mul  %r12

 
 
 
mov  %rax,%r13

 
 
 
mov  %rdx,%r14

 
 
 
movq crypto_sign_ed25519_amd64_64_EC2D1(%rip),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r14

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq crypto_sign_ed25519_amd64_64_EC2D2(%rip),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r15

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq crypto_sign_ed25519_amd64_64_EC2D3(%rip),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%rbx

 
 
 
adc %rdx,%r8

 
 
 
movq 64(%rsp),%r12

 
 
 
movq crypto_sign_ed25519_amd64_64_EC2D0(%rip),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r14

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq crypto_sign_ed25519_amd64_64_EC2D1(%rip),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r15

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r15

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq crypto_sign_ed25519_amd64_64_EC2D2(%rip),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%rbx

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%rbx

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq crypto_sign_ed25519_amd64_64_EC2D3(%rip),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 72(%rsp),%r12

 
 
 
movq crypto_sign_ed25519_amd64_64_EC2D0(%rip),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r15

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq crypto_sign_ed25519_amd64_64_EC2D1(%rip),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%rbx

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%rbx

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq crypto_sign_ed25519_amd64_64_EC2D2(%rip),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r8

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq crypto_sign_ed25519_amd64_64_EC2D3(%rip),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r9

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq 80(%rsp),%r12

 
 
 
movq crypto_sign_ed25519_amd64_64_EC2D0(%rip),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%rbx

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq crypto_sign_ed25519_amd64_64_EC2D1(%rip),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r8

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq crypto_sign_ed25519_amd64_64_EC2D2(%rip),%rax

 
 
 
mul  %r12

 
 
 
add  %rax,%r9

 
 
 
adc $0,%rdx

 
 
 
add  %rbp,%r9

 
 
 
mov  $0,%rbp

 
 
 
adc %rdx,%rbp

 
 
 
movq crypto_sign_ed25519_amd64_64_EC2D3(%rip),%rax

 
 
 
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

 
 
 
movq   64(%rsi),%r12

 
 
 
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

 
 
 
movq   72(%rsi),%r12

 
 
 
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

 
 
 
movq   80(%rsi),%r12

 
 
 
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

 
 
 
movq   88(%rsi),%rsi

 
 
 
movq   64(%rcx),%rax

 
 
 
mul  %rsi

 
 
 
add  %rax,%rbx

 
 
 
mov  $0,%r12

 
 
 
adc %rdx,%r12

 
 
 
movq   72(%rcx),%rax

 
 
 
mul  %rsi

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %r12,%r8

 
 
 
mov  $0,%r12

 
 
 
adc %rdx,%r12

 
 
 
movq   80(%rcx),%rax

 
 
 
mul  %rsi

 
 
 
add  %rax,%r9

 
 
 
adc $0,%rdx

 
 
 
add  %r12,%r9

 
 
 
mov  $0,%r12

 
 
 
adc %rdx,%r12

 
 
 
movq   88(%rcx),%rax

 
 
 
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

 
 
 
add  %r13,%r13

 
 
 
adc %r14,%r14

 
 
 
adc %r15,%r15

 
 
 
adc %rbx,%rbx

 
 
 
mov  $0,%rsi

 
 
 
mov  $38,%rdx

 
 
 
cmovae %rsi,%rdx

 
 
 
add  %rdx,%r13

 
 
 
adc %rsi,%r14

 
 
 
adc %rsi,%r15

 
 
 
adc %rsi,%rbx

 
 
 
cmovc %rdx,%rsi

 
 
 
add  %rsi,%r13

 
 
 
mov  %r13,%rsi

 
 
 
mov  %r14,%rdx

 
 
 
mov  %r15,%rcx

 
 
 
mov  %rbx,%r8

 
 
 
addq 56(%rsp),%rsi

 
 
 
adcq 64(%rsp),%rdx

 
 
 
adcq 72(%rsp),%rcx

 
 
 
adcq 80(%rsp),%r8

 
 
 
mov  $0,%r9

 
 
 
mov  $38,%rax

 
 
 
cmovae %r9,%rax

 
 
 
add  %rax,%rsi

 
 
 
adc %r9,%rdx

 
 
 
adc %r9,%rcx

 
 
 
adc %r9,%r8

 
 
 
cmovc %rax,%r9

 
 
 
add  %r9,%rsi

 
 
 
subq 56(%rsp),%r13

 
 
 
sbbq 64(%rsp),%r14

 
 
 
sbbq 72(%rsp),%r15

 
 
 
sbbq 80(%rsp),%rbx

 
 
 
mov  $0,%r9

 
 
 
mov  $38,%rax

 
 
 
cmovae %r9,%rax

 
 
 
sub  %rax,%r13

 
 
 
sbb  %r9,%r14

 
 
 
sbb  %r9,%r15

 
 
 
sbb  %r9,%rbx

 
 
 
cmovc %rax,%r9

 
 
 
sub  %r9,%r13

 
 
 
movq   %rsi,32(%rdi)

 
 
 
movq   %rdx,40(%rdi)

 
 
 
movq   %rcx,48(%rdi)

 
 
 
movq   %r8,56(%rdi)

 
 
 
movq   %r13,96(%rdi)

 
 
 
movq   %r14,104(%rdi)

 
 
 
movq   %r15,112(%rdi)

 
 
 
movq   %rbx,120(%rdi)

 
 
 
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
