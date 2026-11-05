
 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 
.text
.p2align 5
.globl _crypto_sign_ed25519_amd64_51_30k_batch_ge25519_nielsadd2
.globl crypto_sign_ed25519_amd64_51_30k_batch_ge25519_nielsadd2
_crypto_sign_ed25519_amd64_51_30k_batch_ge25519_nielsadd2:
crypto_sign_ed25519_amd64_51_30k_batch_ge25519_nielsadd2:
mov %rsp,%r11
and $31,%r11
add $256,%r11
sub %r11,%rsp

 
 
 
movq %r11,0(%rsp)

 
 
 
movq %r12,8(%rsp)

 
 
 
movq %r13,16(%rsp)

 
 
 
movq %r14,24(%rsp)

 
 
 
movq %r15,32(%rsp)

 
 
 
movq %rbx,40(%rsp)

 
 
 
movq %rbp,48(%rsp)

 
 
 
movq   40(%rdi),%rdx

 
 
 
movq   48(%rdi),%rcx

 
 
 
movq   56(%rdi),%r8

 
 
 
movq   64(%rdi),%r9

 
 
 
movq   72(%rdi),%rax

 
 
 
mov  %rdx,%r10

 
 
 
mov  %rcx,%r11

 
 
 
mov  %r8,%r12

 
 
 
mov  %r9,%r13

 
 
 
mov  %rax,%r14

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P0(%rip),%rdx

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%rcx

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%r8

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%r9

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%rax

 
 
 
addq 0(%rdi),%r10

 
 
 
addq 8(%rdi),%r11

 
 
 
addq 16(%rdi),%r12

 
 
 
addq 24(%rdi),%r13

 
 
 
addq 32(%rdi),%r14

 
 
 
subq 0(%rdi),%rdx

 
 
 
subq 8(%rdi),%rcx

 
 
 
subq 16(%rdi),%r8

 
 
 
subq 24(%rdi),%r9

 
 
 
subq 32(%rdi),%rax

 
 
 
movq %rdx,56(%rsp)

 
 
 
movq %rcx,64(%rsp)

 
 
 
movq %r8,72(%rsp)

 
 
 
movq %r9,80(%rsp)

 
 
 
movq %rax,88(%rsp)

 
 
 
movq %r10,96(%rsp)

 
 
 
movq %r11,104(%rsp)

 
 
 
movq %r12,112(%rsp)

 
 
 
movq %r13,120(%rsp)

 
 
 
movq %r14,128(%rsp)

 
 
 
movq 80(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
movq %rax,136(%rsp)

 
 
 
mulq  16(%rsi)

 
 
 
mov  %rax,%rcx

 
 
 
mov  %rdx,%r8

 
 
 
movq 88(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
movq %rax,144(%rsp)

 
 
 
mulq  8(%rsi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq 56(%rsp),%rax

 
 
 
mulq  0(%rsi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq 56(%rsp),%rax

 
 
 
mulq  8(%rsi)

 
 
 
mov  %rax,%r9

 
 
 
mov  %rdx,%r10

 
 
 
movq 56(%rsp),%rax

 
 
 
mulq  16(%rsi)

 
 
 
mov  %rax,%r11

 
 
 
mov  %rdx,%r12

 
 
 
movq 56(%rsp),%rax

 
 
 
mulq  24(%rsi)

 
 
 
mov  %rax,%r13

 
 
 
mov  %rdx,%r14

 
 
 
movq 56(%rsp),%rax

 
 
 
mulq  32(%rsi)

 
 
 
mov  %rax,%r15

 
 
 
mov  %rdx,%rbx

 
 
 
movq 64(%rsp),%rax

 
 
 
mulq  0(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq 64(%rsp),%rax

 
 
 
mulq  8(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq 64(%rsp),%rax

 
 
 
mulq  16(%rsi)

 
 
 
add  %rax,%r13

 
 
 
adc %rdx,%r14

 
 
 
movq 64(%rsp),%rax

 
 
 
mulq  24(%rsi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq 64(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  32(%rsi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq 72(%rsp),%rax

 
 
 
mulq  0(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq 72(%rsp),%rax

 
 
 
mulq  8(%rsi)

 
 
 
add  %rax,%r13

 
 
 
adc %rdx,%r14

 
 
 
movq 72(%rsp),%rax

 
 
 
mulq  16(%rsi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq 72(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  24(%rsi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq 72(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  32(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq 80(%rsp),%rax

 
 
 
mulq  0(%rsi)

 
 
 
add  %rax,%r13

 
 
 
adc %rdx,%r14

 
 
 
movq 80(%rsp),%rax

 
 
 
mulq  8(%rsi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq 136(%rsp),%rax

 
 
 
mulq  24(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq 136(%rsp),%rax

 
 
 
mulq  32(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq 88(%rsp),%rax

 
 
 
mulq  0(%rsi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq 144(%rsp),%rax

 
 
 
mulq  16(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq 144(%rsp),%rax

 
 
 
mulq  24(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq 144(%rsp),%rax

 
 
 
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

 
 
 
mov  %r8,%r9

 
 
 
shr  $51,%r8

 
 
 
and  %rdx,%rcx

 
 
 
add  %r11,%r8

 
 
 
mov  %r8,%rax

 
 
 
shr  $51,%r8

 
 
 
and  %rdx,%r9

 
 
 
add  %r13,%r8

 
 
 
mov  %r8,%r10

 
 
 
shr  $51,%r8

 
 
 
and  %rdx,%rax

 
 
 
add  %r15,%r8

 
 
 
mov  %r8,%r11

 
 
 
shr  $51,%r8

 
 
 
and  %rdx,%r10

 
 
 
imulq  $19,%r8,%r8

 
 
 
add  %r8,%rcx

 
 
 
and  %rdx,%r11

 
 
 
movq %rcx,56(%rsp)

 
 
 
movq %r9,64(%rsp)

 
 
 
movq %rax,72(%rsp)

 
 
 
movq %r10,80(%rsp)

 
 
 
movq %r11,88(%rsp)

 
 
 
movq 120(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
movq %rax,136(%rsp)

 
 
 
mulq  56(%rsi)

 
 
 
mov  %rax,%rcx

 
 
 
mov  %rdx,%r8

 
 
 
movq 128(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
movq %rax,144(%rsp)

 
 
 
mulq  48(%rsi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq 96(%rsp),%rax

 
 
 
mulq  40(%rsi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq 96(%rsp),%rax

 
 
 
mulq  48(%rsi)

 
 
 
mov  %rax,%r9

 
 
 
mov  %rdx,%r10

 
 
 
movq 96(%rsp),%rax

 
 
 
mulq  56(%rsi)

 
 
 
mov  %rax,%r11

 
 
 
mov  %rdx,%r12

 
 
 
movq 96(%rsp),%rax

 
 
 
mulq  64(%rsi)

 
 
 
mov  %rax,%r13

 
 
 
mov  %rdx,%r14

 
 
 
movq 96(%rsp),%rax

 
 
 
mulq  72(%rsi)

 
 
 
mov  %rax,%r15

 
 
 
mov  %rdx,%rbx

 
 
 
movq 104(%rsp),%rax

 
 
 
mulq  40(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq 104(%rsp),%rax

 
 
 
mulq  48(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq 104(%rsp),%rax

 
 
 
mulq  56(%rsi)

 
 
 
add  %rax,%r13

 
 
 
adc %rdx,%r14

 
 
 
movq 104(%rsp),%rax

 
 
 
mulq  64(%rsi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq 104(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  72(%rsi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq 112(%rsp),%rax

 
 
 
mulq  40(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq 112(%rsp),%rax

 
 
 
mulq  48(%rsi)

 
 
 
add  %rax,%r13

 
 
 
adc %rdx,%r14

 
 
 
movq 112(%rsp),%rax

 
 
 
mulq  56(%rsi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq 112(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  64(%rsi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq 112(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  72(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq 120(%rsp),%rax

 
 
 
mulq  40(%rsi)

 
 
 
add  %rax,%r13

 
 
 
adc %rdx,%r14

 
 
 
movq 120(%rsp),%rax

 
 
 
mulq  48(%rsi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq 136(%rsp),%rax

 
 
 
mulq  64(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq 136(%rsp),%rax

 
 
 
mulq  72(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq 128(%rsp),%rax

 
 
 
mulq  40(%rsi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq 144(%rsp),%rax

 
 
 
mulq  56(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq 144(%rsp),%rax

 
 
 
mulq  64(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq 144(%rsp),%rax

 
 
 
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

 
 
 
mov  %r8,%r9

 
 
 
shr  $51,%r8

 
 
 
and  %rdx,%rcx

 
 
 
add  %r11,%r8

 
 
 
mov  %r8,%rax

 
 
 
shr  $51,%r8

 
 
 
and  %rdx,%r9

 
 
 
add  %r13,%r8

 
 
 
mov  %r8,%r10

 
 
 
shr  $51,%r8

 
 
 
and  %rdx,%rax

 
 
 
add  %r15,%r8

 
 
 
mov  %r8,%r11

 
 
 
shr  $51,%r8

 
 
 
and  %rdx,%r10

 
 
 
imulq  $19,%r8,%r8

 
 
 
add  %r8,%rcx

 
 
 
and  %rdx,%r11

 
 
 
mov  %rcx,%rdx

 
 
 
mov  %r9,%r8

 
 
 
mov  %rax,%r12

 
 
 
mov  %r10,%r13

 
 
 
mov  %r11,%r14

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P0(%rip),%rcx

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%r9

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%rax

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%r10

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%r11

 
 
 
addq 56(%rsp),%rdx

 
 
 
addq 64(%rsp),%r8

 
 
 
addq 72(%rsp),%r12

 
 
 
addq 80(%rsp),%r13

 
 
 
addq 88(%rsp),%r14

 
 
 
subq 56(%rsp),%rcx

 
 
 
subq 64(%rsp),%r9

 
 
 
subq 72(%rsp),%rax

 
 
 
subq 80(%rsp),%r10

 
 
 
subq 88(%rsp),%r11

 
 
 
movq %rdx,56(%rsp)

 
 
 
movq %r8,64(%rsp)

 
 
 
movq %r12,72(%rsp)

 
 
 
movq %r13,80(%rsp)

 
 
 
movq %r14,88(%rsp)

 
 
 
movq %rcx,96(%rsp)

 
 
 
movq %r9,104(%rsp)

 
 
 
movq %rax,112(%rsp)

 
 
 
movq %r10,120(%rsp)

 
 
 
movq %r11,128(%rsp)

 
 
 
movq   144(%rdi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
movq %rax,136(%rsp)

 
 
 
mulq  96(%rsi)

 
 
 
mov  %rax,%rcx

 
 
 
mov  %rdx,%r8

 
 
 
movq   152(%rdi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
movq %rax,144(%rsp)

 
 
 
mulq  88(%rsi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq   120(%rdi),%rax

 
 
 
mulq  80(%rsi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq   120(%rdi),%rax

 
 
 
mulq  88(%rsi)

 
 
 
mov  %rax,%r9

 
 
 
mov  %rdx,%r10

 
 
 
movq   120(%rdi),%rax

 
 
 
mulq  96(%rsi)

 
 
 
mov  %rax,%r11

 
 
 
mov  %rdx,%r12

 
 
 
movq   120(%rdi),%rax

 
 
 
mulq  104(%rsi)

 
 
 
mov  %rax,%r13

 
 
 
mov  %rdx,%r14

 
 
 
movq   120(%rdi),%rax

 
 
 
mulq  112(%rsi)

 
 
 
mov  %rax,%r15

 
 
 
mov  %rdx,%rbx

 
 
 
movq   128(%rdi),%rax

 
 
 
mulq  80(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq   128(%rdi),%rax

 
 
 
mulq  88(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq   128(%rdi),%rax

 
 
 
mulq  96(%rsi)

 
 
 
add  %rax,%r13

 
 
 
adc %rdx,%r14

 
 
 
movq   128(%rdi),%rax

 
 
 
mulq  104(%rsi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq   128(%rdi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  112(%rsi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq   136(%rdi),%rax

 
 
 
mulq  80(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq   136(%rdi),%rax

 
 
 
mulq  88(%rsi)

 
 
 
add  %rax,%r13

 
 
 
adc %rdx,%r14

 
 
 
movq   136(%rdi),%rax

 
 
 
mulq  96(%rsi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq   136(%rdi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  104(%rsi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq   136(%rdi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  112(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq   144(%rdi),%rax

 
 
 
mulq  80(%rsi)

 
 
 
add  %rax,%r13

 
 
 
adc %rdx,%r14

 
 
 
movq   144(%rdi),%rax

 
 
 
mulq  88(%rsi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq 136(%rsp),%rax

 
 
 
mulq  104(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq 136(%rsp),%rax

 
 
 
mulq  112(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq   152(%rdi),%rax

 
 
 
mulq  80(%rsi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq 144(%rsp),%rax

 
 
 
mulq  96(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq 144(%rsp),%rax

 
 
 
mulq  104(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq 144(%rsp),%rax

 
 
 
mulq  112(%rsi)

 
 
 
add  %rax,%r13

 
 
 
adc %rdx,%r14

 
 
 
movq crypto_sign_ed25519_amd64_51_30k_batch_REDMASK51(%rip),%rsi

 
 
 
shld $13,%rcx,%r8

 
 
 
and  %rsi,%rcx

 
 
 
shld $13,%r9,%r10

 
 
 
and  %rsi,%r9

 
 
 
add  %r8,%r9

 
 
 
shld $13,%r11,%r12

 
 
 
and  %rsi,%r11

 
 
 
add  %r10,%r11

 
 
 
shld $13,%r13,%r14

 
 
 
and  %rsi,%r13

 
 
 
add  %r12,%r13

 
 
 
shld $13,%r15,%rbx

 
 
 
and  %rsi,%r15

 
 
 
add  %r14,%r15

 
 
 
imulq  $19,%rbx,%rdx

 
 
 
add  %rdx,%rcx

 
 
 
mov  %rcx,%rdx

 
 
 
shr  $51,%rdx

 
 
 
add  %r9,%rdx

 
 
 
mov  %rdx,%r8

 
 
 
shr  $51,%rdx

 
 
 
and  %rsi,%rcx

 
 
 
add  %r11,%rdx

 
 
 
mov  %rdx,%r9

 
 
 
shr  $51,%rdx

 
 
 
and  %rsi,%r8

 
 
 
add  %r13,%rdx

 
 
 
mov  %rdx,%rax

 
 
 
shr  $51,%rdx

 
 
 
and  %rsi,%r9

 
 
 
add  %r15,%rdx

 
 
 
mov  %rdx,%r10

 
 
 
shr  $51,%rdx

 
 
 
and  %rsi,%rax

 
 
 
imulq  $19,%rdx,%rdx

 
 
 
add  %rdx,%rcx

 
 
 
and  %rsi,%r10

 
 
 
movq %rcx,136(%rsp)

 
 
 
movq   80(%rdi),%rsi

 
 
 
movq   88(%rdi),%rdx

 
 
 
movq   96(%rdi),%rcx

 
 
 
movq   104(%rdi),%r11

 
 
 
movq   112(%rdi),%r12

 
 
 
add  %rsi,%rsi

 
 
 
add  %rdx,%rdx

 
 
 
add  %rcx,%rcx

 
 
 
add  %r11,%r11

 
 
 
add  %r12,%r12

 
 
 
mov  %rsi,%r13

 
 
 
mov  %rdx,%r14

 
 
 
mov  %rcx,%r15

 
 
 
mov  %r11,%rbx

 
 
 
mov  %r12,%rbp

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P0(%rip),%rsi

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%rdx

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%rcx

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%r11

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%r12

 
 
 
addq 136(%rsp),%r13

 
 
 
add  %r8,%r14

 
 
 
add  %r9,%r15

 
 
 
add  %rax,%rbx

 
 
 
add  %r10,%rbp

 
 
 
subq 136(%rsp),%rsi

 
 
 
sub  %r8,%rdx

 
 
 
sub  %r9,%rcx

 
 
 
sub  %rax,%r11

 
 
 
sub  %r10,%r12

 
 
 
movq %r13,136(%rsp)

 
 
 
movq %r14,144(%rsp)

 
 
 
movq %r15,152(%rsp)

 
 
 
movq %rbx,160(%rsp)

 
 
 
movq %rbp,168(%rsp)

 
 
 
movq %rsi,176(%rsp)

 
 
 
movq %rdx,184(%rsp)

 
 
 
movq %rcx,192(%rsp)

 
 
 
movq %r11,200(%rsp)

 
 
 
movq %r12,208(%rsp)

 
 
 
movq 120(%rsp),%rsi

 
 
 
imulq  $19,%rsi,%rax

 
 
 
movq %rax,216(%rsp)

 
 
 
mulq  192(%rsp)

 
 
 
mov  %rax,%rsi

 
 
 
mov  %rdx,%rcx

 
 
 
movq 128(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
movq %rax,224(%rsp)

 
 
 
mulq  184(%rsp)

 
 
 
add  %rax,%rsi

 
 
 
adc %rdx,%rcx

 
 
 
movq 96(%rsp),%rax

 
 
 
mulq  176(%rsp)

 
 
 
add  %rax,%rsi

 
 
 
adc %rdx,%rcx

 
 
 
movq 96(%rsp),%rax

 
 
 
mulq  184(%rsp)

 
 
 
mov  %rax,%r8

 
 
 
mov  %rdx,%r9

 
 
 
movq 96(%rsp),%rax

 
 
 
mulq  192(%rsp)

 
 
 
mov  %rax,%r10

 
 
 
mov  %rdx,%r11

 
 
 
movq 96(%rsp),%rax

 
 
 
mulq  200(%rsp)

 
 
 
mov  %rax,%r12

 
 
 
mov  %rdx,%r13

 
 
 
movq 96(%rsp),%rax

 
 
 
mulq  208(%rsp)

 
 
 
mov  %rax,%r14

 
 
 
mov  %rdx,%r15

 
 
 
movq 104(%rsp),%rax

 
 
 
mulq  176(%rsp)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 104(%rsp),%rax

 
 
 
mulq  184(%rsp)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 104(%rsp),%rax

 
 
 
mulq  192(%rsp)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq 104(%rsp),%rax

 
 
 
mulq  200(%rsp)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq 104(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  208(%rsp)

 
 
 
add  %rax,%rsi

 
 
 
adc %rdx,%rcx

 
 
 
movq 112(%rsp),%rax

 
 
 
mulq  176(%rsp)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 112(%rsp),%rax

 
 
 
mulq  184(%rsp)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq 112(%rsp),%rax

 
 
 
mulq  192(%rsp)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq 112(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  200(%rsp)

 
 
 
add  %rax,%rsi

 
 
 
adc %rdx,%rcx

 
 
 
movq 112(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  208(%rsp)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 120(%rsp),%rax

 
 
 
mulq  176(%rsp)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq 120(%rsp),%rax

 
 
 
mulq  184(%rsp)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq 216(%rsp),%rax

 
 
 
mulq  200(%rsp)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 216(%rsp),%rax

 
 
 
mulq  208(%rsp)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 128(%rsp),%rax

 
 
 
mulq  176(%rsp)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq 224(%rsp),%rax

 
 
 
mulq  192(%rsp)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 224(%rsp),%rax

 
 
 
mulq  200(%rsp)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 224(%rsp),%rax

 
 
 
mulq  208(%rsp)

 
 
 
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

 
 
 
mov  %rcx,%r8

 
 
 
shr  $51,%rcx

 
 
 
and  %rdx,%rsi

 
 
 
add  %r10,%rcx

 
 
 
mov  %rcx,%r9

 
 
 
shr  $51,%rcx

 
 
 
and  %rdx,%r8

 
 
 
add  %r12,%rcx

 
 
 
mov  %rcx,%rax

 
 
 
shr  $51,%rcx

 
 
 
and  %rdx,%r9

 
 
 
add  %r14,%rcx

 
 
 
mov  %rcx,%r10

 
 
 
shr  $51,%rcx

 
 
 
and  %rdx,%rax

 
 
 
imulq  $19,%rcx,%rcx

 
 
 
add  %rcx,%rsi

 
 
 
and  %rdx,%r10

 
 
 
movq   %rsi,0(%rdi)

 
 
 
movq   %r8,8(%rdi)

 
 
 
movq   %r9,16(%rdi)

 
 
 
movq   %rax,24(%rdi)

 
 
 
movq   %r10,32(%rdi)

 
 
 
movq 80(%rsp),%rsi

 
 
 
imulq  $19,%rsi,%rax

 
 
 
movq %rax,216(%rsp)

 
 
 
mulq  152(%rsp)

 
 
 
mov  %rax,%rsi

 
 
 
mov  %rdx,%rcx

 
 
 
movq 88(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
movq %rax,224(%rsp)

 
 
 
mulq  144(%rsp)

 
 
 
add  %rax,%rsi

 
 
 
adc %rdx,%rcx

 
 
 
movq 56(%rsp),%rax

 
 
 
mulq  136(%rsp)

 
 
 
add  %rax,%rsi

 
 
 
adc %rdx,%rcx

 
 
 
movq 56(%rsp),%rax

 
 
 
mulq  144(%rsp)

 
 
 
mov  %rax,%r8

 
 
 
mov  %rdx,%r9

 
 
 
movq 56(%rsp),%rax

 
 
 
mulq  152(%rsp)

 
 
 
mov  %rax,%r10

 
 
 
mov  %rdx,%r11

 
 
 
movq 56(%rsp),%rax

 
 
 
mulq  160(%rsp)

 
 
 
mov  %rax,%r12

 
 
 
mov  %rdx,%r13

 
 
 
movq 56(%rsp),%rax

 
 
 
mulq  168(%rsp)

 
 
 
mov  %rax,%r14

 
 
 
mov  %rdx,%r15

 
 
 
movq 64(%rsp),%rax

 
 
 
mulq  136(%rsp)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 64(%rsp),%rax

 
 
 
mulq  144(%rsp)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 64(%rsp),%rax

 
 
 
mulq  152(%rsp)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq 64(%rsp),%rax

 
 
 
mulq  160(%rsp)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq 64(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  168(%rsp)

 
 
 
add  %rax,%rsi

 
 
 
adc %rdx,%rcx

 
 
 
movq 72(%rsp),%rax

 
 
 
mulq  136(%rsp)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 72(%rsp),%rax

 
 
 
mulq  144(%rsp)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq 72(%rsp),%rax

 
 
 
mulq  152(%rsp)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq 72(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  160(%rsp)

 
 
 
add  %rax,%rsi

 
 
 
adc %rdx,%rcx

 
 
 
movq 72(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  168(%rsp)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 80(%rsp),%rax

 
 
 
mulq  136(%rsp)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq 80(%rsp),%rax

 
 
 
mulq  144(%rsp)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq 216(%rsp),%rax

 
 
 
mulq  160(%rsp)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 216(%rsp),%rax

 
 
 
mulq  168(%rsp)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 88(%rsp),%rax

 
 
 
mulq  136(%rsp)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq 224(%rsp),%rax

 
 
 
mulq  152(%rsp)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 224(%rsp),%rax

 
 
 
mulq  160(%rsp)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 224(%rsp),%rax

 
 
 
mulq  168(%rsp)

 
 
 
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

 
 
 
mov  %rcx,%r8

 
 
 
shr  $51,%rcx

 
 
 
and  %rdx,%rsi

 
 
 
add  %r10,%rcx

 
 
 
mov  %rcx,%r9

 
 
 
shr  $51,%rcx

 
 
 
and  %rdx,%r8

 
 
 
add  %r12,%rcx

 
 
 
mov  %rcx,%rax

 
 
 
shr  $51,%rcx

 
 
 
and  %rdx,%r9

 
 
 
add  %r14,%rcx

 
 
 
mov  %rcx,%r10

 
 
 
shr  $51,%rcx

 
 
 
and  %rdx,%rax

 
 
 
imulq  $19,%rcx,%rcx

 
 
 
add  %rcx,%rsi

 
 
 
and  %rdx,%r10

 
 
 
movq   %rsi,40(%rdi)

 
 
 
movq   %r8,48(%rdi)

 
 
 
movq   %r9,56(%rdi)

 
 
 
movq   %rax,64(%rdi)

 
 
 
movq   %r10,72(%rdi)

 
 
 
movq 160(%rsp),%rsi

 
 
 
imulq  $19,%rsi,%rax

 
 
 
movq %rax,216(%rsp)

 
 
 
mulq  192(%rsp)

 
 
 
mov  %rax,%rsi

 
 
 
mov  %rdx,%rcx

 
 
 
movq 168(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
movq %rax,224(%rsp)

 
 
 
mulq  184(%rsp)

 
 
 
add  %rax,%rsi

 
 
 
adc %rdx,%rcx

 
 
 
movq 136(%rsp),%rax

 
 
 
mulq  176(%rsp)

 
 
 
add  %rax,%rsi

 
 
 
adc %rdx,%rcx

 
 
 
movq 136(%rsp),%rax

 
 
 
mulq  184(%rsp)

 
 
 
mov  %rax,%r8

 
 
 
mov  %rdx,%r9

 
 
 
movq 136(%rsp),%rax

 
 
 
mulq  192(%rsp)

 
 
 
mov  %rax,%r10

 
 
 
mov  %rdx,%r11

 
 
 
movq 136(%rsp),%rax

 
 
 
mulq  200(%rsp)

 
 
 
mov  %rax,%r12

 
 
 
mov  %rdx,%r13

 
 
 
movq 136(%rsp),%rax

 
 
 
mulq  208(%rsp)

 
 
 
mov  %rax,%r14

 
 
 
mov  %rdx,%r15

 
 
 
movq 144(%rsp),%rax

 
 
 
mulq  176(%rsp)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 144(%rsp),%rax

 
 
 
mulq  184(%rsp)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 144(%rsp),%rax

 
 
 
mulq  192(%rsp)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq 144(%rsp),%rax

 
 
 
mulq  200(%rsp)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq 144(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  208(%rsp)

 
 
 
add  %rax,%rsi

 
 
 
adc %rdx,%rcx

 
 
 
movq 152(%rsp),%rax

 
 
 
mulq  176(%rsp)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 152(%rsp),%rax

 
 
 
mulq  184(%rsp)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq 152(%rsp),%rax

 
 
 
mulq  192(%rsp)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq 152(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  200(%rsp)

 
 
 
add  %rax,%rsi

 
 
 
adc %rdx,%rcx

 
 
 
movq 152(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  208(%rsp)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 160(%rsp),%rax

 
 
 
mulq  176(%rsp)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq 160(%rsp),%rax

 
 
 
mulq  184(%rsp)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq 216(%rsp),%rax

 
 
 
mulq  200(%rsp)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 216(%rsp),%rax

 
 
 
mulq  208(%rsp)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 168(%rsp),%rax

 
 
 
mulq  176(%rsp)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq 224(%rsp),%rax

 
 
 
mulq  192(%rsp)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 224(%rsp),%rax

 
 
 
mulq  200(%rsp)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 224(%rsp),%rax

 
 
 
mulq  208(%rsp)

 
 
 
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

 
 
 
mov  %rcx,%r8

 
 
 
shr  $51,%rcx

 
 
 
and  %rdx,%rsi

 
 
 
add  %r10,%rcx

 
 
 
mov  %rcx,%r9

 
 
 
shr  $51,%rcx

 
 
 
and  %rdx,%r8

 
 
 
add  %r12,%rcx

 
 
 
mov  %rcx,%rax

 
 
 
shr  $51,%rcx

 
 
 
and  %rdx,%r9

 
 
 
add  %r14,%rcx

 
 
 
mov  %rcx,%r10

 
 
 
shr  $51,%rcx

 
 
 
and  %rdx,%rax

 
 
 
imulq  $19,%rcx,%rcx

 
 
 
add  %rcx,%rsi

 
 
 
and  %rdx,%r10

 
 
 
movq   %rsi,80(%rdi)

 
 
 
movq   %r8,88(%rdi)

 
 
 
movq   %r9,96(%rdi)

 
 
 
movq   %rax,104(%rdi)

 
 
 
movq   %r10,112(%rdi)

 
 
 
movq 120(%rsp),%rsi

 
 
 
imulq  $19,%rsi,%rax

 
 
 
movq %rax,136(%rsp)

 
 
 
mulq  72(%rsp)

 
 
 
mov  %rax,%rsi

 
 
 
mov  %rdx,%rcx

 
 
 
movq 128(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
movq %rax,144(%rsp)

 
 
 
mulq  64(%rsp)

 
 
 
add  %rax,%rsi

 
 
 
adc %rdx,%rcx

 
 
 
movq 96(%rsp),%rax

 
 
 
mulq  56(%rsp)

 
 
 
add  %rax,%rsi

 
 
 
adc %rdx,%rcx

 
 
 
movq 96(%rsp),%rax

 
 
 
mulq  64(%rsp)

 
 
 
mov  %rax,%r8

 
 
 
mov  %rdx,%r9

 
 
 
movq 96(%rsp),%rax

 
 
 
mulq  72(%rsp)

 
 
 
mov  %rax,%r10

 
 
 
mov  %rdx,%r11

 
 
 
movq 96(%rsp),%rax

 
 
 
mulq  80(%rsp)

 
 
 
mov  %rax,%r12

 
 
 
mov  %rdx,%r13

 
 
 
movq 96(%rsp),%rax

 
 
 
mulq  88(%rsp)

 
 
 
mov  %rax,%r14

 
 
 
mov  %rdx,%r15

 
 
 
movq 104(%rsp),%rax

 
 
 
mulq  56(%rsp)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 104(%rsp),%rax

 
 
 
mulq  64(%rsp)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 104(%rsp),%rax

 
 
 
mulq  72(%rsp)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq 104(%rsp),%rax

 
 
 
mulq  80(%rsp)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq 104(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  88(%rsp)

 
 
 
add  %rax,%rsi

 
 
 
adc %rdx,%rcx

 
 
 
movq 112(%rsp),%rax

 
 
 
mulq  56(%rsp)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 112(%rsp),%rax

 
 
 
mulq  64(%rsp)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq 112(%rsp),%rax

 
 
 
mulq  72(%rsp)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq 112(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  80(%rsp)

 
 
 
add  %rax,%rsi

 
 
 
adc %rdx,%rcx

 
 
 
movq 112(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  88(%rsp)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 120(%rsp),%rax

 
 
 
mulq  56(%rsp)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq 120(%rsp),%rax

 
 
 
mulq  64(%rsp)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq 136(%rsp),%rax

 
 
 
mulq  80(%rsp)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 136(%rsp),%rax

 
 
 
mulq  88(%rsp)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 128(%rsp),%rax

 
 
 
mulq  56(%rsp)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq 144(%rsp),%rax

 
 
 
mulq  72(%rsp)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 144(%rsp),%rax

 
 
 
mulq  80(%rsp)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 144(%rsp),%rax

 
 
 
mulq  88(%rsp)

 
 
 
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

 
 
 
mov  %rcx,%r8

 
 
 
shr  $51,%rcx

 
 
 
and  %rdx,%rsi

 
 
 
add  %r10,%rcx

 
 
 
mov  %rcx,%r9

 
 
 
shr  $51,%rcx

 
 
 
and  %rdx,%r8

 
 
 
add  %r12,%rcx

 
 
 
mov  %rcx,%rax

 
 
 
shr  $51,%rcx

 
 
 
and  %rdx,%r9

 
 
 
add  %r14,%rcx

 
 
 
mov  %rcx,%r10

 
 
 
shr  $51,%rcx

 
 
 
and  %rdx,%rax

 
 
 
imulq  $19,%rcx,%rcx

 
 
 
add  %rcx,%rsi

 
 
 
and  %rdx,%r10

 
 
 
movq   %rsi,120(%rdi)

 
 
 
movq   %r8,128(%rdi)

 
 
 
movq   %r9,136(%rdi)

 
 
 
movq   %rax,144(%rdi)

 
 
 
movq   %r10,152(%rdi)

 
 
 
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
