
 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 
.text
.p2align 5
.globl _crypto_sign_ed25519_amd64_51_30k_batch_fe25519_nsquare
.globl crypto_sign_ed25519_amd64_51_30k_batch_fe25519_nsquare
_crypto_sign_ed25519_amd64_51_30k_batch_fe25519_nsquare:
crypto_sign_ed25519_amd64_51_30k_batch_fe25519_nsquare:
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

 
._loop:

 
 
 
movq   0(%rdi),%rax

 
 
 
mulq  0(%rdi)

 
 
 
mov  %rax,%rcx

 
 
 
mov  %rdx,%r8

 
 
 
movq   0(%rdi),%rax

 
 
 
shl  $1,%rax

 
 
 
mulq  8(%rdi)

 
 
 
mov  %rax,%r9

 
 
 
mov  %rdx,%r10

 
 
 
movq   0(%rdi),%rax

 
 
 
shl  $1,%rax

 
 
 
mulq  16(%rdi)

 
 
 
mov  %rax,%r11

 
 
 
mov  %rdx,%r12

 
 
 
movq   0(%rdi),%rax

 
 
 
shl  $1,%rax

 
 
 
mulq  24(%rdi)

 
 
 
mov  %rax,%r13

 
 
 
mov  %rdx,%r14

 
 
 
movq   0(%rdi),%rax

 
 
 
shl  $1,%rax

 
 
 
mulq  32(%rdi)

 
 
 
mov  %rax,%r15

 
 
 
mov  %rdx,%rbx

 
 
 
movq   8(%rdi),%rax

 
 
 
mulq  8(%rdi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq   8(%rdi),%rax

 
 
 
shl  $1,%rax

 
 
 
mulq  16(%rdi)

 
 
 
add  %rax,%r13

 
 
 
adc %rdx,%r14

 
 
 
movq   8(%rdi),%rax

 
 
 
shl  $1,%rax

 
 
 
mulq  24(%rdi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq   8(%rdi),%rdx

 
 
 
imulq  $38,%rdx,%rax

 
 
 
mulq  32(%rdi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq   16(%rdi),%rax

 
 
 
mulq  16(%rdi)

 
 
 
add  %rax,%r15

 
 
 
adc %rdx,%rbx

 
 
 
movq   16(%rdi),%rdx

 
 
 
imulq  $38,%rdx,%rax

 
 
 
mulq  24(%rdi)

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
movq   16(%rdi),%rdx

 
 
 
imulq  $38,%rdx,%rax

 
 
 
mulq  32(%rdi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq   24(%rdi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  24(%rdi)

 
 
 
add  %rax,%r9

 
 
 
adc %rdx,%r10

 
 
 
movq   24(%rdi),%rdx

 
 
 
imulq  $38,%rdx,%rax

 
 
 
mulq  32(%rdi)

 
 
 
add  %rax,%r11

 
 
 
adc %rdx,%r12

 
 
 
movq   32(%rdi),%rdx

 
 
 
imulq  $19,%rdx,%rax

 
 
 
mulq  32(%rdi)

 
 
 
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

 
 
 
movq   %rcx,0(%rdi)

 
 
 
movq   %r9,8(%rdi)

 
 
 
movq   %rax,16(%rdi)

 
 
 
movq   %r10,24(%rdi)

 
 
 
movq   %r11,32(%rdi)

 
 
 
sub  $1,%rsi
 

 
jg ._loop

 
 
 
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
