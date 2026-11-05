
 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 
.text
.p2align 5
.globl _crypto_sign_ed25519_amd64_51_30k_batch_choose_t
.globl crypto_sign_ed25519_amd64_51_30k_batch_choose_t
_crypto_sign_ed25519_amd64_51_30k_batch_choose_t:
crypto_sign_ed25519_amd64_51_30k_batch_choose_t:
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

 
 
 
movq %rdi,56(%rsp)

 
 
 
imulq  $960,%rsi,%rdi

 
 
 
mov  %rdx,%rsi

 
 
 
sar  $7,%rsi

 
 
 
mov  %rdx,%r8

 
 
 
add  %rsi,%r8

 
 
 
xor  %rsi,%r8

 
 
 
mov  $1,%rsi

 
 
 
mov  $0,%r9

 
 
 
mov  $0,%rax

 
 
 
mov  $0,%r10

 
 
 
mov  $0,%r11

 
 
 
mov  $1,%r12

 
 
 
mov  $0,%r13

 
 
 
mov  $0,%r14

 
 
 
mov  $0,%r15

 
 
 
mov  $0,%rbx

 
 
 
cmp  $1,%r8

 
 
 
movq   0(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%rsi

 
 
 
movq   8(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r9

 
 
 
movq   16(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%rax

 
 
 
movq   24(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r10

 
 
 
movq   32(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r11

 
 
 
movq   40(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r12

 
 
 
movq   48(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r13

 
 
 
movq   56(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r14

 
 
 
movq   64(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r15

 
 
 
movq   72(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%rbx

 
 
 
cmp  $2,%r8

 
 
 
movq   120(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%rsi

 
 
 
movq   128(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r9

 
 
 
movq   136(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%rax

 
 
 
movq   144(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r10

 
 
 
movq   152(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r11

 
 
 
movq   160(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r12

 
 
 
movq   168(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r13

 
 
 
movq   176(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r14

 
 
 
movq   184(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r15

 
 
 
movq   192(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%rbx

 
 
 
cmp  $3,%r8

 
 
 
movq   240(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%rsi

 
 
 
movq   248(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r9

 
 
 
movq   256(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%rax

 
 
 
movq   264(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r10

 
 
 
movq   272(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r11

 
 
 
movq   280(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r12

 
 
 
movq   288(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r13

 
 
 
movq   296(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r14

 
 
 
movq   304(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r15

 
 
 
movq   312(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%rbx

 
 
 
cmp  $4,%r8

 
 
 
movq   360(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%rsi

 
 
 
movq   368(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r9

 
 
 
movq   376(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%rax

 
 
 
movq   384(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r10

 
 
 
movq   392(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r11

 
 
 
movq   400(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r12

 
 
 
movq   408(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r13

 
 
 
movq   416(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r14

 
 
 
movq   424(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r15

 
 
 
movq   432(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%rbx

 
 
 
cmp  $5,%r8

 
 
 
movq   480(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%rsi

 
 
 
movq   488(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r9

 
 
 
movq   496(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%rax

 
 
 
movq   504(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r10

 
 
 
movq   512(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r11

 
 
 
movq   520(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r12

 
 
 
movq   528(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r13

 
 
 
movq   536(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r14

 
 
 
movq   544(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r15

 
 
 
movq   552(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%rbx

 
 
 
cmp  $6,%r8

 
 
 
movq   600(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%rsi

 
 
 
movq   608(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r9

 
 
 
movq   616(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%rax

 
 
 
movq   624(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r10

 
 
 
movq   632(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r11

 
 
 
movq   640(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r12

 
 
 
movq   648(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r13

 
 
 
movq   656(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r14

 
 
 
movq   664(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r15

 
 
 
movq   672(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%rbx

 
 
 
cmp  $7,%r8

 
 
 
movq   720(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%rsi

 
 
 
movq   728(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r9

 
 
 
movq   736(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%rax

 
 
 
movq   744(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r10

 
 
 
movq   752(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r11

 
 
 
movq   760(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r12

 
 
 
movq   768(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r13

 
 
 
movq   776(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r14

 
 
 
movq   784(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r15

 
 
 
movq   792(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%rbx

 
 
 
cmp  $8,%r8

 
 
 
movq   840(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%rsi

 
 
 
movq   848(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r9

 
 
 
movq   856(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%rax

 
 
 
movq   864(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r10

 
 
 
movq   872(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r11

 
 
 
movq   880(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r12

 
 
 
movq   888(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r13

 
 
 
movq   896(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r14

 
 
 
movq   904(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%r15

 
 
 
movq   912(%rcx,%rdi),%rbp

 
 
 
cmove %rbp,%rbx

 
 
 
cmp  $0,%rdx

 
 
 
mov  %rsi,%rbp

 
 
 
cmovl %r12,%rsi

 
 
 
cmovl %rbp,%r12

 
 
 
mov  %r9,%rbp

 
 
 
cmovl %r13,%r9

 
 
 
cmovl %rbp,%r13

 
 
 
mov  %rax,%rbp

 
 
 
cmovl %r14,%rax

 
 
 
cmovl %rbp,%r14

 
 
 
mov  %r10,%rbp

 
 
 
cmovl %r15,%r10

 
 
 
cmovl %rbp,%r15

 
 
 
mov  %r11,%rbp

 
 
 
cmovl %rbx,%r11

 
 
 
cmovl %rbp,%rbx

 
 
 
movq 56(%rsp),%rbp

 
 
 
movq   %rsi,0(%rbp)

 
 
 
movq   %r9,8(%rbp)

 
 
 
movq   %rax,16(%rbp)

 
 
 
movq   %r10,24(%rbp)

 
 
 
movq   %r11,32(%rbp)

 
 
 
movq   %r12,40(%rbp)

 
 
 
movq   %r13,48(%rbp)

 
 
 
movq   %r14,56(%rbp)

 
 
 
movq   %r15,64(%rbp)

 
 
 
movq   %rbx,72(%rbp)

 
 
 
mov  $0,%rsi

 
 
 
mov  $0,%r9

 
 
 
mov  $0,%rax

 
 
 
mov  $0,%r10

 
 
 
mov  $0,%r11

 
 
 
cmp  $1,%r8

 
 
 
movq   80(%rcx,%rdi),%r12

 
 
 
cmove %r12,%rsi

 
 
 
movq   88(%rcx,%rdi),%r12

 
 
 
cmove %r12,%r9

 
 
 
movq   96(%rcx,%rdi),%r12

 
 
 
cmove %r12,%rax

 
 
 
movq   104(%rcx,%rdi),%r12

 
 
 
cmove %r12,%r10

 
 
 
movq   112(%rcx,%rdi),%r12

 
 
 
cmove %r12,%r11

 
 
 
cmp  $2,%r8

 
 
 
movq   200(%rcx,%rdi),%r12

 
 
 
cmove %r12,%rsi

 
 
 
movq   208(%rcx,%rdi),%r12

 
 
 
cmove %r12,%r9

 
 
 
movq   216(%rcx,%rdi),%r12

 
 
 
cmove %r12,%rax

 
 
 
movq   224(%rcx,%rdi),%r12

 
 
 
cmove %r12,%r10

 
 
 
movq   232(%rcx,%rdi),%r12

 
 
 
cmove %r12,%r11

 
 
 
cmp  $3,%r8

 
 
 
movq   320(%rcx,%rdi),%r12

 
 
 
cmove %r12,%rsi

 
 
 
movq   328(%rcx,%rdi),%r12

 
 
 
cmove %r12,%r9

 
 
 
movq   336(%rcx,%rdi),%r12

 
 
 
cmove %r12,%rax

 
 
 
movq   344(%rcx,%rdi),%r12

 
 
 
cmove %r12,%r10

 
 
 
movq   352(%rcx,%rdi),%r12

 
 
 
cmove %r12,%r11

 
 
 
cmp  $4,%r8

 
 
 
movq   440(%rcx,%rdi),%r12

 
 
 
cmove %r12,%rsi

 
 
 
movq   448(%rcx,%rdi),%r12

 
 
 
cmove %r12,%r9

 
 
 
movq   456(%rcx,%rdi),%r12

 
 
 
cmove %r12,%rax

 
 
 
movq   464(%rcx,%rdi),%r12

 
 
 
cmove %r12,%r10

 
 
 
movq   472(%rcx,%rdi),%r12

 
 
 
cmove %r12,%r11

 
 
 
cmp  $5,%r8

 
 
 
movq   560(%rcx,%rdi),%r12

 
 
 
cmove %r12,%rsi

 
 
 
movq   568(%rcx,%rdi),%r12

 
 
 
cmove %r12,%r9

 
 
 
movq   576(%rcx,%rdi),%r12

 
 
 
cmove %r12,%rax

 
 
 
movq   584(%rcx,%rdi),%r12

 
 
 
cmove %r12,%r10

 
 
 
movq   592(%rcx,%rdi),%r12

 
 
 
cmove %r12,%r11

 
 
 
cmp  $6,%r8

 
 
 
movq   680(%rcx,%rdi),%r12

 
 
 
cmove %r12,%rsi

 
 
 
movq   688(%rcx,%rdi),%r12

 
 
 
cmove %r12,%r9

 
 
 
movq   696(%rcx,%rdi),%r12

 
 
 
cmove %r12,%rax

 
 
 
movq   704(%rcx,%rdi),%r12

 
 
 
cmove %r12,%r10

 
 
 
movq   712(%rcx,%rdi),%r12

 
 
 
cmove %r12,%r11

 
 
 
cmp  $7,%r8

 
 
 
movq   800(%rcx,%rdi),%r12

 
 
 
cmove %r12,%rsi

 
 
 
movq   808(%rcx,%rdi),%r12

 
 
 
cmove %r12,%r9

 
 
 
movq   816(%rcx,%rdi),%r12

 
 
 
cmove %r12,%rax

 
 
 
movq   824(%rcx,%rdi),%r12

 
 
 
cmove %r12,%r10

 
 
 
movq   832(%rcx,%rdi),%r12

 
 
 
cmove %r12,%r11

 
 
 
cmp  $8,%r8

 
 
 
movq   920(%rcx,%rdi),%r8

 
 
 
cmove %r8,%rsi

 
 
 
movq   928(%rcx,%rdi),%r8

 
 
 
cmove %r8,%r9

 
 
 
movq   936(%rcx,%rdi),%r8

 
 
 
cmove %r8,%rax

 
 
 
movq   944(%rcx,%rdi),%r8

 
 
 
cmove %r8,%r10

 
 
 
movq   952(%rcx,%rdi),%rdi

 
 
 
cmove %rdi,%r11

 
 
 
movq crypto_sign_ed25519_amd64_51_30k_batch_2P0(%rip),%rdi

 
 
 
movq crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%rcx

 
 
 
movq crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%r8

 
 
 
movq crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%r12

 
 
 
movq crypto_sign_ed25519_amd64_51_30k_batch_2P1234(%rip),%r13

 
 
 
sub  %rsi,%rdi

 
 
 
sub  %r9,%rcx

 
 
 
sub  %rax,%r8

 
 
 
sub  %r10,%r12

 
 
 
sub  %r11,%r13

 
 
 
cmp  $0,%rdx

 
 
 
cmovl %rdi,%rsi

 
 
 
cmovl %rcx,%r9

 
 
 
cmovl %r8,%rax

 
 
 
cmovl %r12,%r10

 
 
 
cmovl %r13,%r11

 
 
 
movq   %rsi,80(%rbp)

 
 
 
movq   %r9,88(%rbp)

 
 
 
movq   %rax,96(%rbp)

 
 
 
movq   %r10,104(%rbp)

 
 
 
movq   %r11,112(%rbp)

 
 
 
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
