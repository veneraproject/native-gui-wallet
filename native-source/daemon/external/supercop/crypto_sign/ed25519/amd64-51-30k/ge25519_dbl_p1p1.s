
 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 
.text
.p2align 5
.globl _crypto_sign_ed25519_amd64_51_30k_batch_ge25519_dbl_p1p1
.globl crypto_sign_ed25519_amd64_51_30k_batch_ge25519_dbl_p1p1
_crypto_sign_ed25519_amd64_51_30k_batch_ge25519_dbl_p1p1:
crypto_sign_ed25519_amd64_51_30k_batch_ge25519_dbl_p1p1:
mov %rsp,%r11
and $31,%r11
add $224,%r11
sub %r11,%rsp

 
 
 
movq %r11,0(%rsp)

 
 
 
movq %r12,8(%rsp)

 
 
 
movq %r13,16(%rsp)

 
 
 
movq %r14,24(%rsp)

 
 
 
movq %r15,32(%rsp)

 
 
 
movq %rbx,40(%rsp)

 
 
 
movq %rbp,48(%rsp)

 
 
 
movq   0(%rsi),%rax

 
 
 
mulq  0(%rsi)

 
 
 
mov  %rax,%rcx

 
 
 
mov  %rdx,%r8

 
 
 
movq   0(%rsi),%rax

 
 
 
shl  $1,%rax

 
 
 
mulq  8(%rsi)

 
 
 
mov  %rax,%r9

 
 
 
mov  %rdx,%r10

 
 
 
movq   0(%rsi),%rax

 
 
 
shl  $1,%rax

 
 
 
mulq  16(%rsi)

 
 
 
mov  %rax,%r11

 
 
 
mov  %rdx,%r12

 
 
 
movq   0(%rsi),%rax

 
 
 
shl  $1,%rax

 
 
 
mulq  24(%rsi)

 
 
 
mov  %rax,%r13

 
 
 
mov  %rdx,%r14

 
 
 
movq   0(%rsi),%rax

 
 
 
shl  $1,%rax

 
 
 
mulq  32(%rsi)

 
 
 
mov  %rax,%r15

 
 
 
mov  %rdx,%rbx

 
 
 
movq   8(%rsi),%rax

 
 
 
mulq  8(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq   8(%rsi),%rax

 
 
 
shl  $1,%rax

 
 
 
mulq  16(%rsi)

 
 
 
add  %rax,%r13

 
 
 
adc %rdx,%r14

 
 
 
movq   8(%rsi),%rax

 
 
 
shl  $1,%rax

 
 
 
mulq  24(%rsi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq   8(%rsi),%rdx

 
 
 
imulq  $38,%rdx,%rax

 
 
 
mulq  32(%rsi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq   16(%rsi),%rax

 
 
 
mulq  16(%rsi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq   16(%rsi),%rdx

 
 
 
imulq  $38,%rdx,%rax

 
 
 
mulq  24(%rsi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq   16(%rsi),%rdx

 
 
 
imulq  $38,%rdx,%rax

 
 
 
mulq  32(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq   24(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  24(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq   24(%rsi),%rdx

 
 
 
imulq  $38,%rdx,%rax

 
 
 
mulq  32(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq   32(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  32(%rsi)

 
 
 
add  %rax,%r13

 
 
 
adc %rdx,%r14

 
 
 
movq crypto_sign_ed25519_amd64_51_30k_batch_REDMASK51(%rip),%rdx

 
 
 
shld $13,%rcx,%r8

 
 
 
and  %rdx,%rcx

 
 
 
shld $13,%r9,%r10

 
 
 
and  %rdx,%r9

 
 
 
add  %r8,%r9

 
 
 
shld $13,%r11,%r12

 
 
 
and  %rdx,%r11

 
 
 
add  %r10,%r11

 
 
 
shld $13,%r13,%r14

 
 
 
and  %rdx,%r13

 
 
 
add  %r12,%r13

 
 
 
shld $13,%r15,%rbx

 
 
 
and  %rdx,%r15

 
 
 
add  %r14,%r15

 
 
 
imulq  $19,%rbx,%r8

 
 
 
add  %r8,%rcx

 
 
 
mov  %rcx,%r8

 
 
 
shr  $51,%r8

 
 
 
add  %r9,%r8

 
 
 
and  %rdx,%rcx

 
 
 
mov  %r8,%r9

 
 
 
shr  $51,%r8

 
 
 
add  %r11,%r8

 
 
 
and  %rdx,%r9

 
 
 
mov  %r8,%rax

 
 
 
shr  $51,%r8

 
 
 
add  %r13,%r8

 
 
 
and  %rdx,%rax

 
 
 
mov  %r8,%r10

 
 
 
shr  $51,%r8

 
 
 
add  %r15,%r8

 
 
 
and  %rdx,%r10

 
 
 
mov  %r8,%r11

 
 
 
shr  $51,%r8

 
 
 
imulq  $19,%r8,%r8

 
 
 
add  %r8,%rcx

 
 
 
and  %rdx,%r11

 
 
 
movq %rcx,56(%rsp)

 
 
 
movq %r9,64(%rsp)

 
 
 
movq %rax,72(%rsp)

 
 
 
movq %r10,80(%rsp)

 
 
 
movq %r11,88(%rsp)

 
 
 
movq   40(%rsi),%rax

 
 
 
mulq  40(%rsi)

 
 
 
mov  %rax,%rcx

 
 
 
mov  %rdx,%r8

 
 
 
movq   40(%rsi),%rax

 
 
 
shl  $1,%rax

 
 
 
mulq  48(%rsi)

 
 
 
mov  %rax,%r9

 
 
 
mov  %rdx,%r10

 
 
 
movq   40(%rsi),%rax

 
 
 
shl  $1,%rax

 
 
 
mulq  56(%rsi)

 
 
 
mov  %rax,%r11

 
 
 
mov  %rdx,%r12

 
 
 
movq   40(%rsi),%rax

 
 
 
shl  $1,%rax

 
 
 
mulq  64(%rsi)

 
 
 
mov  %rax,%r13

 
 
 
mov  %rdx,%r14

 
 
 
movq   40(%rsi),%rax

 
 
 
shl  $1,%rax

 
 
 
mulq  72(%rsi)

 
 
 
mov  %rax,%r15

 
 
 
mov  %rdx,%rbx

 
 
 
movq   48(%rsi),%rax

 
 
 
mulq  48(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq   48(%rsi),%rax

 
 
 
shl  $1,%rax

 
 
 
mulq  56(%rsi)

 
 
 
add  %rax,%r13

 
 
 
adc %rdx,%r14

 
 
 
movq   48(%rsi),%rax

 
 
 
shl  $1,%rax

 
 
 
mulq  64(%rsi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq   48(%rsi),%rdx

 
 
 
imulq  $38,%rdx,%rax

 
 
 
mulq  72(%rsi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq   56(%rsi),%rax

 
 
 
mulq  56(%rsi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq   56(%rsi),%rdx

 
 
 
imulq  $38,%rdx,%rax

 
 
 
mulq  64(%rsi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq   56(%rsi),%rdx

 
 
 
imulq  $38,%rdx,%rax

 
 
 
mulq  72(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq   64(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  64(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq   64(%rsi),%rdx

 
 
 
imulq  $38,%rdx,%rax

 
 
 
mulq  72(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq   72(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  72(%rsi)

 
 
 
add  %rax,%r13

 
 
 
adc %rdx,%r14

 
 
 
movq crypto_sign_ed25519_amd64_51_30k_batch_REDMASK51(%rip),%rdx

 
 
 
shld $13,%rcx,%r8

 
 
 
and  %rdx,%rcx

 
 
 
shld $13,%r9,%r10

 
 
 
and  %rdx,%r9

 
 
 
add  %r8,%r9

 
 
 
shld $13,%r11,%r12

 
 
 
and  %rdx,%r11

 
 
 
add  %r10,%r11

 
 
 
shld $13,%r13,%r14

 
 
 
and  %rdx,%r13

 
 
 
add  %r12,%r13

 
 
 
shld $13,%r15,%rbx

 
 
 
and  %rdx,%r15

 
 
 
add  %r14,%r15

 
 
 
imulq  $19,%rbx,%r8

 
 
 
add  %r8,%rcx

 
 
 
mov  %rcx,%r8

 
 
 
shr  $51,%r8

 
 
 
add  %r9,%r8

 
 
 
and  %rdx,%rcx

 
 
 
mov  %r8,%r9

 
 
 
shr  $51,%r8

 
 
 
add  %r11,%r8

 
 
 
and  %rdx,%r9

 
 
 
mov  %r8,%rax

 
 
 
shr  $51,%r8

 
 
 
add  %r13,%r8

 
 
 
and  %rdx,%rax

 
 
 
mov  %r8,%r10

 
 
 
shr  $51,%r8

 
 
 
add  %r15,%r8

 
 
 
and  %rdx,%r10

 
 
 
mov  %r8,%r11

 
 
 
shr  $51,%r8

 
 
 
imulq  $19,%r8,%r8

 
 
 
add  %r8,%rcx

 
 
 
and  %rdx,%r11

 
 
 
movq %rcx,96(%rsp)

 
 
 
movq %r9,104(%rsp)

 
 
 
movq %rax,112(%rsp)

 
 
 
movq %r10,120(%rsp)

 
 
 
movq %r11,128(%rsp)

 
 
 
movq   80(%rsi),%rax

 
 
 
mulq  80(%rsi)

 
 
 
mov  %rax,%rcx

 
 
 
mov  %rdx,%r8

 
 
 
movq   80(%rsi),%rax

 
 
 
shl  $1,%rax

 
 
 
mulq  88(%rsi)

 
 
 
mov  %rax,%r9

 
 
 
mov  %rdx,%r10

 
 
 
movq   80(%rsi),%rax

 
 
 
shl  $1,%rax

 
 
 
mulq  96(%rsi)

 
 
 
mov  %rax,%r11

 
 
 
mov  %rdx,%r12

 
 
 
movq   80(%rsi),%rax

 
 
 
shl  $1,%rax

 
 
 
mulq  104(%rsi)

 
 
 
mov  %rax,%r13

 
 
 
mov  %rdx,%r14

 
 
 
movq   80(%rsi),%rax

 
 
 
shl  $1,%rax

 
 
 
mulq  112(%rsi)

 
 
 
mov  %rax,%r15

 
 
 
mov  %rdx,%rbx

 
 
 
movq   88(%rsi),%rax

 
 
 
mulq  88(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq   88(%rsi),%rax

 
 
 
shl  $1,%rax

 
 
 
mulq  96(%rsi)

 
 
 
add  %rax,%r13

 
 
 
adc %rdx,%r14

 
 
 
movq   88(%rsi),%rax

 
 
 
shl  $1,%rax

 
 
 
mulq  104(%rsi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq   88(%rsi),%rdx

 
 
 
imulq  $38,%rdx,%rax

 
 
 
mulq  112(%rsi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq   96(%rsi),%rax

 
 
 
mulq  96(%rsi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq   96(%rsi),%rdx

 
 
 
imulq  $38,%rdx,%rax

 
 
 
mulq  104(%rsi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq   96(%rsi),%rdx

 
 
 
imulq  $38,%rdx,%rax

 
 
 
mulq  112(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq   104(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  104(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq   104(%rsi),%rdx

 
 
 
imulq  $38,%rdx,%rax

 
 
 
mulq  112(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq   112(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  112(%rsi)

 
 
 
add  %rax,%r13

 
 
 
adc %rdx,%r14

 
 
 
movq crypto_sign_ed25519_amd64_51_30k_batch_REDMASK51(%rip),%rdx

 
 
 
shld $13,%rcx,%r8

 
 
 
and  %rdx,%rcx

 
 
 
shld $13,%r9,%r10

 
 
 
and  %rdx,%r9

 
 
 
add  %r8,%r9

 
 
 
shld $13,%r11,%r12

 
 
 
and  %rdx,%r11

 
 
 
add  %r10,%r11

 
 
 
shld $13,%r13,%r14

 
 
 
and  %rdx,%r13

 
 
 
add  %r12,%r13

 
 
 
shld $13,%r15,%rbx

 
 
 
and  %rdx,%r15

 
 
 
add  %r14,%r15

 
 
 
imulq  $19,%rbx,%r8

 
 
 
add  %r8,%rcx

 
 
 
mov  %rcx,%r8

 
 
 
shr  $51,%r8

 
 
 
add  %r9,%r8

 
 
 
and  %rdx,%rcx

 
 
 
mov  %r8,%r9

 
 
 
shr  $51,%r8

 
 
 
add  %r11,%r8

 
 
 
and  %rdx,%r9

 
 
 
mov  %r8,%rax

 
 
 
shr  $51,%r8

 
 
 
add  %r13,%r8

 
 
 
and  %rdx,%rax

 
 
 
mov  %r8,%r10

 
 
 
shr  $51,%r8

 
 
 
add  %r15,%r8

 
 
 
and  %rdx,%r10

 
 
 
mov  %r8,%r11

 
 
 
shr  $51,%r8

 
 
 
imulq  $19,%r8,%r8

 
 
 
add  %r8,%rcx

 
 
 
and  %rdx,%r11

 
 
 
add  %rcx,%rcx

 
 
 
add  %r9,%r9

 
 
 
add  %rax,%rax

 
 
 
add  %r10,%r10

 
 
 
add  %r11,%r11

 
 
 
movq %rcx,136(%rsp)

 
 
 
movq %r9,144(%rsp)

 
 
 
movq %rax,152(%rsp)

 
 
 
movq %r10,160(%rsp)

 
 
 
movq %r11,168(%rsp)

 
 
 
movq crypto_sign_ed25519_amd64_51_30k_batch_2P0(%rip),%rdx

 
 
 
movq crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%rcx

 
 
 
movq crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%r8

 
 
 
movq crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%r9

 
 
 
movq crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%rax

 
 
 
mov  %rdx,%r10

 
 
 
mov  %rcx,%r11

 
 
 
mov  %r8,%r12

 
 
 
mov  %r9,%r13

 
 
 
mov  %rax,%r14

 
 
 
subq 56(%rsp),%rdx

 
 
 
subq 64(%rsp),%rcx

 
 
 
subq 72(%rsp),%r8

 
 
 
subq 80(%rsp),%r9

 
 
 
subq 88(%rsp),%rax

 
 
 
subq 96(%rsp),%r10

 
 
 
subq 104(%rsp),%r11

 
 
 
subq 112(%rsp),%r12

 
 
 
subq 120(%rsp),%r13

 
 
 
subq 128(%rsp),%r14

 
 
 
movq %rdx,56(%rsp)

 
 
 
movq %rcx,64(%rsp)

 
 
 
movq %r8,72(%rsp)

 
 
 
movq %r9,80(%rsp)

 
 
 
movq %rax,88(%rsp)

 
 
 
movq %r10,176(%rsp)

 
 
 
movq %r11,184(%rsp)

 
 
 
movq %r12,192(%rsp)

 
 
 
movq %r13,200(%rsp)

 
 
 
movq %r14,208(%rsp)

 
 
 
mov  %rdx,%r10

 
 
 
mov  %rcx,%r11

 
 
 
mov  %r8,%r12

 
 
 
mov  %r9,%r13

 
 
 
mov  %rax,%r14

 
 
 
addq 96(%rsp),%r10

 
 
 
addq 104(%rsp),%r11

 
 
 
addq 112(%rsp),%r12

 
 
 
addq 120(%rsp),%r13

 
 
 
addq 128(%rsp),%r14

 
 
 
movq   %r10,40(%rdi)

 
 
 
movq   %r11,48(%rdi)

 
 
 
movq   %r12,56(%rdi)

 
 
 
movq   %r13,64(%rdi)

 
 
 
movq   %r14,72(%rdi)

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P0(%rip),%rdx

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%rcx

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%r8

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%r9

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%rax

 
 
 
subq 96(%rsp),%rdx

 
 
 
subq 104(%rsp),%rcx

 
 
 
subq 112(%rsp),%r8

 
 
 
subq 120(%rsp),%r9

 
 
 
subq 128(%rsp),%rax

 
 
 
movq   %rdx,80(%rdi)

 
 
 
movq   %rcx,88(%rdi)

 
 
 
movq   %r8,96(%rdi)

 
 
 
movq   %r9,104(%rdi)

 
 
 
movq   %rax,112(%rdi)

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_4P0(%rip),%r10

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_4P1234(%rip),%r11

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_4P1234(%rip),%r12

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_4P1234(%rip),%r13

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_4P1234(%rip),%r14

 
 
 
subq 136(%rsp),%r10

 
 
 
subq 144(%rsp),%r11

 
 
 
subq 152(%rsp),%r12

 
 
 
subq 160(%rsp),%r13

 
 
 
subq 168(%rsp),%r14

 
 
 
movq   %r10,120(%rdi)

 
 
 
movq   %r11,128(%rdi)

 
 
 
movq   %r12,136(%rdi)

 
 
 
movq   %r13,144(%rdi)

 
 
 
movq   %r14,152(%rdi)

 
 
 
movq   0(%rsi),%rdx

 
 
 
movq   8(%rsi),%rcx

 
 
 
movq   16(%rsi),%r8

 
 
 
movq   24(%rsi),%r9

 
 
 
movq   32(%rsi),%rax

 
 
 
addq 40(%rsi),%rdx

 
 
 
addq 48(%rsi),%rcx

 
 
 
addq 56(%rsi),%r8

 
 
 
addq 64(%rsi),%r9

 
 
 
addq 72(%rsi),%rax

 
 
 
movq %rdx,96(%rsp)

 
 
 
movq %rcx,104(%rsp)

 
 
 
movq %r8,112(%rsp)

 
 
 
movq %r9,120(%rsp)

 
 
 
movq %rax,128(%rsp)

 
 
 
movq 96(%rsp),%rax

 
 
 
mulq  96(%rsp)

 
 
 
mov  %rax,%rsi

 
 
 
mov  %rdx,%rcx

 
 
 
movq 96(%rsp),%rax

 
 
 
shl  $1,%rax

 
 
 
mulq  104(%rsp)

 
 
 
mov  %rax,%r8

 
 
 
mov  %rdx,%r9

 
 
 
movq 96(%rsp),%rax

 
 
 
shl  $1,%rax

 
 
 
mulq  112(%rsp)

 
 
 
mov  %rax,%r10

 
 
 
mov  %rdx,%r11

 
 
 
movq 96(%rsp),%rax

 
 
 
shl  $1,%rax

 
 
 
mulq  120(%rsp)

 
 
 
mov  %rax,%r12

 
 
 
mov  %rdx,%r13

 
 
 
movq 96(%rsp),%rax

 
 
 
shl  $1,%rax

 
 
 
mulq  128(%rsp)

 
 
 
mov  %rax,%r14

 
 
 
mov  %rdx,%r15

 
 
 
movq 104(%rsp),%rax

 
 
 
mulq  104(%rsp)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 104(%rsp),%rax

 
 
 
shl  $1,%rax

 
 
 
mulq  112(%rsp)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq 104(%rsp),%rax

 
 
 
shl  $1,%rax

 
 
 
mulq  120(%rsp)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq 104(%rsp),%rdx

 
 
 
imulq  $38,%rdx,%rax

 
 
 
mulq  128(%rsp)

 
 
 
add  %rax,%rsi

 
 
 
adc %rdx,%rcx

 
 
 
movq 112(%rsp),%rax

 
 
 
mulq  112(%rsp)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq 112(%rsp),%rdx

 
 
 
imulq  $38,%rdx,%rax

 
 
 
mulq  120(%rsp)

 
 
 
add  %rax,%rsi

 
 
 
adc %rdx,%rcx

 
 
 
movq 112(%rsp),%rdx

 
 
 
imulq  $38,%rdx,%rax

 
 
 
mulq  128(%rsp)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 120(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  120(%rsp)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 120(%rsp),%rdx

 
 
 
imulq  $38,%rdx,%rax

 
 
 
mulq  128(%rsp)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 128(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  128(%rsp)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq crypto_sign_ed25519_amd64_51_30k_batch_REDMASK51(%rip),%rdx

 
 
 
shld $13,%rsi,%rcx

 
 
 
and  %rdx,%rsi

 
 
 
shld $13,%r8,%r9

 
 
 
and  %rdx,%r8

 
 
 
add  %rcx,%r8

 
 
 
shld $13,%r10,%r11

 
 
 
and  %rdx,%r10

 
 
 
add  %r9,%r10

 
 
 
shld $13,%r12,%r13

 
 
 
and  %rdx,%r12

 
 
 
add  %r11,%r12

 
 
 
shld $13,%r14,%r15

 
 
 
and  %rdx,%r14

 
 
 
add  %r13,%r14

 
 
 
imulq  $19,%r15,%rcx

 
 
 
add  %rcx,%rsi

 
 
 
mov  %rsi,%rcx

 
 
 
shr  $51,%rcx

 
 
 
add  %r8,%rcx

 
 
 
and  %rdx,%rsi

 
 
 
mov  %rcx,%r8

 
 
 
shr  $51,%rcx

 
 
 
add  %r10,%rcx

 
 
 
and  %rdx,%r8

 
 
 
mov  %rcx,%r9

 
 
 
shr  $51,%rcx

 
 
 
add  %r12,%rcx

 
 
 
and  %rdx,%r9

 
 
 
mov  %rcx,%rax

 
 
 
shr  $51,%rcx

 
 
 
add  %r14,%rcx

 
 
 
and  %rdx,%rax

 
 
 
mov  %rcx,%r10

 
 
 
shr  $51,%rcx

 
 
 
imulq  $19,%rcx,%rcx

 
 
 
add  %rcx,%rsi

 
 
 
and  %rdx,%r10

 
 
 
addq 56(%rsp),%rsi

 
 
 
addq 64(%rsp),%r8

 
 
 
addq 72(%rsp),%r9

 
 
 
addq 80(%rsp),%rax

 
 
 
addq 88(%rsp),%r10

 
 
 
addq 176(%rsp),%rsi

 
 
 
addq 184(%rsp),%r8

 
 
 
addq 192(%rsp),%r9

 
 
 
addq 200(%rsp),%rax

 
 
 
addq 208(%rsp),%r10

 
 
 
movq   %rsi,0(%rdi)

 
 
 
movq   %r8,8(%rdi)

 
 
 
movq   %r9,16(%rdi)

 
 
 
movq   %rax,24(%rdi)

 
 
 
movq   %r10,32(%rdi)

 
 
 
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
