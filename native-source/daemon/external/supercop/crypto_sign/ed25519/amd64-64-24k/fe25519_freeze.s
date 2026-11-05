
 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 
.text
.p2align 5
.globl _crypto_sign_ed25519_amd64_64_fe25519_freeze
.globl crypto_sign_ed25519_amd64_64_fe25519_freeze
_crypto_sign_ed25519_amd64_64_fe25519_freeze:
crypto_sign_ed25519_amd64_64_fe25519_freeze:
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

 
 
 
mov  %rsi,%r9

 
 
 
mov  %rdx,%rax

 
 
 
mov  %rcx,%r10

 
 
 
mov  %r8,%r11

 
 
 
mov  $1,%r12

 
 
 
shl  $63,%r12

 
 
 
add  $19,%r9

 
 
 
adc $0,%rax

 
 
 
adc $0,%r10

 
 
 
adc %r12,%r11

 
 
 
cmovc %r9,%rsi

 
 
 
cmovc %rax,%rdx

 
 
 
cmovc %r10,%rcx

 
 
 
cmovc %r11,%r8

 
 
 
mov  %rsi,%r9

 
 
 
mov  %rdx,%rax

 
 
 
mov  %rcx,%r10

 
 
 
mov  %r8,%r11

 
 
 
add  $19,%r9

 
 
 
adc $0,%rax

 
 
 
adc $0,%r10

 
 
 
adc %r12,%r11

 
 
 
cmovc %r9,%rsi

 
 
 
cmovc %rax,%rdx

 
 
 
cmovc %r10,%rcx

 
 
 
cmovc %r11,%r8

 
 
 
movq   %rsi,0(%rdi)

 
 
 
movq   %rdx,8(%rdi)

 
 
 
movq   %rcx,16(%rdi)

 
 
 
movq   %r8,24(%rdi)

 
 
 
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
