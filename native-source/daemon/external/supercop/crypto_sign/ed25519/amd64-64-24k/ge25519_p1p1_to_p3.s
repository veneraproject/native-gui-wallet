
 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 
.text
.p2align 5
.globl _crypto_sign_ed25519_amd64_64_ge25519_p1p1_to_p3
.globl crypto_sign_ed25519_amd64_64_ge25519_p1p1_to_p3
_crypto_sign_ed25519_amd64_64_ge25519_p1p1_to_p3:
crypto_sign_ed25519_amd64_64_ge25519_p1p1_to_p3:
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

 
 
 
mov  $0,%r8

 
 
 
mov  $0,%r9

 
 
 
mov  $0,%r10

 
 
 
movq   0(%rsi),%r11

 
 
 
movq   96(%rsi),%rax

 
 
 
mul  %r11

 
 
 
mov  %rax,%r12

 
 
 
mov  %rdx,%r13

 
 
 
movq   104(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r13

 
 
 
mov  $0,%r14

 
 
 
adc %rdx,%r14

 
 
 
movq   112(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r14

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq   120(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rcx

 
 
 
movq   8(%rsi),%r11

 
 
 
movq   96(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r13

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   104(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r14

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%r14

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   112(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r15

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%r15

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   120(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%rcx

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq   16(%rsi),%r11

 
 
 
movq   96(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r14

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   104(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r15

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%r15

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   112(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%rcx

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%rcx

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   120(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq   24(%rsi),%r11

 
 
 
movq   96(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r15

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   104(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%rcx

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%rcx

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   112(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%r8

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   120(%rsi),%rax

 
 
 
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

 
 
 
movq   %r12,0(%rdi)

 
 
 
movq   %r13,8(%rdi)

 
 
 
movq   %r14,16(%rdi)

 
 
 
movq   %r15,24(%rdi)

 
 
 
mov  $0,%rcx

 
 
 
mov  $0,%r8

 
 
 
mov  $0,%r9

 
 
 
mov  $0,%r10

 
 
 
movq   64(%rsi),%r11

 
 
 
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

 
 
 
movq   72(%rsi),%r11

 
 
 
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

 
 
 
movq   80(%rsi),%r11

 
 
 
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

 
 
 
movq   88(%rsi),%r11

 
 
 
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

 
 
 
movq   %r12,32(%rdi)

 
 
 
movq   %r13,40(%rdi)

 
 
 
movq   %r14,48(%rdi)

 
 
 
movq   %r15,56(%rdi)

 
 
 
mov  $0,%rcx

 
 
 
mov  $0,%r8

 
 
 
mov  $0,%r9

 
 
 
mov  $0,%r10

 
 
 
movq   32(%rsi),%r11

 
 
 
movq   96(%rsi),%rax

 
 
 
mul  %r11

 
 
 
mov  %rax,%r12

 
 
 
mov  %rdx,%r13

 
 
 
movq   104(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r13

 
 
 
mov  $0,%r14

 
 
 
adc %rdx,%r14

 
 
 
movq   112(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r14

 
 
 
mov  $0,%r15

 
 
 
adc %rdx,%r15

 
 
 
movq   120(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rcx

 
 
 
movq   40(%rsi),%r11

 
 
 
movq   96(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r13

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   104(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r14

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%r14

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   112(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r15

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%r15

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   120(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%rcx

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq   48(%rsi),%r11

 
 
 
movq   96(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r14

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   104(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r15

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%r15

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   112(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%rcx

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%rcx

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   120(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq   56(%rsi),%r11

 
 
 
movq   96(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r15

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   104(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%rcx

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%rcx

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   112(%rsi),%rax

 
 
 
mul  %r11

 
 
 
add  %rax,%r8

 
 
 
adc $0,%rdx

 
 
 
add  %rbx,%r8

 
 
 
mov  $0,%rbx

 
 
 
adc %rdx,%rbx

 
 
 
movq   120(%rsi),%rax

 
 
 
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

 
 
 
movq   %r12,64(%rdi)

 
 
 
movq   %r13,72(%rdi)

 
 
 
movq   %r14,80(%rdi)

 
 
 
movq   %r15,88(%rdi)

 
 
 
mov  $0,%rcx

 
 
 
mov  $0,%r8

 
 
 
mov  $0,%r9

 
 
 
mov  $0,%r10

 
 
 
movq   0(%rsi),%r11

 
 
 
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

 
 
 
movq   8(%rsi),%r11

 
 
 
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

 
 
 
movq   16(%rsi),%r11

 
 
 
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

 
 
 
movq   24(%rsi),%r11

 
 
 
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

 
 
 
movq   %r12,96(%rdi)

 
 
 
movq   %r13,104(%rdi)

 
 
 
movq   %r14,112(%rdi)

 
 
 
movq   %r15,120(%rdi)

 
 
 
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
