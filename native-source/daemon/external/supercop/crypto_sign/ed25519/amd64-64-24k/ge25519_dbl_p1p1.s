
 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 
.text
.p2align 5
.globl _crypto_sign_ed25519_amd64_64_ge25519_dbl_p1p1
.globl crypto_sign_ed25519_amd64_64_ge25519_dbl_p1p1
_crypto_sign_ed25519_amd64_64_ge25519_dbl_p1p1:
crypto_sign_ed25519_amd64_64_ge25519_dbl_p1p1:
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

 
 
 
mov  %rax,%r11

 
 
 
mov  %r12,%rax

 
 
 
mov  %rdx,%r12

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r12

 
 
 
mov  %r13,%rax

 
 
 
mov  $0,%r13

 
 
 
adc %rdx,%r13

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r13

 
 
 
mov  %rcx,%rax

 
 
 
mov  $0,%rcx

 
 
 
adc %rdx,%rcx

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%rcx

 
 
 
mov  $0,%rax

 
 
 
adc %rdx,%rax

 
 
 
add  %r11,%r14

 
 
 
adc %r12,%r8

 
 
 
adc %r13,%r9

 
 
 
adc %rcx,%r10

 
 
 
mov  $0,%rdx

 
 
 
adc %rdx,%rax

 
 
 
imulq  $38,%rax,%rcx

 
 
 
add  %rcx,%r14

 
 
 
adc %rdx,%r8

 
 
 
adc %rdx,%r9

 
 
 
adc %rdx,%r10

 
 
 
adc %rdx,%rdx

 
 
 
imulq  $38,%rdx,%rdx

 
 
 
add  %rdx,%r14

 
 
 
movq %r14,56(%rsp)

 
 
 
movq %r8,64(%rsp)

 
 
 
movq %r9,72(%rsp)

 
 
 
movq %r10,80(%rsp)

 
 
 
mov  $0,%rcx

 
 
 
movq   40(%rsi),%rax

 
 
 
mulq  32(%rsi)

 
 
 
mov  %rax,%r8

 
 
 
mov  %rdx,%r9

 
 
 
movq   48(%rsi),%rax

 
 
 
mulq  40(%rsi)

 
 
 
mov  %rax,%r10

 
 
 
mov  %rdx,%r11

 
 
 
movq   56(%rsi),%rax

 
 
 
mulq  48(%rsi)

 
 
 
mov  %rax,%r12

 
 
 
mov  %rdx,%r13

 
 
 
movq   48(%rsi),%rax

 
 
 
mulq  32(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
adc $0,%r11

 
 
 
movq   56(%rsi),%rax

 
 
 
mulq  40(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
adc $0,%r13

 
 
 
movq   56(%rsi),%rax

 
 
 
mulq  32(%rsi)

 
 
 
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

 
 
 
movq   32(%rsi),%rax

 
 
 
mulq  32(%rsi)

 
 
 
mov  %rax,%r14

 
 
 
mov  %rdx,%r15

 
 
 
movq   40(%rsi),%rax

 
 
 
mulq  40(%rsi)

 
 
 
mov  %rax,%rbx

 
 
 
mov  %rdx,%rbp

 
 
 
movq   48(%rsi),%rax

 
 
 
mulq  48(%rsi)

 
 
 
add  %r15,%r8

 
 
 
adc %rbx,%r9

 
 
 
adc %rbp,%r10

 
 
 
adc %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
adc $0,%r13

 
 
 
adc $0,%rcx

 
 
 
movq   56(%rsi),%rax

 
 
 
mulq  56(%rsi)

 
 
 
add  %rax,%r13

 
 
 
adc %rdx,%rcx

 
 
 
mov  %r11,%rax

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
mov  %rax,%r11

 
 
 
mov  %r12,%rax

 
 
 
mov  %rdx,%r12

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r12

 
 
 
mov  %r13,%rax

 
 
 
mov  $0,%r13

 
 
 
adc %rdx,%r13

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r13

 
 
 
mov  %rcx,%rax

 
 
 
mov  $0,%rcx

 
 
 
adc %rdx,%rcx

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%rcx

 
 
 
mov  $0,%rax

 
 
 
adc %rdx,%rax

 
 
 
add  %r11,%r14

 
 
 
adc %r12,%r8

 
 
 
adc %r13,%r9

 
 
 
adc %rcx,%r10

 
 
 
mov  $0,%rdx

 
 
 
adc %rdx,%rax

 
 
 
imulq  $38,%rax,%rcx

 
 
 
add  %rcx,%r14

 
 
 
adc %rdx,%r8

 
 
 
adc %rdx,%r9

 
 
 
adc %rdx,%r10

 
 
 
adc %rdx,%rdx

 
 
 
imulq  $38,%rdx,%rdx

 
 
 
add  %rdx,%r14

 
 
 
movq %r14,88(%rsp)

 
 
 
movq %r8,96(%rsp)

 
 
 
movq %r9,104(%rsp)

 
 
 
movq %r10,112(%rsp)

 
 
 
mov  $0,%rcx

 
 
 
movq   72(%rsi),%rax

 
 
 
mulq  64(%rsi)

 
 
 
mov  %rax,%r8

 
 
 
mov  %rdx,%r9

 
 
 
movq   80(%rsi),%rax

 
 
 
mulq  72(%rsi)

 
 
 
mov  %rax,%r10

 
 
 
mov  %rdx,%r11

 
 
 
movq   88(%rsi),%rax

 
 
 
mulq  80(%rsi)

 
 
 
mov  %rax,%r12

 
 
 
mov  %rdx,%r13

 
 
 
movq   80(%rsi),%rax

 
 
 
mulq  64(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
adc $0,%r11

 
 
 
movq   88(%rsi),%rax

 
 
 
mulq  72(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
adc $0,%r13

 
 
 
movq   88(%rsi),%rax

 
 
 
mulq  64(%rsi)

 
 
 
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

 
 
 
movq   64(%rsi),%rax

 
 
 
mulq  64(%rsi)

 
 
 
mov  %rax,%r14

 
 
 
mov  %rdx,%r15

 
 
 
movq   72(%rsi),%rax

 
 
 
mulq  72(%rsi)

 
 
 
mov  %rax,%rbx

 
 
 
mov  %rdx,%rbp

 
 
 
movq   80(%rsi),%rax

 
 
 
mulq  80(%rsi)

 
 
 
add  %r15,%r8

 
 
 
adc %rbx,%r9

 
 
 
adc %rbp,%r10

 
 
 
adc %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
adc $0,%r13

 
 
 
adc $0,%rcx

 
 
 
movq   88(%rsi),%rax

 
 
 
mulq  88(%rsi)

 
 
 
add  %rax,%r13

 
 
 
adc %rdx,%rcx

 
 
 
mov  %r11,%rax

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
mov  %rax,%r11

 
 
 
mov  %r12,%rax

 
 
 
mov  %rdx,%r12

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r12

 
 
 
mov  %r13,%rax

 
 
 
mov  $0,%r13

 
 
 
adc %rdx,%r13

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r13

 
 
 
mov  %rcx,%rax

 
 
 
mov  $0,%rcx

 
 
 
adc %rdx,%rcx

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%rcx

 
 
 
mov  $0,%rax

 
 
 
adc %rdx,%rax

 
 
 
add  %r11,%r14

 
 
 
adc %r12,%r8

 
 
 
adc %r13,%r9

 
 
 
adc %rcx,%r10

 
 
 
mov  $0,%rdx

 
 
 
adc %rdx,%rax

 
 
 
imulq  $38,%rax,%rcx

 
 
 
add  %rcx,%r14

 
 
 
adc %rdx,%r8

 
 
 
adc %rdx,%r9

 
 
 
adc %rdx,%r10

 
 
 
adc %rdx,%rdx

 
 
 
imulq  $38,%rdx,%rdx

 
 
 
add  %rdx,%r14

 
 
 
add  %r14,%r14

 
 
 
adc %r8,%r8

 
 
 
adc %r9,%r9

 
 
 
adc %r10,%r10

 
 
 
mov  $0,%rdx

 
 
 
mov  $38,%rcx

 
 
 
cmovae %rdx,%rcx

 
 
 
add  %rcx,%r14

 
 
 
adc %rdx,%r8

 
 
 
adc %rdx,%r9

 
 
 
adc %rdx,%r10

 
 
 
cmovc %rcx,%rdx

 
 
 
add  %rdx,%r14

 
 
 
movq %r14,120(%rsp)

 
 
 
movq %r8,128(%rsp)

 
 
 
movq %r9,136(%rsp)

 
 
 
movq %r10,144(%rsp)

 
 
 
mov  $0,%rdx

 
 
 
mov  $0,%rcx

 
 
 
mov  $0,%r8

 
 
 
mov  $0,%r9

 
 
 
subq 56(%rsp),%rdx

 
 
 
sbbq 64(%rsp),%rcx

 
 
 
sbbq 72(%rsp),%r8

 
 
 
sbbq 80(%rsp),%r9

 
 
 
mov  $0,%rax

 
 
 
mov  $38,%r10

 
 
 
cmovae %rax,%r10

 
 
 
sub  %r10,%rdx

 
 
 
sbb  %rax,%rcx

 
 
 
sbb  %rax,%r8

 
 
 
sbb  %rax,%r9

 
 
 
cmovc %r10,%rax

 
 
 
sub  %rax,%rdx

 
 
 
movq %rdx,56(%rsp)

 
 
 
movq %rcx,64(%rsp)

 
 
 
movq %r8,72(%rsp)

 
 
 
movq %r9,80(%rsp)

 
 
 
mov  $0,%rax

 
 
 
mov  $0,%r10

 
 
 
mov  $0,%r11

 
 
 
mov  $0,%r12

 
 
 
subq 88(%rsp),%rax

 
 
 
sbbq 96(%rsp),%r10

 
 
 
sbbq 104(%rsp),%r11

 
 
 
sbbq 112(%rsp),%r12

 
 
 
mov  $0,%r13

 
 
 
mov  $38,%r14

 
 
 
cmovae %r13,%r14

 
 
 
sub  %r14,%rax

 
 
 
sbb  %r13,%r10

 
 
 
sbb  %r13,%r11

 
 
 
sbb  %r13,%r12

 
 
 
cmovc %r14,%r13

 
 
 
sub  %r13,%rax

 
 
 
movq %rax,152(%rsp)

 
 
 
movq %r10,160(%rsp)

 
 
 
movq %r11,168(%rsp)

 
 
 
movq %r12,176(%rsp)

 
 
 
mov  %rdx,%rax

 
 
 
mov  %rcx,%r10

 
 
 
mov  %r8,%r11

 
 
 
mov  %r9,%r12

 
 
 
addq 88(%rsp),%rax

 
 
 
adcq 96(%rsp),%r10

 
 
 
adcq 104(%rsp),%r11

 
 
 
adcq 112(%rsp),%r12

 
 
 
mov  $0,%r13

 
 
 
mov  $38,%r14

 
 
 
cmovae %r13,%r14

 
 
 
add  %r14,%rax

 
 
 
adc %r13,%r10

 
 
 
adc %r13,%r11

 
 
 
adc %r13,%r12

 
 
 
cmovc %r14,%r13

 
 
 
add  %r13,%rax

 
 
 
movq   %rax,32(%rdi)

 
 
 
movq   %r10,40(%rdi)

 
 
 
movq   %r11,48(%rdi)

 
 
 
movq   %r12,56(%rdi)

 
 
 
subq 88(%rsp),%rdx

 
 
 
sbbq 96(%rsp),%rcx

 
 
 
sbbq 104(%rsp),%r8

 
 
 
sbbq 112(%rsp),%r9

 
 
 
mov  $0,%r13

 
 
 
mov  $38,%r14

 
 
 
cmovae %r13,%r14

 
 
 
sub  %r14,%rdx

 
 
 
sbb  %r13,%rcx

 
 
 
sbb  %r13,%r8

 
 
 
sbb  %r13,%r9

 
 
 
cmovc %r14,%r13

 
 
 
sub  %r13,%rdx

 
 
 
movq   %rdx,64(%rdi)

 
 
 
movq   %rcx,72(%rdi)

 
 
 
movq   %r8,80(%rdi)

 
 
 
movq   %r9,88(%rdi)

 
 
 
subq 120(%rsp),%rax

 
 
 
sbbq 128(%rsp),%r10

 
 
 
sbbq 136(%rsp),%r11

 
 
 
sbbq 144(%rsp),%r12

 
 
 
mov  $0,%rdx

 
 
 
mov  $38,%rcx

 
 
 
cmovae %rdx,%rcx

 
 
 
sub  %rcx,%rax

 
 
 
sbb  %rdx,%r10

 
 
 
sbb  %rdx,%r11

 
 
 
sbb  %rdx,%r12

 
 
 
cmovc %rcx,%rdx

 
 
 
sub  %rdx,%rax

 
 
 
movq   %rax,96(%rdi)

 
 
 
movq   %r10,104(%rdi)

 
 
 
movq   %r11,112(%rdi)

 
 
 
movq   %r12,120(%rdi)

 
 
 
movq   0(%rsi),%rdx

 
 
 
movq   8(%rsi),%rcx

 
 
 
movq   16(%rsi),%r8

 
 
 
movq   24(%rsi),%r9

 
 
 
addq 32(%rsi),%rdx

 
 
 
adcq 40(%rsi),%rcx

 
 
 
adcq 48(%rsi),%r8

 
 
 
adcq 56(%rsi),%r9

 
 
 
mov  $0,%rsi

 
 
 
mov  $38,%rax

 
 
 
cmovae %rsi,%rax

 
 
 
add  %rax,%rdx

 
 
 
adc %rsi,%rcx

 
 
 
adc %rsi,%r8

 
 
 
adc %rsi,%r9

 
 
 
cmovc %rax,%rsi

 
 
 
add  %rsi,%rdx

 
 
 
movq %rdx,88(%rsp)

 
 
 
movq %rcx,96(%rsp)

 
 
 
movq %r8,104(%rsp)

 
 
 
movq %r9,112(%rsp)

 
 
 
mov  $0,%rsi

 
 
 
movq 96(%rsp),%rax

 
 
 
mulq  88(%rsp)

 
 
 
mov  %rax,%rcx

 
 
 
mov  %rdx,%r8

 
 
 
movq 104(%rsp),%rax

 
 
 
mulq  96(%rsp)

 
 
 
mov  %rax,%r9

 
 
 
mov  %rdx,%r10

 
 
 
movq 112(%rsp),%rax

 
 
 
mulq  104(%rsp)

 
 
 
mov  %rax,%r11

 
 
 
mov  %rdx,%r12

 
 
 
movq 104(%rsp),%rax

 
 
 
mulq  88(%rsp)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
adc $0,%r10

 
 
 
movq 112(%rsp),%rax

 
 
 
mulq  96(%rsp)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
adc $0,%r12

 
 
 
movq 112(%rsp),%rax

 
 
 
mulq  88(%rsp)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
adc $0,%r11

 
 
 
adc $0,%r12

 
 
 
adc $0,%rsi

 
 
 
add  %rcx,%rcx

 
 
 
adc %r8,%r8

 
 
 
adc %r9,%r9

 
 
 
adc %r10,%r10

 
 
 
adc %r11,%r11

 
 
 
adc %r12,%r12

 
 
 
adc %rsi,%rsi

 
 
 
movq 88(%rsp),%rax

 
 
 
mulq  88(%rsp)

 
 
 
mov  %rax,%r13

 
 
 
mov  %rdx,%r14

 
 
 
movq 96(%rsp),%rax

 
 
 
mulq  96(%rsp)

 
 
 
mov  %rax,%r15

 
 
 
mov  %rdx,%rbx

 
 
 
movq 104(%rsp),%rax

 
 
 
mulq  104(%rsp)

 
 
 
add  %r14,%rcx

 
 
 
adc %r15,%r8

 
 
 
adc %rbx,%r9

 
 
 
adc %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
adc $0,%r12

 
 
 
adc $0,%rsi

 
 
 
movq 112(%rsp),%rax

 
 
 
mulq  112(%rsp)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%rsi

 
 
 
mov  %r10,%rax

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
mov  %rax,%r10

 
 
 
mov  %r11,%rax

 
 
 
mov  %rdx,%r11

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r11

 
 
 
mov  %r12,%rax

 
 
 
mov  $0,%r12

 
 
 
adc %rdx,%r12

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r12

 
 
 
mov  %rsi,%rax

 
 
 
mov  $0,%rsi

 
 
 
adc %rdx,%rsi

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%rsi

 
 
 
mov  $0,%rax

 
 
 
adc %rdx,%rax

 
 
 
add  %r10,%r13

 
 
 
adc %r11,%rcx

 
 
 
adc %r12,%r8

 
 
 
adc %rsi,%r9

 
 
 
mov  $0,%rsi

 
 
 
adc %rsi,%rax

 
 
 
imulq  $38,%rax,%rdx

 
 
 
add  %rdx,%r13

 
 
 
adc %rsi,%rcx

 
 
 
adc %rsi,%r8

 
 
 
adc %rsi,%r9

 
 
 
adc %rsi,%rsi

 
 
 
imulq  $38,%rsi,%rsi

 
 
 
add  %rsi,%r13

 
 
 
addq 56(%rsp),%r13

 
 
 
adcq 64(%rsp),%rcx

 
 
 
adcq 72(%rsp),%r8

 
 
 
adcq 80(%rsp),%r9

 
 
 
mov  $0,%rsi

 
 
 
mov  $38,%rdx

 
 
 
cmovae %rsi,%rdx

 
 
 
add  %rdx,%r13

 
 
 
adc %rsi,%rcx

 
 
 
adc %rsi,%r8

 
 
 
adc %rsi,%r9

 
 
 
cmovc %rdx,%rsi

 
 
 
add  %rsi,%r13

 
 
 
addq 152(%rsp),%r13

 
 
 
adcq 160(%rsp),%rcx

 
 
 
adcq 168(%rsp),%r8

 
 
 
adcq 176(%rsp),%r9

 
 
 
mov  $0,%rsi

 
 
 
mov  $38,%rdx

 
 
 
cmovae %rsi,%rdx

 
 
 
add  %rdx,%r13

 
 
 
adc %rsi,%rcx

 
 
 
adc %rsi,%r8

 
 
 
adc %rsi,%r9

 
 
 
cmovc %rdx,%rsi

 
 
 
add  %rsi,%r13

 
 
 
movq   %r13,0(%rdi)

 
 
 
movq   %rcx,8(%rdi)

 
 
 
movq   %r8,16(%rdi)

 
 
 
movq   %r9,24(%rdi)

 
 
 
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
