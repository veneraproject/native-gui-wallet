
 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 
.text
.p2align 5
.globl _crypto_sign_ed25519_amd64_64_fe25519_sub
.globl crypto_sign_ed25519_amd64_64_fe25519_sub
_crypto_sign_ed25519_amd64_64_fe25519_sub:
crypto_sign_ed25519_amd64_64_fe25519_sub:
mov %rsp,%r11
and $31,%r11
add $0,%r11
sub %r11,%rsp

 
 
 
movq   0(%rsi),%rcx

 
 
 
movq   8(%rsi),%r8

 
 
 
movq   16(%rsi),%r9

 
 
 
movq   24(%rsi),%rsi

 
 
 
subq 0(%rdx),%rcx

 
 
 
sbbq 8(%rdx),%r8

 
 
 
sbbq 16(%rdx),%r9

 
 
 
sbbq 24(%rdx),%rsi

 
 
 
mov  $0,%rdx

 
 
 
mov  $38,%rax

 
 
 
cmovae %rdx,%rax

 
 
 
sub  %rax,%rcx

 
 
 
sbb  %rdx,%r8

 
 
 
sbb  %rdx,%r9

 
 
 
sbb  %rdx,%rsi

 
 
 
cmovc %rax,%rdx

 
 
 
sub  %rdx,%rcx

 
 
 
movq   %rcx,0(%rdi)

 
 
 
movq   %r8,8(%rdi)

 
 
 
movq   %r9,16(%rdi)

 
 
 
movq   %rsi,24(%rdi)

 
add %r11,%rsp
mov %rdi,%rax
mov %rsi,%rdx
ret
