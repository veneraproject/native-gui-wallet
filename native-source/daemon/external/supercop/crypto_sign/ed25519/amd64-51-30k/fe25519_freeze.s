
 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 
.text
.p2align 5
.globl _crypto_sign_ed25519_amd64_51_30k_batch_fe25519_freeze
.globl crypto_sign_ed25519_amd64_51_30k_batch_fe25519_freeze
_crypto_sign_ed25519_amd64_51_30k_batch_fe25519_freeze:
crypto_sign_ed25519_amd64_51_30k_batch_fe25519_freeze:
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

 
 
 
movq   0(%rdi),%rsi

 
 
 
movq   8(%rdi),%rdx

 
 
 
movq   16(%rdi),%rcx

 
 
 
movq   24(%rdi),%r8

 
 
 
movq   32(%rdi),%r9

 
 
 
movq crypto_sign_ed25519_amd64_51_30k_batch_REDMASK51(%rip),%rax

 
 
 
mov  %rax,%r10

 
 
 
sub  $18,%r10

 
 
 
mov  $3,%r11

 
._reduceloop:

 
 
 
mov  %rsi,%r12

 
 
 
shr  $51,%r12

 
 
 
and  %rax,%rsi

 
 
 
add  %r12,%rdx

 
 
 
mov  %rdx,%r12

 
 
 
shr  $51,%r12

 
 
 
and  %rax,%rdx

 
 
 
add  %r12,%rcx

 
 
 
mov  %rcx,%r12

 
 
 
shr  $51,%r12

 
 
 
and  %rax,%rcx

 
 
 
add  %r12,%r8

 
 
 
mov  %r8,%r12

 
 
 
shr  $51,%r12

 
 
 
and  %rax,%r8

 
 
 
add  %r12,%r9

 
 
 
mov  %r9,%r12

 
 
 
shr  $51,%r12

 
 
 
and  %rax,%r9

 
 
 
imulq  $19,%r12,%r12

 
 
 
add  %r12,%rsi

 
 
 
sub  $1,%r11
 

 
ja ._reduceloop

 
 
 
mov  $1,%r12

 
 
 
cmp  %r10,%rsi

 
 
 
cmovl %r11,%r12

 
 
 
cmp  %rax,%rdx

 
 
 
cmovne %r11,%r12

 
 
 
cmp  %rax,%rcx

 
 
 
cmovne %r11,%r12

 
 
 
cmp  %rax,%r8

 
 
 
cmovne %r11,%r12

 
 
 
cmp  %rax,%r9

 
 
 
cmovne %r11,%r12

 
 
 
neg  %r12

 
 
 
and  %r12,%rax

 
 
 
and  %r12,%r10

 
 
 
sub  %r10,%rsi

 
 
 
sub  %rax,%rdx

 
 
 
sub  %rax,%rcx

 
 
 
sub  %rax,%r8

 
 
 
sub  %rax,%r9

 
 
 
movq   %rsi,0(%rdi)

 
 
 
movq   %rdx,8(%rdi)

 
 
 
movq   %rcx,16(%rdi)

 
 
 
movq   %r8,24(%rdi)

 
 
 
movq   %r9,32(%rdi)

 
 
 
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
