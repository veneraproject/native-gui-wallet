
 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 
.text
.p2align 5
.globl _crypto_sign_ed25519_amd64_64_fe25519_add
.globl crypto_sign_ed25519_amd64_64_fe25519_add
_crypto_sign_ed25519_amd64_64_fe25519_add:
crypto_sign_ed25519_amd64_64_fe25519_add:
mov %rsp,%r11
and $31,%r11
add $0,%r11
sub %r11,%rsp

 
 
 
movq   0(%rsi),%rcx

 
 
 
movq   8(%rsi),%r8

 
 
 
movq   16(%rsi),%r9

 
 
 
movq   24(%rsi),%rsi

 
 
 
addq 0(%rdx),%rcx

 
 
 
adcq 8(%rdx),%r8

 
 
 
adcq 16(%rdx),%r9

 
 
 
adcq 24(%rdx),%rsi

 
 
 
mov  $0,%rdx

 
 
 
mov  $38,%rax

 
 
 
cmovae %rdx,%rax

 
 
 
add  %rax,%rcx

 
 
 
adc %rdx,%r8

 
 
 
adc %rdx,%r9

 
 
 
adc %rdx,%rsi

 
 
 
cmovc %rax,%rdx

 
 
 
add  %rdx,%rcx

 
 
 
movq   %rcx,0(%rdi)

 
 
 
movq   %r8,8(%rdi)

 
 
 
movq   %r9,16(%rdi)

 
 
 
movq   %rsi,24(%rdi)

 
add %r11,%rsp
mov %rdi,%rax
mov %rsi,%rdx
ret
