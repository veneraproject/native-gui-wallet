
 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 
.text
.p2align 5
.globl _crypto_sign_ed25519_amd64_64_ge25519_nielsadd2
.globl crypto_sign_ed25519_amd64_64_ge25519_nielsadd2
_crypto_sign_ed25519_amd64_64_ge25519_nielsadd2:
crypto_sign_ed25519_amd64_64_ge25519_nielsadd2:
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

 
 
 
movq   32(%rdi),%rdx

 
 
 
movq   40(%rdi),%rcx

 
 
 
movq   48(%rdi),%r8

 
 
 
movq   56(%rdi),%r9

 
 
 
mov  %rdx,%rax

 
 
 
mov  %rcx,%r10

 
 
 
mov  %r8,%r11

 
 
 
mov  %r9,%r12

 
 
 
subq 0(%rdi),%rdx

 
 
 
sbbq 8(%rdi),%rcx

 
 
 
sbbq 16(%rdi),%r8

 
 
 
sbbq 24(%rdi),%r9

 
 
 
mov  $0,%r13

 
 
 
mov  $38,%r14

 
 
 
cmovae %r13,%r14

 
 
 
sub  %r14,%rdx

 
 
 
sbb  %r13,%rcx

 
 
 
sbb  %r13,%r8

 
 
 
sbb  %r13,%r9

 
 
 
cmovc %r14,%r13

 
 
 
sub  %r13,%rdx

 
 
 
addq 0(%rdi),%rax

 
 
 
adcq 8(%rdi),%r10

 
 
 
adcq 16(%rdi),%r11

 
 
 
adcq 24(%rdi),%r12

 
 
 
mov  $0,%r13

 
 
 
mov  $38,%r14

 
 
 
cmovae %r13,%r14

 
 
 
add  %r14,%rax

 
 
 
adc %r13,%r10

 
 
 
adc %r13,%r11

 
 
 
adc %r13,%r12

 
 
 
cmovc %r14,%r13

 
 
 
add  %r13,%rax

 
 
 
movq %rdx,56(%rsp)

 
 
 
movq %rcx,64(%rsp)

 
 
 
movq %r8,72(%rsp)

 
 
 
movq %r9,80(%rsp)

 
 
 
movq %rax,88(%rsp)

 
 
 
movq %r10,96(%rsp)

 
 
 
movq %r11,104(%rsp)

 
 
 
movq %r12,112(%rsp)

 
 
 
mov  $0,%rcx

 
 
 
mov  $0,%r8

 
 
 
mov  $0,%r9

 
 
 
mov  $0,%r10

 
 
 
movq 56(%rsp),%r11

 
 
 
movq   0(%rsi),%rax

 
 
 
mul  %r11

 
 
 
mov  %rax,%r12

 
 
 
mov  %rdx,%r13

 
 
 
movq   8(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r13

 
 
 
mov  $0,%r14

 
 
 
adc %rdx,%r14

 
 
 
movq   16(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r14

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq   24(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rcx

 
 
 
movq 64(%rsp),%r11

 
 
 
movq   0(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r13

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   8(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r14

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%r14

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   16(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r15

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%r15

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   24(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%rcx

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq 72(%rsp),%r11

 
 
 
movq   0(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r14

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   8(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r15

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%r15

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   16(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%rcx

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%rcx

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   24(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 80(%rsp),%r11

 
 
 
movq   0(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r15

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   8(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%rcx

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%rcx

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   16(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%r8

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   24(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r9

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%r9

 
 
 
adc %rdx,%r10

 
 
 
mov  %rcx,%rax

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
mov  %rax,%rcx

 
 
 
mov  %r8,%rax

 
 
 
mov  %rdx,%r8

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r8

 
 
 
mov  %r9,%rax

 
 
 
mov  $0,%r9

 
 
 
adc %rdx,%r9

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r9

 
 
 
mov  %r10,%rax

 
 
 
mov  $0,%r10

 
 
 
adc %rdx,%r10

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r10

 
 
 
mov  $0,%rax

 
 
 
adc %rdx,%rax

 
 
 
add  %rcx,%r12

 
 
 
adc %r8,%r13

 
 
 
adc %r9,%r14

 
 
 
adc %r10,%r15

 
 
 
mov  $0,%rdx

 
 
 
adc %rdx,%rax

 
 
 
imulq  $38,%rax,%rcx

 
 
 
add  %rcx,%r12

 
 
 
adc %rdx,%r13

 
 
 
adc %rdx,%r14

 
 
 
adc %rdx,%r15

 
 
 
adc %rdx,%rdx

 
 
 
imulq  $38,%rdx,%rdx

 
 
 
add  %rdx,%r12

 
 
 
movq %r12,56(%rsp)

 
 
 
movq %r13,64(%rsp)

 
 
 
movq %r14,72(%rsp)

 
 
 
movq %r15,80(%rsp)

 
 
 
mov  $0,%rcx

 
 
 
mov  $0,%r8

 
 
 
mov  $0,%r9

 
 
 
mov  $0,%r10

 
 
 
movq 88(%rsp),%r11

 
 
 
movq   32(%rsi),%rax

 
 
 
mul  %r11

 
 
 
mov  %rax,%r12

 
 
 
mov  %rdx,%r13

 
 
 
movq   40(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r13

 
 
 
mov  $0,%r14

 
 
 
adc %rdx,%r14

 
 
 
movq   48(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r14

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq   56(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rcx

 
 
 
movq 96(%rsp),%r11

 
 
 
movq   32(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r13

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   40(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r14

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%r14

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   48(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r15

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%r15

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   56(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%rcx

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq 104(%rsp),%r11

 
 
 
movq   32(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r14

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   40(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r15

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%r15

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   48(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%rcx

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%rcx

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   56(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 112(%rsp),%r11

 
 
 
movq   32(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r15

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   40(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%rcx

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%rcx

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   48(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%r8

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   56(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r9

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%r9

 
 
 
adc %rdx,%r10

 
 
 
mov  %rcx,%rax

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
mov  %rax,%rcx

 
 
 
mov  %r8,%rax

 
 
 
mov  %rdx,%r8

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r8

 
 
 
mov  %r9,%rax

 
 
 
mov  $0,%r9

 
 
 
adc %rdx,%r9

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r9

 
 
 
mov  %r10,%rax

 
 
 
mov  $0,%r10

 
 
 
adc %rdx,%r10

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r10

 
 
 
mov  $0,%rax

 
 
 
adc %rdx,%rax

 
 
 
add  %rcx,%r12

 
 
 
adc %r8,%r13

 
 
 
adc %r9,%r14

 
 
 
adc %r10,%r15

 
 
 
mov  $0,%rdx

 
 
 
adc %rdx,%rax

 
 
 
imulq  $38,%rax,%rcx

 
 
 
add  %rcx,%r12

 
 
 
adc %rdx,%r13

 
 
 
adc %rdx,%r14

 
 
 
adc %rdx,%r15

 
 
 
adc %rdx,%rdx

 
 
 
imulq  $38,%rdx,%rdx

 
 
 
add  %rdx,%r12

 
 
 
mov  %r12,%rdx

 
 
 
mov  %r13,%rcx

 
 
 
mov  %r14,%r8

 
 
 
mov  %r15,%r9

 
 
 
subq 56(%rsp),%r12

 
 
 
sbbq 64(%rsp),%r13

 
 
 
sbbq 72(%rsp),%r14

 
 
 
sbbq 80(%rsp),%r15

 
 
 
mov  $0,%rax

 
 
 
mov  $38,%r10

 
 
 
cmovae %rax,%r10

 
 
 
sub  %r10,%r12

 
 
 
sbb  %rax,%r13

 
 
 
sbb  %rax,%r14

 
 
 
sbb  %rax,%r15

 
 
 
cmovc %r10,%rax

 
 
 
sub  %rax,%r12

 
 
 
addq 56(%rsp),%rdx

 
 
 
adcq 64(%rsp),%rcx

 
 
 
adcq 72(%rsp),%r8

 
 
 
adcq 80(%rsp),%r9

 
 
 
mov  $0,%rax

 
 
 
mov  $38,%r10

 
 
 
cmovae %rax,%r10

 
 
 
add  %r10,%rdx

 
 
 
adc %rax,%rcx

 
 
 
adc %rax,%r8

 
 
 
adc %rax,%r9

 
 
 
cmovc %r10,%rax

 
 
 
add  %rax,%rdx

 
 
 
movq %rdx,56(%rsp)

 
 
 
movq %rcx,64(%rsp)

 
 
 
movq %r8,72(%rsp)

 
 
 
movq %r9,80(%rsp)

 
 
 
movq %r12,88(%rsp)

 
 
 
movq %r13,96(%rsp)

 
 
 
movq %r14,104(%rsp)

 
 
 
movq %r15,112(%rsp)

 
 
 
mov  $0,%rcx

 
 
 
mov  $0,%r8

 
 
 
mov  $0,%r9

 
 
 
mov  $0,%r10

 
 
 
movq   96(%rdi),%r11

 
 
 
movq   64(%rsi),%rax

 
 
 
mul  %r11

 
 
 
mov  %rax,%r12

 
 
 
mov  %rdx,%r13

 
 
 
movq   72(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r13

 
 
 
mov  $0,%r14

 
 
 
adc %rdx,%r14

 
 
 
movq   80(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r14

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq   88(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rcx

 
 
 
movq   104(%rdi),%r11

 
 
 
movq   64(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r13

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   72(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r14

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%r14

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   80(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r15

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%r15

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   88(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%rcx

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq   112(%rdi),%r11

 
 
 
movq   64(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r14

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   72(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r15

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%r15

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   80(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%rcx

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%rcx

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   88(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq   120(%rdi),%r11

 
 
 
movq   64(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r15

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   72(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%rcx

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%rcx

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   80(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%r8

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   88(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r9

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%r9

 
 
 
adc %rdx,%r10

 
 
 
mov  %rcx,%rax

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
mov  %rax,%rsi

 
 
 
mov  %r8,%rax

 
 
 
mov  %rdx,%rcx

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%rcx

 
 
 
mov  %r9,%rax

 
 
 
mov  $0,%r8

 
 
 
adc %rdx,%r8

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r8

 
 
 
mov  %r10,%rax

 
 
 
mov  $0,%r9

 
 
 
adc %rdx,%r9

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r9

 
 
 
mov  $0,%rax

 
 
 
adc %rdx,%rax

 
 
 
add  %rsi,%r12

 
 
 
adc %rcx,%r13

 
 
 
adc %r8,%r14

 
 
 
adc %r9,%r15

 
 
 
mov  $0,%rsi

 
 
 
adc %rsi,%rax

 
 
 
imulq  $38,%rax,%rdx

 
 
 
add  %rdx,%r12

 
 
 
adc %rsi,%r13

 
 
 
adc %rsi,%r14

 
 
 
adc %rsi,%r15

 
 
 
adc %rsi,%rsi

 
 
 
imulq  $38,%rsi,%rsi

 
 
 
add  %rsi,%r12

 
 
 
movq   64(%rdi),%rsi

 
 
 
movq   72(%rdi),%rdx

 
 
 
movq   80(%rdi),%rcx

 
 
 
movq   88(%rdi),%r8

 
 
 
add  %rsi,%rsi

 
 
 
adc %rdx,%rdx

 
 
 
adc %rcx,%rcx

 
 
 
adc %r8,%r8

 
 
 
mov  $0,%r9

 
 
 
mov  $38,%rax

 
 
 
cmovae %r9,%rax

 
 
 
add  %rax,%rsi

 
 
 
adc %r9,%rdx

 
 
 
adc %r9,%rcx

 
 
 
adc %r9,%r8

 
 
 
cmovc %rax,%r9

 
 
 
add  %r9,%rsi

 
 
 
mov  %rsi,%r9

 
 
 
mov  %rdx,%rax

 
 
 
mov  %rcx,%r10

 
 
 
mov  %r8,%r11

 
 
 
sub  %r12,%rsi

 
 
 
sbb  %r13,%rdx

 
 
 
sbb  %r14,%rcx

 
 
 
sbb  %r15,%r8

 
 
 
mov  $0,%rbx

 
 
 
mov  $38,%rbp

 
 
 
cmovae %rbx,%rbp

 
 
 
sub  %rbp,%rsi

 
 
 
sbb  %rbx,%rdx

 
 
 
sbb  %rbx,%rcx

 
 
 
sbb  %rbx,%r8

 
 
 
cmovc %rbp,%rbx

 
 
 
sub  %rbx,%rsi

 
 
 
add  %r12,%r9

 
 
 
adc %r13,%rax

 
 
 
adc %r14,%r10

 
 
 
adc %r15,%r11

 
 
 
mov  $0,%r12

 
 
 
mov  $38,%r13

 
 
 
cmovae %r12,%r13

 
 
 
add  %r13,%r9

 
 
 
adc %r12,%rax

 
 
 
adc %r12,%r10

 
 
 
adc %r12,%r11

 
 
 
cmovc %r13,%r12

 
 
 
add  %r12,%r9

 
 
 
movq %r9,120(%rsp)

 
 
 
movq %rax,128(%rsp)

 
 
 
movq %r10,136(%rsp)

 
 
 
movq %r11,144(%rsp)

 
 
 
movq %rsi,152(%rsp)

 
 
 
movq %rdx,160(%rsp)

 
 
 
movq %rcx,168(%rsp)

 
 
 
movq %r8,176(%rsp)

 
 
 
mov  $0,%rsi

 
 
 
mov  $0,%rcx

 
 
 
mov  $0,%r8

 
 
 
mov  $0,%r9

 
 
 
movq 88(%rsp),%r10

 
 
 
movq 152(%rsp),%rax

 
 
 
mul  %r10

 
 
 
mov  %rax,%r11

 
 
 
mov  %rdx,%r12

 
 
 
movq 160(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r12

 
 
 
mov  $0,%r13

 
 
 
adc %rdx,%r13

 
 
 
movq 168(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r13

 
 
 
mov  $0,%r14

 
 
 
adc %rdx,%r14

 
 
 
movq 176(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%rsi

 
 
 
movq 96(%rsp),%r10

 
 
 
movq 152(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r12

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 160(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r13

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%r13

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 168(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r14

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%r14

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 176(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%rsi

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%rsi

 
 
 
adc %rdx,%rcx

 
 
 
movq 104(%rsp),%r10

 
 
 
movq 152(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r13

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 160(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r14

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%r14

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 168(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%rsi

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%rsi

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 176(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%rcx

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq 112(%rsp),%r10

 
 
 
movq 152(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r14

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 160(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%rsi

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%rsi

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 168(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%rcx

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%rcx

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 176(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%r8

 
 
 
adc %rdx,%r9

 
 
 
mov  %rsi,%rax

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
mov  %rax,%rsi

 
 
 
mov  %rcx,%rax

 
 
 
mov  %rdx,%rcx

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%rcx

 
 
 
mov  %r8,%rax

 
 
 
mov  $0,%r8

 
 
 
adc %rdx,%r8

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r8

 
 
 
mov  %r9,%rax

 
 
 
mov  $0,%r9

 
 
 
adc %rdx,%r9

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r9

 
 
 
mov  $0,%rax

 
 
 
adc %rdx,%rax

 
 
 
add  %rsi,%r11

 
 
 
adc %rcx,%r12

 
 
 
adc %r8,%r13

 
 
 
adc %r9,%r14

 
 
 
mov  $0,%rsi

 
 
 
adc %rsi,%rax

 
 
 
imulq  $38,%rax,%rdx

 
 
 
add  %rdx,%r11

 
 
 
adc %rsi,%r12

 
 
 
adc %rsi,%r13

 
 
 
adc %rsi,%r14

 
 
 
adc %rsi,%rsi

 
 
 
imulq  $38,%rsi,%rsi

 
 
 
add  %rsi,%r11

 
 
 
movq   %r11,0(%rdi)

 
 
 
movq   %r12,8(%rdi)

 
 
 
movq   %r13,16(%rdi)

 
 
 
movq   %r14,24(%rdi)

 
 
 
mov  $0,%rsi

 
 
 
mov  $0,%rcx

 
 
 
mov  $0,%r8

 
 
 
mov  $0,%r9

 
 
 
movq 56(%rsp),%r10

 
 
 
movq 120(%rsp),%rax

 
 
 
mul  %r10

 
 
 
mov  %rax,%r11

 
 
 
mov  %rdx,%r12

 
 
 
movq 128(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r12

 
 
 
mov  $0,%r13

 
 
 
adc %rdx,%r13

 
 
 
movq 136(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r13

 
 
 
mov  $0,%r14

 
 
 
adc %rdx,%r14

 
 
 
movq 144(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%rsi

 
 
 
movq 64(%rsp),%r10

 
 
 
movq 120(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r12

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 128(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r13

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%r13

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 136(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r14

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%r14

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 144(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%rsi

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%rsi

 
 
 
adc %rdx,%rcx

 
 
 
movq 72(%rsp),%r10

 
 
 
movq 120(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r13

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 128(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r14

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%r14

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 136(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%rsi

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%rsi

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 144(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%rcx

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq 80(%rsp),%r10

 
 
 
movq 120(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r14

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 128(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%rsi

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%rsi

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 136(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%rcx

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%rcx

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 144(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%r8

 
 
 
adc %rdx,%r9

 
 
 
mov  %rsi,%rax

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
mov  %rax,%rsi

 
 
 
mov  %rcx,%rax

 
 
 
mov  %rdx,%rcx

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%rcx

 
 
 
mov  %r8,%rax

 
 
 
mov  $0,%r8

 
 
 
adc %rdx,%r8

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r8

 
 
 
mov  %r9,%rax

 
 
 
mov  $0,%r9

 
 
 
adc %rdx,%r9

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r9

 
 
 
mov  $0,%rax

 
 
 
adc %rdx,%rax

 
 
 
add  %rsi,%r11

 
 
 
adc %rcx,%r12

 
 
 
adc %r8,%r13

 
 
 
adc %r9,%r14

 
 
 
mov  $0,%rsi

 
 
 
adc %rsi,%rax

 
 
 
imulq  $38,%rax,%rdx

 
 
 
add  %rdx,%r11

 
 
 
adc %rsi,%r12

 
 
 
adc %rsi,%r13

 
 
 
adc %rsi,%r14

 
 
 
adc %rsi,%rsi

 
 
 
imulq  $38,%rsi,%rsi

 
 
 
add  %rsi,%r11

 
 
 
movq   %r11,32(%rdi)

 
 
 
movq   %r12,40(%rdi)

 
 
 
movq   %r13,48(%rdi)

 
 
 
movq   %r14,56(%rdi)

 
 
 
mov  $0,%rsi

 
 
 
mov  $0,%rcx

 
 
 
mov  $0,%r8

 
 
 
mov  $0,%r9

 
 
 
movq 120(%rsp),%r10

 
 
 
movq 152(%rsp),%rax

 
 
 
mul  %r10

 
 
 
mov  %rax,%r11

 
 
 
mov  %rdx,%r12

 
 
 
movq 160(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r12

 
 
 
mov  $0,%r13

 
 
 
adc %rdx,%r13

 
 
 
movq 168(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r13

 
 
 
mov  $0,%r14

 
 
 
adc %rdx,%r14

 
 
 
movq 176(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%rsi

 
 
 
movq 128(%rsp),%r10

 
 
 
movq 152(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r12

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 160(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r13

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%r13

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 168(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r14

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%r14

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 176(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%rsi

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%rsi

 
 
 
adc %rdx,%rcx

 
 
 
movq 136(%rsp),%r10

 
 
 
movq 152(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r13

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 160(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r14

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%r14

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 168(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%rsi

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%rsi

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 176(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%rcx

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq 144(%rsp),%r10

 
 
 
movq 152(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r14

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 160(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%rsi

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%rsi

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 168(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%rcx

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%rcx

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 176(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%r8

 
 
 
adc %rdx,%r9

 
 
 
mov  %rsi,%rax

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
mov  %rax,%rsi

 
 
 
mov  %rcx,%rax

 
 
 
mov  %rdx,%rcx

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%rcx

 
 
 
mov  %r8,%rax

 
 
 
mov  $0,%r8

 
 
 
adc %rdx,%r8

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r8

 
 
 
mov  %r9,%rax

 
 
 
mov  $0,%r9

 
 
 
adc %rdx,%r9

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r9

 
 
 
mov  $0,%rax

 
 
 
adc %rdx,%rax

 
 
 
add  %rsi,%r11

 
 
 
adc %rcx,%r12

 
 
 
adc %r8,%r13

 
 
 
adc %r9,%r14

 
 
 
mov  $0,%rsi

 
 
 
adc %rsi,%rax

 
 
 
imulq  $38,%rax,%rdx

 
 
 
add  %rdx,%r11

 
 
 
adc %rsi,%r12

 
 
 
adc %rsi,%r13

 
 
 
adc %rsi,%r14

 
 
 
adc %rsi,%rsi

 
 
 
imulq  $38,%rsi,%rsi

 
 
 
add  %rsi,%r11

 
 
 
movq   %r11,64(%rdi)

 
 
 
movq   %r12,72(%rdi)

 
 
 
movq   %r13,80(%rdi)

 
 
 
movq   %r14,88(%rdi)

 
 
 
mov  $0,%rsi

 
 
 
mov  $0,%rcx

 
 
 
mov  $0,%r8

 
 
 
mov  $0,%r9

 
 
 
movq 88(%rsp),%r10

 
 
 
movq 56(%rsp),%rax

 
 
 
mul  %r10

 
 
 
mov  %rax,%r11

 
 
 
mov  %rdx,%r12

 
 
 
movq 64(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r12

 
 
 
mov  $0,%r13

 
 
 
adc %rdx,%r13

 
 
 
movq 72(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r13

 
 
 
mov  $0,%r14

 
 
 
adc %rdx,%r14

 
 
 
movq 80(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%rsi

 
 
 
movq 96(%rsp),%r10

 
 
 
movq 56(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r12

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 64(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r13

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%r13

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 72(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r14

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%r14

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 80(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%rsi

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%rsi

 
 
 
adc %rdx,%rcx

 
 
 
movq 104(%rsp),%r10

 
 
 
movq 56(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r13

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 64(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r14

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%r14

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 72(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%rsi

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%rsi

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 80(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%rcx

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq 112(%rsp),%r10

 
 
 
movq 56(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r14

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 64(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%rsi

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%rsi

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 72(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%rcx

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%rcx

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq 80(%rsp),%rax

 
 
 
mul  %r10

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %r15,%r8

 
 
 
adc %rdx,%r9

 
 
 
mov  %rsi,%rax

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
mov  %rax,%rsi

 
 
 
mov  %rcx,%rax

 
 
 
mov  %rdx,%rcx

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%rcx

 
 
 
mov  %r8,%rax

 
 
 
mov  $0,%r8

 
 
 
adc %rdx,%r8

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r8

 
 
 
mov  %r9,%rax

 
 
 
mov  $0,%r9

 
 
 
adc %rdx,%r9

 
mulq  crypto_sign_ed25519_amd64_64_38(%rip)

 
 
 
add  %rax,%r9

 
 
 
mov  $0,%rax

 
 
 
adc %rdx,%rax

 
 
 
add  %rsi,%r11

 
 
 
adc %rcx,%r12

 
 
 
adc %r8,%r13

 
 
 
adc %r9,%r14

 
 
 
mov  $0,%rsi

 
 
 
adc %rsi,%rax

 
 
 
imulq  $38,%rax,%rdx

 
 
 
add  %rdx,%r11

 
 
 
adc %rsi,%r12

 
 
 
adc %rsi,%r13

 
 
 
adc %rsi,%r14

 
 
 
adc %rsi,%rsi

 
 
 
imulq  $38,%rsi,%rsi

 
 
 
add  %rsi,%r11

 
 
 
movq   %r11,96(%rdi)

 
 
 
movq   %r12,104(%rdi)

 
 
 
movq   %r13,112(%rdi)

 
 
 
movq   %r14,120(%rdi)

 
 
 
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
