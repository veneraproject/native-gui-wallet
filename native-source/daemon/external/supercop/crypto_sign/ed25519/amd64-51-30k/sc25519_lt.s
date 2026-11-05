
 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 
.text
.p2align 5
.globl _crypto_sign_ed25519_amd64_51_30k_batch_sc25519_lt
.globl crypto_sign_ed25519_amd64_51_30k_batch_sc25519_lt
_crypto_sign_ed25519_amd64_51_30k_batch_sc25519_lt:
crypto_sign_ed25519_amd64_51_30k_batch_sc25519_lt:
mov %rsp,%r11
and $31,%r11
add $0,%r11
sub %r11,%rsp

 
 
 
movq   0(%rdi),%rdx

 
 
 
movq   8(%rdi),%rcx

 
 
 
movq   16(%rdi),%r8

 
 
 
movq   24(%rdi),%rdi

 
 
 
subq 0(%rsi),%rdx

 
 
 
sbbq 8(%rsi),%rcx

 
 
 
sbbq 16(%rsi),%r8

 
 
 
sbbq 24(%rsi),%rdi

 
 
 
mov  $0,%rdi

 
 
 
mov  $1,%rsi

 
 
 
cmovc %rsi,%rdi

 
add %r11,%rsp
mov %rdi,%rax
mov %rsi,%rdx
ret
