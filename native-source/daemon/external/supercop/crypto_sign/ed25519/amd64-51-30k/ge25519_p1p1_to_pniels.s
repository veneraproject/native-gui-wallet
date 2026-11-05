
 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 
.text
.p2align 5
.globl _crypto_sign_ed25519_amd64_51_30k_batch_ge25519_p1p1_to_pniels
.globl crypto_sign_ed25519_amd64_51_30k_batch_ge25519_p1p1_to_pniels
_crypto_sign_ed25519_amd64_51_30k_batch_ge25519_p1p1_to_pniels:
crypto_sign_ed25519_amd64_51_30k_batch_ge25519_p1p1_to_pniels:
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

 
 
 
movq   24(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
movq %rax,56(%rsp)

 
 
 
mulq  136(%rsi)

 
 
 
mov  %rax,%rcx

 
 
 
mov  %rdx,%r8

 
 
 
movq   32(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
movq %rax,64(%rsp)

 
 
 
mulq  128(%rsi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq   0(%rsi),%rax

 
 
 
mulq  120(%rsi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq   0(%rsi),%rax

 
 
 
mulq  128(%rsi)

 
 
 
mov  %rax,%r9

 
 
 
mov  %rdx,%r10

 
 
 
movq   0(%rsi),%rax

 
 
 
mulq  136(%rsi)

 
 
 
mov  %rax,%r11

 
 
 
mov  %rdx,%r12

 
 
 
movq   0(%rsi),%rax

 
 
 
mulq  144(%rsi)

 
 
 
mov  %rax,%r13

 
 
 
mov  %rdx,%r14

 
 
 
movq   0(%rsi),%rax

 
 
 
mulq  152(%rsi)

 
 
 
mov  %rax,%r15

 
 
 
mov  %rdx,%rbx

 
 
 
movq   8(%rsi),%rax

 
 
 
mulq  120(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq   8(%rsi),%rax

 
 
 
mulq  128(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq   8(%rsi),%rax

 
 
 
mulq  136(%rsi)

 
 
 
add  %rax,%r13

 
 
 
adc %rdx,%r14

 
 
 
movq   8(%rsi),%rax

 
 
 
mulq  144(%rsi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq   8(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  152(%rsi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq   16(%rsi),%rax

 
 
 
mulq  120(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq   16(%rsi),%rax

 
 
 
mulq  128(%rsi)

 
 
 
add  %rax,%r13

 
 
 
adc %rdx,%r14

 
 
 
movq   16(%rsi),%rax

 
 
 
mulq  136(%rsi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq   16(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  144(%rsi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq   16(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  152(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq   24(%rsi),%rax

 
 
 
mulq  120(%rsi)

 
 
 
add  %rax,%r13

 
 
 
adc %rdx,%r14

 
 
 
movq   24(%rsi),%rax

 
 
 
mulq  128(%rsi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq 56(%rsp),%rax

 
 
 
mulq  144(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq 56(%rsp),%rax

 
 
 
mulq  152(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq   32(%rsi),%rax

 
 
 
mulq  120(%rsi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq 64(%rsp),%rax

 
 
 
mulq  136(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq 64(%rsp),%rax

 
 
 
mulq  144(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq 64(%rsp),%rax

 
 
 
mulq  152(%rsi)

 
 
 
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

 
 
 
movq   104(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
movq %rax,96(%rsp)

 
 
 
mulq  56(%rsi)

 
 
 
mov  %rax,%rcx

 
 
 
mov  %rdx,%r8

 
 
 
movq   112(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
movq %rax,104(%rsp)

 
 
 
mulq  48(%rsi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq   80(%rsi),%rax

 
 
 
mulq  40(%rsi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq   80(%rsi),%rax

 
 
 
mulq  48(%rsi)

 
 
 
mov  %rax,%r9

 
 
 
mov  %rdx,%r10

 
 
 
movq   80(%rsi),%rax

 
 
 
mulq  56(%rsi)

 
 
 
mov  %rax,%r11

 
 
 
mov  %rdx,%r12

 
 
 
movq   80(%rsi),%rax

 
 
 
mulq  64(%rsi)

 
 
 
mov  %rax,%r13

 
 
 
mov  %rdx,%r14

 
 
 
movq   80(%rsi),%rax

 
 
 
mulq  72(%rsi)

 
 
 
mov  %rax,%r15

 
 
 
mov  %rdx,%rbx

 
 
 
movq   88(%rsi),%rax

 
 
 
mulq  40(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq   88(%rsi),%rax

 
 
 
mulq  48(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq   88(%rsi),%rax

 
 
 
mulq  56(%rsi)

 
 
 
add  %rax,%r13

 
 
 
adc %rdx,%r14

 
 
 
movq   88(%rsi),%rax

 
 
 
mulq  64(%rsi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq   88(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  72(%rsi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq   96(%rsi),%rax

 
 
 
mulq  40(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq   96(%rsi),%rax

 
 
 
mulq  48(%rsi)

 
 
 
add  %rax,%r13

 
 
 
adc %rdx,%r14

 
 
 
movq   96(%rsi),%rax

 
 
 
mulq  56(%rsi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq   96(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  64(%rsi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq   96(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  72(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq   104(%rsi),%rax

 
 
 
mulq  40(%rsi)

 
 
 
add  %rax,%r13

 
 
 
adc %rdx,%r14

 
 
 
movq   104(%rsi),%rax

 
 
 
mulq  48(%rsi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq 96(%rsp),%rax

 
 
 
mulq  64(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq 96(%rsp),%rax

 
 
 
mulq  72(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq   112(%rsi),%rax

 
 
 
mulq  40(%rsi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq 104(%rsp),%rax

 
 
 
mulq  56(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq 104(%rsp),%rax

 
 
 
mulq  64(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq 104(%rsp),%rax

 
 
 
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

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P0(%rip),%rdx

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%r8

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%r12

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%r13

 
 
 
add  crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%r14

 
 
 
movq 56(%rsp),%r15

 
 
 
sub  %r15,%rdx

 
 
 
add  %r15,%rcx

 
 
 
movq 64(%rsp),%r15

 
 
 
sub  %r15,%r8

 
 
 
add  %r15,%r9

 
 
 
movq 72(%rsp),%r15

 
 
 
sub  %r15,%r12

 
 
 
add  %r15,%rax

 
 
 
movq 80(%rsp),%r15

 
 
 
sub  %r15,%r13

 
 
 
add  %r15,%r10

 
 
 
movq 88(%rsp),%r15

 
 
 
sub  %r15,%r14

 
 
 
add  %r15,%r11

 
 
 
movq   %rdx,0(%rdi)

 
 
 
movq   %r8,8(%rdi)

 
 
 
movq   %r12,16(%rdi)

 
 
 
movq   %r13,24(%rdi)

 
 
 
movq   %r14,32(%rdi)

 
 
 
movq   %rcx,40(%rdi)

 
 
 
movq   %r9,48(%rdi)

 
 
 
movq   %rax,56(%rdi)

 
 
 
movq   %r10,64(%rdi)

 
 
 
movq   %r11,72(%rdi)

 
 
 
movq   64(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
movq %rax,56(%rsp)

 
 
 
mulq  136(%rsi)

 
 
 
mov  %rax,%rcx

 
 
 
mov  %rdx,%r8

 
 
 
movq   72(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
movq %rax,64(%rsp)

 
 
 
mulq  128(%rsi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq   40(%rsi),%rax

 
 
 
mulq  120(%rsi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq   40(%rsi),%rax

 
 
 
mulq  128(%rsi)

 
 
 
mov  %rax,%r9

 
 
 
mov  %rdx,%r10

 
 
 
movq   40(%rsi),%rax

 
 
 
mulq  136(%rsi)

 
 
 
mov  %rax,%r11

 
 
 
mov  %rdx,%r12

 
 
 
movq   40(%rsi),%rax

 
 
 
mulq  144(%rsi)

 
 
 
mov  %rax,%r13

 
 
 
mov  %rdx,%r14

 
 
 
movq   40(%rsi),%rax

 
 
 
mulq  152(%rsi)

 
 
 
mov  %rax,%r15

 
 
 
mov  %rdx,%rbx

 
 
 
movq   48(%rsi),%rax

 
 
 
mulq  120(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq   48(%rsi),%rax

 
 
 
mulq  128(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq   48(%rsi),%rax

 
 
 
mulq  136(%rsi)

 
 
 
add  %rax,%r13

 
 
 
adc %rdx,%r14

 
 
 
movq   48(%rsi),%rax

 
 
 
mulq  144(%rsi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq   48(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  152(%rsi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq   56(%rsi),%rax

 
 
 
mulq  120(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq   56(%rsi),%rax

 
 
 
mulq  128(%rsi)

 
 
 
add  %rax,%r13

 
 
 
adc %rdx,%r14

 
 
 
movq   56(%rsi),%rax

 
 
 
mulq  136(%rsi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq   56(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  144(%rsi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq   56(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  152(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq   64(%rsi),%rax

 
 
 
mulq  120(%rsi)

 
 
 
add  %rax,%r13

 
 
 
adc %rdx,%r14

 
 
 
movq   64(%rsi),%rax

 
 
 
mulq  128(%rsi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq 56(%rsp),%rax

 
 
 
mulq  144(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq 56(%rsp),%rax

 
 
 
mulq  152(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq   72(%rsi),%rax

 
 
 
mulq  120(%rsi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq 64(%rsp),%rax

 
 
 
mulq  136(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq 64(%rsp),%rax

 
 
 
mulq  144(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq 64(%rsp),%rax

 
 
 
mulq  152(%rsi)

 
 
 
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

 
 
 
movq   %rcx,80(%rdi)

 
 
 
movq   %r9,88(%rdi)

 
 
 
movq   %rax,96(%rdi)

 
 
 
movq   %r10,104(%rdi)

 
 
 
movq   %r11,112(%rdi)

 
 
 
movq   24(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
movq %rax,56(%rsp)

 
 
 
mulq  96(%rsi)

 
 
 
mov  %rax,%rcx

 
 
 
mov  %rdx,%r8

 
 
 
movq   32(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
movq %rax,64(%rsp)

 
 
 
mulq  88(%rsi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq   0(%rsi),%rax

 
 
 
mulq  80(%rsi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq   0(%rsi),%rax

 
 
 
mulq  88(%rsi)

 
 
 
mov  %rax,%r9

 
 
 
mov  %rdx,%r10

 
 
 
movq   0(%rsi),%rax

 
 
 
mulq  96(%rsi)

 
 
 
mov  %rax,%r11

 
 
 
mov  %rdx,%r12

 
 
 
movq   0(%rsi),%rax

 
 
 
mulq  104(%rsi)

 
 
 
mov  %rax,%r13

 
 
 
mov  %rdx,%r14

 
 
 
movq   0(%rsi),%rax

 
 
 
mulq  112(%rsi)

 
 
 
mov  %rax,%r15

 
 
 
mov  %rdx,%rbx

 
 
 
movq   8(%rsi),%rax

 
 
 
mulq  80(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq   8(%rsi),%rax

 
 
 
mulq  88(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq   8(%rsi),%rax

 
 
 
mulq  96(%rsi)

 
 
 
add  %rax,%r13

 
 
 
adc %rdx,%r14

 
 
 
movq   8(%rsi),%rax

 
 
 
mulq  104(%rsi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq   8(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  112(%rsi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq   16(%rsi),%rax

 
 
 
mulq  80(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq   16(%rsi),%rax

 
 
 
mulq  88(%rsi)

 
 
 
add  %rax,%r13

 
 
 
adc %rdx,%r14

 
 
 
movq   16(%rsi),%rax

 
 
 
mulq  96(%rsi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq   16(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  104(%rsi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq   16(%rsi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  112(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq   24(%rsi),%rax

 
 
 
mulq  80(%rsi)

 
 
 
add  %rax,%r13

 
 
 
adc %rdx,%r14

 
 
 
movq   24(%rsi),%rax

 
 
 
mulq  88(%rsi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq 56(%rsp),%rax

 
 
 
mulq  104(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq 56(%rsp),%rax

 
 
 
mulq  112(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq   32(%rsi),%rax

 
 
 
mulq  80(%rsi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq 64(%rsp),%rax

 
 
 
mulq  96(%rsi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq 64(%rsp),%rax

 
 
 
mulq  104(%rsi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq 64(%rsp),%rax

 
 
 
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

 
 
 
movq %rcx,56(%rsp)

 
 
 
movq %r8,64(%rsp)

 
 
 
movq %r9,72(%rsp)

 
 
 
movq %rax,80(%rsp)

 
 
 
movq %r10,88(%rsp)

 
 
 
movq 80(%rsp),%rsi

 
 
 
imulq  $19,%rsi,%rax

 
 
 
movq %rax,96(%rsp)

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D2(%rip)

 
 
 
mov  %rax,%rsi

 
 
 
mov  %rdx,%rcx

 
 
 
movq 88(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
movq %rax,104(%rsp)

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D1(%rip)

 
 
 
add  %rax,%rsi

 
 
 
adc %rdx,%rcx

 
 
 
movq 56(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D0(%rip)

 
 
 
add  %rax,%rsi

 
 
 
adc %rdx,%rcx

 
 
 
movq 56(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D1(%rip)

 
 
 
mov  %rax,%r8

 
 
 
mov  %rdx,%r9

 
 
 
movq 56(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D2(%rip)

 
 
 
mov  %rax,%r10

 
 
 
mov  %rdx,%r11

 
 
 
movq 56(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D3(%rip)

 
 
 
mov  %rax,%r12

 
 
 
mov  %rdx,%r13

 
 
 
movq 56(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D4(%rip)

 
 
 
mov  %rax,%r14

 
 
 
mov  %rdx,%r15

 
 
 
movq 64(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D0(%rip)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 64(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D1(%rip)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 64(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D2(%rip)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq 64(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D3(%rip)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq 64(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D4(%rip)

 
 
 
add  %rax,%rsi

 
 
 
adc %rdx,%rcx

 
 
 
movq 72(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D0(%rip)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 72(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D1(%rip)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq 72(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D2(%rip)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq 72(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D3(%rip)

 
 
 
add  %rax,%rsi

 
 
 
adc %rdx,%rcx

 
 
 
movq 72(%rsp),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D4(%rip)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 80(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D0(%rip)

 
 
 
add  %rax,%r12

 
 
 
adc %rdx,%r13

 
 
 
movq 80(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D1(%rip)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq 96(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D3(%rip)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 96(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D4(%rip)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 88(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D0(%rip)

 
 
 
add  %rax,%r14

 
 
 
adc %rdx,%r15

 
 
 
movq 104(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D2(%rip)

 
 
 
add  %rax,%r8

 
 
 
adc %rdx,%r9

 
 
 
movq 104(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D3(%rip)

 
 
 
add  %rax,%r10

 
 
 
adc %rdx,%r11

 
 
 
movq 104(%rsp),%rax

 
mulq  crypto_sign_ed25519_amd64_51_30k_batch_EC2D4(%rip)

 
 
 
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
