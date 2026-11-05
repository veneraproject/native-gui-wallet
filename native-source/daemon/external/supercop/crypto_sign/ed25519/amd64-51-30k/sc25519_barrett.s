
 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 
.text
.p2align 5
.globl _crypto_sign_ed25519_amd64_51_30k_batch_sc25519_barrett
.globl crypto_sign_ed25519_amd64_51_30k_batch_sc25519_barrett
_crypto_sign_ed25519_amd64_51_30k_batch_sc25519_barrett:
crypto_sign_ed25519_amd64_51_30k_batch_sc25519_barrett:
mov %rsp,%r11
and $31,%r11
add $96,%r11
sub %r11,%rsp

 
 
 
movq %r11,0(%rsp)

 
 
 
movq %r12,8(%rsp)

 
 
 
movq %r13,16(%rsp)

 
 
 
movq %r14,24(%rsp)

 
 
 
movq %r15,32(%rsp)

 
 
 
movq %rbx,40(%rsp)

 
 
 
movq %rbp,48(%rsp)

 
 
 
xor  %rcx,%rcx

 
 
 
xor  %r8,%r8

 
 
 
xor  %r9,%r9

 
 
 
xor  %r10,%r10

 
 
 
xor  %r11,%r11

 
 
 
movq   24(%rsi),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_MU3

 
 
 
mov  %rax,%r12

 
 
 
mov  %rdx,%r13

 
 
 
movq   24(%rsi),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_MU4

 
 
 
mov  %rax,%r14

 
 
 
add  %r13,%r14

 
 
 
adc %rdx,%r8

 
 
 
movq   32(%rsi),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_MU2

 
 
 
add  %rax,%r12

 
 
 
mov  $0,%r13

 
 
 
adc %rdx,%r13

 
 
 
movq   32(%rsi),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_MU3

 
 
 
add  %rax,%r14

 
 
 
adc %rcx,%rdx

 
 
 
add  %r13,%r14

 
 
 
mov  $0,%r13

 
 
 
adc %rdx,%r13

 
 
 
movq   32(%rsi),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_MU4

 
 
 
add  %rax,%r8

 
 
 
adc %rcx,%rdx

 
 
 
add  %r13,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq   40(%rsi),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_MU1

 
 
 
add  %rax,%r12

 
 
 
mov  $0,%r13

 
 
 
adc %rdx,%r13

 
 
 
movq   40(%rsi),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_MU2

 
 
 
add  %rax,%r14

 
 
 
adc %rcx,%rdx

 
 
 
add  %r13,%r14

 
 
 
mov  $0,%r13

 
 
 
adc %rdx,%r13

 
 
 
movq   40(%rsi),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_MU3

 
 
 
add  %rax,%r8

 
 
 
adc %rcx,%rdx

 
 
 
add  %r13,%r8

 
 
 
mov  $0,%r13

 
 
 
adc %rdx,%r13

 
 
 
movq   40(%rsi),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_MU4

 
 
 
add  %rax,%r9

 
 
 
adc %rcx,%rdx

 
 
 
add  %r13,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq   48(%rsi),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_MU0

 
 
 
add  %rax,%r12

 
 
 
mov  $0,%r12

 
 
 
adc %rdx,%r12

 
 
 
movq   48(%rsi),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_MU1

 
 
 
add  %rax,%r14

 
 
 
adc %rcx,%rdx

 
 
 
add  %r12,%r14

 
 
 
mov  $0,%r12

 
 
 
adc %rdx,%r12

 
 
 
movq   48(%rsi),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_MU2

 
 
 
add  %rax,%r8

 
 
 
adc %rcx,%rdx

 
 
 
add  %r12,%r8

 
 
 
mov  $0,%r12

 
 
 
adc %rdx,%r12

 
 
 
movq   48(%rsi),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_MU3

 
 
 
add  %rax,%r9

 
 
 
adc %rcx,%rdx

 
 
 
add  %r12,%r9

 
 
 
mov  $0,%r12

 
 
 
adc %rdx,%r12

 
 
 
movq   48(%rsi),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_MU4

 
 
 
add  %rax,%r10

 
 
 
adc %rcx,%rdx

 
 
 
add  %r12,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq   56(%rsi),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_MU0

 
 
 
add  %rax,%r14

 

 
 
 
mov  $0,%r12

 
 
 
adc %rdx,%r12

 
 
 
movq   56(%rsi),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_MU1

 
 
 
add  %rax,%r8

 
 
 
adc %rcx,%rdx

 
 
 
add  %r12,%r8

 
 
 
mov  $0,%r12

 
 
 
adc %rdx,%r12

 
 
 
movq %r8,56(%rsp)

 
 
 
movq   56(%rsi),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_MU2

 
 
 
add  %rax,%r9

 
 
 
adc %rcx,%rdx

 
 
 
add  %r12,%r9

 
 
 
mov  $0,%r8

 
 
 
adc %rdx,%r8

 
 
 
movq %r9,64(%rsp)

 
 
 
movq   56(%rsi),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_MU3

 
 
 
add  %rax,%r10

 
 
 
adc %rcx,%rdx

 
 
 
add  %r8,%r10

 
 
 
mov  $0,%r8

 
 
 
adc %rdx,%r8

 
 
 
movq %r10,72(%rsp)

 
 
 
movq   56(%rsi),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_MU4

 
 
 
add  %rax,%r11

 
 
 
adc %rcx,%rdx

 
 
 
add  %r8,%r11

 
 
 
movq %r11,80(%rsp)

 
 
 
movq 56(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_ORDER0

 
 
 
mov  %rax,%r8

 
 
 
mov  %rdx,%r9

 
 
 
movq 56(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_ORDER1

 
 
 
mov  %rax,%r10

 
 
 
add  %r9,%r10

 
 
 
mov  $0,%r9

 
 
 
adc %rdx,%r9

 
 
 
movq 56(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_ORDER2

 
 
 
mov  %rax,%r11

 
 
 
add  %r9,%r11

 
 
 
mov  $0,%r9

 
 
 
adc %rdx,%r9

 
 
 
movq 56(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_ORDER3

 

 
 
 
mov  %rax,%r12

 
 
 
add  %r9,%r12

 
 
 
movq 64(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_ORDER0

 
 
 
add  %rax,%r10

 
 
 
mov  $0,%r9

 
 
 
adc %rdx,%r9

 
 
 
movq 64(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_ORDER1

 
 
 
add  %rax,%r11

 
 
 
adc %rcx,%rdx

 
 
 
add  %r9,%r11

 
 
 
mov  $0,%rcx

 
 
 
adc %rdx,%rcx

 
 
 
movq 64(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_ORDER2

 

 
 
 
add  %rax,%r12

 
 
 
add  %rcx,%r12

 
 
 
movq 72(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_ORDER0

 
 
 
add  %rax,%r11

 
 
 
mov  $0,%rcx

 
 
 
adc %rdx,%rcx

 
 
 
movq 72(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_ORDER1

 

 
 
 
add  %rax,%r12

 
 
 
add  %rcx,%r12

 
 
 
movq 80(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_ORDER0

 

 
 
 
add  %rax,%r12

 
 
 
movq   0(%rsi),%rdx

 
 
 
sub  %r8,%rdx

 
 
 
mov  %rdx,%rcx

 
 
 
movq   8(%rsi),%r8

 
 
 
sbb  %r10,%r8

 
 
 
mov  %r8,%r9

 
 
 
movq   16(%rsi),%rax

 
 
 
sbb  %r11,%rax

 
 
 
mov  %rax,%r10

 
 
 
movq   24(%rsi),%rsi

 
 
 
sbb  %r12,%rsi

 
 
 
mov  %rsi,%r11

 
 
 
sub  crypto_sign_ed25519_amd64_51_30k_batch_ORDER0,%rcx

 
 
 
sbb  crypto_sign_ed25519_amd64_51_30k_batch_ORDER1,%r9

 
 
 
sbb  crypto_sign_ed25519_amd64_51_30k_batch_ORDER2,%r10

 
 
 
sbb  crypto_sign_ed25519_amd64_51_30k_batch_ORDER3,%r11

 
 
 
cmovae %rcx,%rdx

 
 
 
mov  %rdx,%rcx

 
 
 
cmovae %r9,%r8

 
 
 
mov  %r8,%r9

 
 
 
cmovae %r10,%rax

 
 
 
mov  %rax,%r10

 
 
 
cmovae %r11,%rsi

 
 
 
mov  %rsi,%r11

 
 
 
sub  crypto_sign_ed25519_amd64_51_30k_batch_ORDER0,%rcx

 
 
 
sbb  crypto_sign_ed25519_amd64_51_30k_batch_ORDER1,%r9

 
 
 
sbb  crypto_sign_ed25519_amd64_51_30k_batch_ORDER2,%r10

 
 
 
sbb  crypto_sign_ed25519_amd64_51_30k_batch_ORDER3,%r11

 
 
 
cmovae %rcx,%rdx

 
 
 
cmovae %r9,%r8

 
 
 
cmovae %r10,%rax

 
 
 
cmovae %r11,%rsi

 
 
 
movq   %rdx,0(%rdi)

 
 
 
movq   %r8,8(%rdi)

 
 
 
movq   %rax,16(%rdi)

 
 
 
movq   %rsi,24(%rdi)

 
 
 
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
