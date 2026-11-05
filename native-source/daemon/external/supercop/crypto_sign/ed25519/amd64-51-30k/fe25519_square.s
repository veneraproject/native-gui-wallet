
 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 
.text
.p2align 5
.globl _crypto_sign_ed25519_amd64_51_30k_batch_fe25519_square
.globl crypto_sign_ed25519_amd64_51_30k_batch_fe25519_square
_crypto_sign_ed25519_amd64_51_30k_batch_fe25519_square:
crypto_sign_ed25519_amd64_51_30k_batch_fe25519_square:
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

 
 
 
and  %rsi,%rcx

 
 
 
mov  %rdx,%r8

 
 
 
shr  $51,%rdx

 
 
 
add  %r11,%rdx

 
 
 
and  %rsi,%r8

 
 
 
mov  %rdx,%r9

 
 
 
shr  $51,%rdx

 
 
 
add  %r13,%rdx

 
 
 
and  %rsi,%r9

 
 
 
mov  %rdx,%rax

 
 
 
shr  $51,%rdx

 
 
 
add  %r15,%rdx

 
 
 
and  %rsi,%rax

 
 
 
mov  %rdx,%r10

 
 
 
shr  $51,%rdx

 
 
 
imulq  $19,%rdx,%rdx

 
 
 
add  %rdx,%rcx

 
 
 
and  %rsi,%r10

 
 
 
movq   %rcx,0(%rdi)

 
 
 
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
