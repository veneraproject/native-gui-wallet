
 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 
.text
.p2align 5
.globl _crypto_sign_ed25519_amd64_51_30k_batch_ge25519_add_p1p1
.globl crypto_sign_ed25519_amd64_51_30k_batch_ge25519_add_p1p1
_crypto_sign_ed25519_amd64_51_30k_batch_ge25519_add_p1p1:
crypto_sign_ed25519_amd64_51_30k_batch_ge25519_add_p1p1:
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

 
 
 
mov  %rdx,%rcx

 
 
 
movq   40(%rsi),%rdx

 
 
 
movq   48(%rsi),%r8

 
 
 
movq   56(%rsi),%r9

 
 
 
movq   64(%rsi),%rax

 
 
 
movq   72(%rsi),%r10

 
 
 
mov  %rdx,%r11

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P0(%rip),%rdx

 
 
 
movq   0(%rsi),%r12

 
 
 
add  %r12,%r11

 
 
 
sub  %r12,%rdx

 
 
 
mov  %r8,%r12

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%r8

 
 
 
movq   8(%rsi),%r13

 
 
 
add  %r13,%r12

 
 
 
sub  %r13,%r8

 
 
 
mov  %r9,%r13

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%r9

 
 
 
movq   16(%rsi),%r14

 
 
 
add  %r14,%r13

 
 
 
sub  %r14,%r9

 
 
 
mov  %rax,%r14

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%rax

 
 
 
movq   24(%rsi),%r15

 
 
 
add  %r15,%r14

 
 
 
sub  %r15,%rax

 
 
 
mov  %r10,%r15

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%r10

 
 
 
movq   32(%rsi),%rbx

 
 
 
add  %rbx,%r15

 
 
 
sub  %rbx,%r10

 
 
 
movq %rdx,56(%rsp)

 
 
 
movq %r8,64(%rsp)

 
 
 
movq %r9,72(%rsp)

 
 
 
movq %rax,80(%rsp)

 
 
 
movq %r10,88(%rsp)

 
 
 
movq %r11,96(%rsp)

 
 
 
movq %r12,104(%rsp)

 
 
 
movq %r13,112(%rsp)

 
 
 
movq %r14,120(%rsp)

 
 
 
movq %r15,128(%rsp)

 
 
 
movq   40(%rcx),%rdx

 
 
 
movq   48(%rcx),%r8

 
 
 
movq   56(%rcx),%r9

 
 
 
movq   64(%rcx),%rax

 
 
 
movq   72(%rcx),%r10

 
 
 
mov  %rdx,%r11

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P0(%rip),%rdx

 
 
 
movq   0(%rcx),%r12

 
 
 
add  %r12,%r11

 
 
 
sub  %r12,%rdx

 
 
 
mov  %r8,%r12

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%r8

 
 
 
movq   8(%rcx),%r13

 
 
 
add  %r13,%r12

 
 
 
sub  %r13,%r8

 
 
 
mov  %r9,%r13

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%r9

 
 
 
movq   16(%rcx),%r14

 
 
 
add  %r14,%r13

 
 
 
sub  %r14,%r9

 
 
 
mov  %rax,%r14

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%rax

 
 
 
movq   24(%rcx),%r15

 
 
 
add  %r15,%r14

 
 
 
sub  %r15,%rax

 
 
 
mov  %r10,%r15

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%r10

 
 
 
movq   32(%rcx),%rbx

 
 
 
add  %rbx,%r15

 
 
 
sub  %rbx,%r10

 
 
 
movq %rdx,136(%rsp)

 
 
 
movq %r8,144(%rsp)

 
 
 
movq %r9,152(%rsp)

 
 
 
movq %rax,160(%rsp)

 
 
 
movq %r10,168(%rsp)

 
 
 
movq %r11,176(%rsp)

 
 
 
movq %r12,184(%rsp)

 
 
 
movq %r13,192(%rsp)

 
 
 
movq %r14,200(%rsp)

 
 
 
movq %r15,208(%rsp)

 
 
 
movq 80(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
movq %rax,216(%rsp)

 
 
 
mulq  152(%rsp)

 
 
 
mov  %rax,%r8

 
 
 
mov  %rdx,%r9

 
 
 
movq 88(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
movq %rax,224(%rsp)

 
 
 
mulq  144(%rsp)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 56(%rsp),%rax

 
 
 
mulq  136(%rsp)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 56(%rsp),%rax

 
 
 
mulq  144(%rsp)

 
 
 
mov  %rax,%r10

 
 
 
mov  %rdx,%r11

 
 
 
movq 56(%rsp),%rax

 
 
 
mulq  152(%rsp)

 
 
 
mov  %rax,%r12

 
 
 
mov  %rdx,%r13

 
 
 
movq 56(%rsp),%rax

 
 
 
mulq  160(%rsp)

 
 
 
mov  %rax,%r14

 
 
 
mov  %rdx,%r15

 
 
 
movq 56(%rsp),%rax

 
 
 
mulq  168(%rsp)

 
 
 
mov  %rax,%rbx

 
 
 
mov  %rdx,%rbp

 
 
 
movq 64(%rsp),%rax

 
 
 
mulq  136(%rsp)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 64(%rsp),%rax

 
 
 
mulq  144(%rsp)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq 64(%rsp),%rax

 
 
 
mulq  152(%rsp)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq 64(%rsp),%rax

 
 
 
mulq  160(%rsp)

 
 
 
add  %rax,%rbx

 
 
 
adc %rdx,%rbp

 
 
 
movq 64(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  168(%rsp)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 72(%rsp),%rax

 
 
 
mulq  136(%rsp)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq 72(%rsp),%rax

 
 
 
mulq  144(%rsp)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq 72(%rsp),%rax

 
 
 
mulq  152(%rsp)

 
 
 
add  %rax,%rbx

 
 
 
adc %rdx,%rbp

 
 
 
movq 72(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  160(%rsp)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 72(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  168(%rsp)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 80(%rsp),%rax

 
 
 
mulq  136(%rsp)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq 80(%rsp),%rax

 
 
 
mulq  144(%rsp)

 
 
 
add  %rax,%rbx

 
 
 
adc %rdx,%rbp

 
 
 
movq 216(%rsp),%rax

 
 
 
mulq  160(%rsp)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 216(%rsp),%rax

 
 
 
mulq  168(%rsp)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq 88(%rsp),%rax

 
 
 
mulq  136(%rsp)

 
 
 
add  %rax,%rbx

 
 
 
adc %rdx,%rbp

 
 
 
movq 224(%rsp),%rax

 
 
 
mulq  152(%rsp)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 224(%rsp),%rax

 
 
 
mulq  160(%rsp)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq 224(%rsp),%rax

 
 
 
mulq  168(%rsp)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq crypto_sign_ed25519_amd64_51_30k_batch_REDMASK51(%rip),%rdx

 
 
 
shld $13,%r8,%r9

 
 
 
and  %rdx,%r8

 
 
 
shld $13,%r10,%r11

 
 
 
and  %rdx,%r10

 
 
 
add  %r9,%r10

 
 
 
shld $13,%r12,%r13

 
 
 
and  %rdx,%r12

 
 
 
add  %r11,%r12

 
 
 
shld $13,%r14,%r15

 
 
 
and  %rdx,%r14

 
 
 
add  %r13,%r14

 
 
 
shld $13,%rbx,%rbp

 
 
 
and  %rdx,%rbx

 
 
 
add  %r15,%rbx

 
 
 
imulq  $19,%rbp,%r9

 
 
 
add  %r9,%r8

 
 
 
mov  %r8,%r9

 
 
 
shr  $51,%r9

 
 
 
add  %r10,%r9

 
 
 
mov  %r9,%rax

 
 
 
shr  $51,%r9

 
 
 
and  %rdx,%r8

 
 
 
add  %r12,%r9

 
 
 
mov  %r9,%r10

 
 
 
shr  $51,%r9

 
 
 
and  %rdx,%rax

 
 
 
add  %r14,%r9

 
 
 
mov  %r9,%r11

 
 
 
shr  $51,%r9

 
 
 
and  %rdx,%r10

 
 
 
add  %rbx,%r9

 
 
 
mov  %r9,%r12

 
 
 
shr  $51,%r9

 
 
 
and  %rdx,%r11

 
 
 
imulq  $19,%r9,%r9

 
 
 
add  %r9,%r8

 
 
 
and  %rdx,%r12

 
 
 
movq %r8,56(%rsp)

 
 
 
movq %rax,64(%rsp)

 
 
 
movq %r10,72(%rsp)

 
 
 
movq %r11,80(%rsp)

 
 
 
movq %r12,88(%rsp)

 
 
 
movq 120(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
movq %rax,136(%rsp)

 
 
 
mulq  192(%rsp)

 
 
 
mov  %rax,%r8

 
 
 
mov  %rdx,%r9

 
 
 
movq 128(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
movq %rax,144(%rsp)

 
 
 
mulq  184(%rsp)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 96(%rsp),%rax

 
 
 
mulq  176(%rsp)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 96(%rsp),%rax

 
 
 
mulq  184(%rsp)

 
 
 
mov  %rax,%r10

 
 
 
mov  %rdx,%r11

 
 
 
movq 96(%rsp),%rax

 
 
 
mulq  192(%rsp)

 
 
 
mov  %rax,%r12

 
 
 
mov  %rdx,%r13

 
 
 
movq 96(%rsp),%rax

 
 
 
mulq  200(%rsp)

 
 
 
mov  %rax,%r14

 
 
 
mov  %rdx,%r15

 
 
 
movq 96(%rsp),%rax

 
 
 
mulq  208(%rsp)

 
 
 
mov  %rax,%rbx

 
 
 
mov  %rdx,%rbp

 
 
 
movq 104(%rsp),%rax

 
 
 
mulq  176(%rsp)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 104(%rsp),%rax

 
 
 
mulq  184(%rsp)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq 104(%rsp),%rax

 
 
 
mulq  192(%rsp)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq 104(%rsp),%rax

 
 
 
mulq  200(%rsp)

 
 
 
add  %rax,%rbx

 
 
 
adc %rdx,%rbp

 
 
 
movq 104(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  208(%rsp)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 112(%rsp),%rax

 
 
 
mulq  176(%rsp)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq 112(%rsp),%rax

 
 
 
mulq  184(%rsp)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq 112(%rsp),%rax

 
 
 
mulq  192(%rsp)

 
 
 
add  %rax,%rbx

 
 
 
adc %rdx,%rbp

 
 
 
movq 112(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  200(%rsp)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 112(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  208(%rsp)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 120(%rsp),%rax

 
 
 
mulq  176(%rsp)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq 120(%rsp),%rax

 
 
 
mulq  184(%rsp)

 
 
 
add  %rax,%rbx

 
 
 
adc %rdx,%rbp

 
 
 
movq 136(%rsp),%rax

 
 
 
mulq  200(%rsp)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 136(%rsp),%rax

 
 
 
mulq  208(%rsp)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq 128(%rsp),%rax

 
 
 
mulq  176(%rsp)

 
 
 
add  %rax,%rbx

 
 
 
adc %rdx,%rbp

 
 
 
movq 144(%rsp),%rax

 
 
 
mulq  192(%rsp)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 144(%rsp),%rax

 
 
 
mulq  200(%rsp)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq 144(%rsp),%rax

 
 
 
mulq  208(%rsp)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq crypto_sign_ed25519_amd64_51_30k_batch_REDMASK51(%rip),%rdx

 
 
 
shld $13,%r8,%r9

 
 
 
and  %rdx,%r8

 
 
 
shld $13,%r10,%r11

 
 
 
and  %rdx,%r10

 
 
 
add  %r9,%r10

 
 
 
shld $13,%r12,%r13

 
 
 
and  %rdx,%r12

 
 
 
add  %r11,%r12

 
 
 
shld $13,%r14,%r15

 
 
 
and  %rdx,%r14

 
 
 
add  %r13,%r14

 
 
 
shld $13,%rbx,%rbp

 
 
 
and  %rdx,%rbx

 
 
 
add  %r15,%rbx

 
 
 
imulq  $19,%rbp,%r9

 
 
 
add  %r9,%r8

 
 
 
mov  %r8,%r9

 
 
 
shr  $51,%r9

 
 
 
add  %r10,%r9

 
 
 
mov  %r9,%rax

 
 
 
shr  $51,%r9

 
 
 
and  %rdx,%r8

 
 
 
add  %r12,%r9

 
 
 
mov  %r9,%r10

 
 
 
shr  $51,%r9

 
 
 
and  %rdx,%rax

 
 
 
add  %r14,%r9

 
 
 
mov  %r9,%r11

 
 
 
shr  $51,%r9

 
 
 
and  %rdx,%r10

 
 
 
add  %rbx,%r9

 
 
 
mov  %r9,%r12

 
 
 
shr  $51,%r9

 
 
 
and  %rdx,%r11

 
 
 
imulq  $19,%r9,%r9

 
 
 
add  %r9,%r8

 
 
 
and  %rdx,%r12

 
 
 
mov  %r8,%rdx

 
 
 
mov  %rax,%r9

 
 
 
mov  %r10,%r13

 
 
 
mov  %r11,%r14

 
 
 
mov  %r12,%r15

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P0(%rip),%r8

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%rax

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%r10

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%r11

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%r12

 
 
 
addq 56(%rsp),%rdx

 
 
 
addq 64(%rsp),%r9

 
 
 
addq 72(%rsp),%r13

 
 
 
addq 80(%rsp),%r14

 
 
 
addq 88(%rsp),%r15

 
 
 
subq 56(%rsp),%r8

 
 
 
subq 64(%rsp),%rax

 
 
 
subq 72(%rsp),%r10

 
 
 
subq 80(%rsp),%r11

 
 
 
subq 88(%rsp),%r12

 
 
 
movq   %r8,0(%rdi)

 
 
 
movq   %rax,8(%rdi)

 
 
 
movq   %r10,16(%rdi)

 
 
 
movq   %r11,24(%rdi)

 
 
 
movq   %r12,32(%rdi)

 
 
 
movq   %rdx,80(%rdi)

 
 
 
movq   %r9,88(%rdi)

 
 
 
movq   %r13,96(%rdi)

 
 
 
movq   %r14,104(%rdi)

 
 
 
movq   %r15,112(%rdi)

 
 
 
movq   144(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
movq %rax,56(%rsp)

 
 
 
mulq  136(%rcx)

 
 
 
mov  %rax,%r8

 
 
 
mov  %rdx,%r9

 
 
 
movq   152(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
movq %rax,64(%rsp)

 
 
 
mulq  128(%rcx)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq   120(%rsi),%rax

 
 
 
mulq  120(%rcx)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq   120(%rsi),%rax

 
 
 
mulq  128(%rcx)

 
 
 
mov  %rax,%r10

 
 
 
mov  %rdx,%r11

 
 
 
movq   120(%rsi),%rax

 
 
 
mulq  136(%rcx)

 
 
 
mov  %rax,%r12

 
 
 
mov  %rdx,%r13

 
 
 
movq   120(%rsi),%rax

 
 
 
mulq  144(%rcx)

 
 
 
mov  %rax,%r14

 
 
 
mov  %rdx,%r15

 
 
 
movq   120(%rsi),%rax

 
 
 
mulq  152(%rcx)

 
 
 
mov  %rax,%rbx

 
 
 
mov  %rdx,%rbp

 
 
 
movq   128(%rsi),%rax

 
 
 
mulq  120(%rcx)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq   128(%rsi),%rax

 
 
 
mulq  128(%rcx)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq   128(%rsi),%rax

 
 
 
mulq  136(%rcx)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq   128(%rsi),%rax

 
 
 
mulq  144(%rcx)

 
 
 
add  %rax,%rbx

 
 
 
adc %rdx,%rbp

 
 
 
movq   128(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  152(%rcx)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq   136(%rsi),%rax

 
 
 
mulq  120(%rcx)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq   136(%rsi),%rax

 
 
 
mulq  128(%rcx)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq   136(%rsi),%rax

 
 
 
mulq  136(%rcx)

 
 
 
add  %rax,%rbx

 
 
 
adc %rdx,%rbp

 
 
 
movq   136(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  144(%rcx)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq   136(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  152(%rcx)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq   144(%rsi),%rax

 
 
 
mulq  120(%rcx)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq   144(%rsi),%rax

 
 
 
mulq  128(%rcx)

 
 
 
add  %rax,%rbx

 
 
 
adc %rdx,%rbp

 
 
 
movq 56(%rsp),%rax

 
 
 
mulq  144(%rcx)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 56(%rsp),%rax

 
 
 
mulq  152(%rcx)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq   152(%rsi),%rax

 
 
 
mulq  120(%rcx)

 
 
 
add  %rax,%rbx

 
 
 
adc %rdx,%rbp

 
 
 
movq 64(%rsp),%rax

 
 
 
mulq  136(%rcx)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 64(%rsp),%rax

 
 
 
mulq  144(%rcx)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq 64(%rsp),%rax

 
 
 
mulq  152(%rcx)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq crypto_sign_ed25519_amd64_51_30k_batch_REDMASK51(%rip),%rdx

 
 
 
shld $13,%r8,%r9

 
 
 
and  %rdx,%r8

 
 
 
shld $13,%r10,%r11

 
 
 
and  %rdx,%r10

 
 
 
add  %r9,%r10

 
 
 
shld $13,%r12,%r13

 
 
 
and  %rdx,%r12

 
 
 
add  %r11,%r12

 
 
 
shld $13,%r14,%r15

 
 
 
and  %rdx,%r14

 
 
 
add  %r13,%r14

 
 
 
shld $13,%rbx,%rbp

 
 
 
and  %rdx,%rbx

 
 
 
add  %r15,%rbx

 
 
 
imulq  $19,%rbp,%r9

 
 
 
add  %r9,%r8

 
 
 
mov  %r8,%r9

 
 
 
shr  $51,%r9

 
 
 
add  %r10,%r9

 
 
 
mov  %r9,%rax

 
 
 
shr  $51,%r9

 
 
 
and  %rdx,%r8

 
 
 
add  %r12,%r9

 
 
 
mov  %r9,%r10

 
 
 
shr  $51,%r9

 
 
 
and  %rdx,%rax

 
 
 
add  %r14,%r9

 
 
 
mov  %r9,%r11

 
 
 
shr  $51,%r9

 
 
 
and  %rdx,%r10

 
 
 
add  %rbx,%r9

 
 
 
mov  %r9,%r12

 
 
 
shr  $51,%r9

 
 
 
and  %rdx,%r11

 
 
 
imulq  $19,%r9,%r9

 
 
 
add  %r9,%r8

 
 
 
and  %rdx,%r12

 
 
 
movq %r8,56(%rsp)

 
 
 
movq %rax,64(%rsp)

 
 
 
movq %r10,72(%rsp)

 
 
 
movq %r11,80(%rsp)

 
 
 
movq %r12,88(%rsp)

 
 
 
movq 80(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
movq %rax,96(%rsp)

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D2(%rip)

 
 
 
mov  %rax,%r8

 
 
 
mov  %rdx,%r9

 
 
 
movq 88(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
movq %rax,104(%rsp)

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D1(%rip)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 56(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D0(%rip)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 56(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D1(%rip)

 
 
 
mov  %rax,%r10

 
 
 
mov  %rdx,%r11

 
 
 
movq 56(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D2(%rip)

 
 
 
mov  %rax,%r12

 
 
 
mov  %rdx,%r13

 
 
 
movq 56(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D3(%rip)

 
 
 
mov  %rax,%r14

 
 
 
mov  %rdx,%r15

 
 
 
movq 56(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D4(%rip)

 
 
 
mov  %rax,%rbx

 
 
 
mov  %rdx,%rbp

 
 
 
movq 64(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D0(%rip)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 64(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D1(%rip)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq 64(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D2(%rip)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq 64(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D3(%rip)

 
 
 
add  %rax,%rbx

 
 
 
adc %rdx,%rbp

 
 
 
movq 64(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D4(%rip)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 72(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D0(%rip)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq 72(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D1(%rip)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq 72(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D2(%rip)

 
 
 
add  %rax,%rbx

 
 
 
adc %rdx,%rbp

 
 
 
movq 72(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D3(%rip)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 72(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D4(%rip)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 80(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D0(%rip)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq 80(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D1(%rip)

 
 
 
add  %rax,%rbx

 
 
 
adc %rdx,%rbp

 
 
 
movq 96(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D3(%rip)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 96(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D4(%rip)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq 88(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D0(%rip)

 
 
 
add  %rax,%rbx

 
 
 
adc %rdx,%rbp

 
 
 
movq 104(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D2(%rip)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 104(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D3(%rip)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq 104(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D4(%rip)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq crypto_sign_ed25519_amd64_51_30k_batch_REDMASK51(%rip),%rdx

 
 
 
shld $13,%r8,%r9

 
 
 
and  %rdx,%r8

 
 
 
shld $13,%r10,%r11

 
 
 
and  %rdx,%r10

 
 
 
add  %r9,%r10

 
 
 
shld $13,%r12,%r13

 
 
 
and  %rdx,%r12

 
 
 
add  %r11,%r12

 
 
 
shld $13,%r14,%r15

 
 
 
and  %rdx,%r14

 
 
 
add  %r13,%r14

 
 
 
shld $13,%rbx,%rbp

 
 
 
and  %rdx,%rbx

 
 
 
add  %r15,%rbx

 
 
 
imulq  $19,%rbp,%r9

 
 
 
add  %r9,%r8

 
 
 
mov  %r8,%r9

 
 
 
shr  $51,%r9

 
 
 
add  %r10,%r9

 
 
 
mov  %r9,%rax

 
 
 
shr  $51,%r9

 
 
 
and  %rdx,%r8

 
 
 
add  %r12,%r9

 
 
 
mov  %r9,%r10

 
 
 
shr  $51,%r9

 
 
 
and  %rdx,%rax

 
 
 
add  %r14,%r9

 
 
 
mov  %r9,%r11

 
 
 
shr  $51,%r9

 
 
 
and  %rdx,%r10

 
 
 
add  %rbx,%r9

 
 
 
mov  %r9,%r12

 
 
 
shr  $51,%r9

 
 
 
and  %rdx,%r11

 
 
 
imulq  $19,%r9,%r9

 
 
 
add  %r9,%r8

 
 
 
and  %rdx,%r12

 
 
 
movq %r8,56(%rsp)

 
 
 
movq %rax,64(%rsp)

 
 
 
movq %r10,72(%rsp)

 
 
 
movq %r11,80(%rsp)

 
 
 
movq %r12,88(%rsp)

 
 
 
movq   104(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
movq %rax,96(%rsp)

 
 
 
mulq  96(%rcx)

 
 
 
mov  %rax,%r8

 
 
 
mov  %rdx,%r9

 
 
 
movq   112(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
movq %rax,104(%rsp)

 
 
 
mulq  88(%rcx)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq   80(%rsi),%rax

 
 
 
mulq  80(%rcx)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq   80(%rsi),%rax

 
 
 
mulq  88(%rcx)

 
 
 
mov  %rax,%r10

 
 
 
mov  %rdx,%r11

 
 
 
movq   80(%rsi),%rax

 
 
 
mulq  96(%rcx)

 
 
 
mov  %rax,%r12

 
 
 
mov  %rdx,%r13

 
 
 
movq   80(%rsi),%rax

 
 
 
mulq  104(%rcx)

 
 
 
mov  %rax,%r14

 
 
 
mov  %rdx,%r15

 
 
 
movq   80(%rsi),%rax

 
 
 
mulq  112(%rcx)

 
 
 
mov  %rax,%rbx

 
 
 
mov  %rdx,%rbp

 
 
 
movq   88(%rsi),%rax

 
 
 
mulq  80(%rcx)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq   88(%rsi),%rax

 
 
 
mulq  88(%rcx)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq   88(%rsi),%rax

 
 
 
mulq  96(%rcx)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq   88(%rsi),%rax

 
 
 
mulq  104(%rcx)

 
 
 
add  %rax,%rbx

 
 
 
adc %rdx,%rbp

 
 
 
movq   88(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  112(%rcx)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq   96(%rsi),%rax

 
 
 
mulq  80(%rcx)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq   96(%rsi),%rax

 
 
 
mulq  88(%rcx)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq   96(%rsi),%rax

 
 
 
mulq  96(%rcx)

 
 
 
add  %rax,%rbx

 
 
 
adc %rdx,%rbp

 
 
 
movq   96(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  104(%rcx)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq   96(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  112(%rcx)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq   104(%rsi),%rax

 
 
 
mulq  80(%rcx)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq   104(%rsi),%rax

 
 
 
mulq  88(%rcx)

 
 
 
add  %rax,%rbx

 
 
 
adc %rdx,%rbp

 
 
 
movq 96(%rsp),%rax

 
 
 
mulq  104(%rcx)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 96(%rsp),%rax

 
 
 
mulq  112(%rcx)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq   112(%rsi),%rax

 
 
 
mulq  80(%rcx)

 
 
 
add  %rax,%rbx

 
 
 
adc %rdx,%rbp

 
 
 
movq 104(%rsp),%rax

 
 
 
mulq  96(%rcx)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 104(%rsp),%rax

 
 
 
mulq  104(%rcx)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq 104(%rsp),%rax

 
 
 
mulq  112(%rcx)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq crypto_sign_ed25519_amd64_51_30k_batch_REDMASK51(%rip),%rsi

 
 
 
shld $13,%r8,%r9

 
 
 
and  %rsi,%r8

 
 
 
shld $13,%r10,%r11

 
 
 
and  %rsi,%r10

 
 
 
add  %r9,%r10

 
 
 
shld $13,%r12,%r13

 
 
 
and  %rsi,%r12

 
 
 
add  %r11,%r12

 
 
 
shld $13,%r14,%r15

 
 
 
and  %rsi,%r14

 
 
 
add  %r13,%r14

 
 
 
shld $13,%rbx,%rbp

 
 
 
and  %rsi,%rbx

 
 
 
add  %r15,%rbx

 
 
 
imulq  $19,%rbp,%rdx

 
 
 
add  %rdx,%r8

 
 
 
mov  %r8,%rdx

 
 
 
shr  $51,%rdx

 
 
 
add  %r10,%rdx

 
 
 
mov  %rdx,%rcx

 
 
 
shr  $51,%rdx

 
 
 
and  %rsi,%r8

 
 
 
add  %r12,%rdx

 
 
 
mov  %rdx,%r9

 
 
 
shr  $51,%rdx

 
 
 
and  %rsi,%rcx

 
 
 
add  %r14,%rdx

 
 
 
mov  %rdx,%rax

 
 
 
shr  $51,%rdx

 
 
 
and  %rsi,%r9

 
 
 
add  %rbx,%rdx

 
 
 
mov  %rdx,%r10

 
 
 
shr  $51,%rdx

 
 
 
and  %rsi,%rax

 
 
 
imulq  $19,%rdx,%rdx

 
 
 
add  %rdx,%r8

 
 
 
and  %rsi,%r10

 
 
 
add  %r8,%r8

 
 
 
add  %rcx,%rcx

 
 
 
add  %r9,%r9

 
 
 
add  %rax,%rax

 
 
 
add  %r10,%r10

 
 
 
mov  %r8,%rsi

 
 
 
mov  %rcx,%rdx

 
 
 
mov  %r9,%r11

 
 
 
mov  %rax,%r12

 
 
 
mov  %r10,%r13

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P0(%rip),%r8

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%rcx

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%r9

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%rax

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%r10

 
 
 
addq 56(%rsp),%rsi

 
 
 
addq 64(%rsp),%rdx

 
 
 
addq 72(%rsp),%r11

 
 
 
addq 80(%rsp),%r12

 
 
 
addq 88(%rsp),%r13

 
 
 
subq 56(%rsp),%r8

 
 
 
subq 64(%rsp),%rcx

 
 
 
subq 72(%rsp),%r9

 
 
 
subq 80(%rsp),%rax

 
 
 
subq 88(%rsp),%r10

 
 
 
movq   %rsi,40(%rdi)

 
 
 
movq   %rdx,48(%rdi)

 
 
 
movq   %r11,56(%rdi)

 
 
 
movq   %r12,64(%rdi)

 
 
 
movq   %r13,72(%rdi)

 
 
 
movq   %r8,120(%rdi)

 
 
 
movq   %rcx,128(%rdi)

 
 
 
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
