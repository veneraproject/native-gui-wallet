                                                      
                                                                 
 
                      
 
                                                                    
                                                                             
                                                                   
                                                                  
                                                                      
                                                                        
                                                                         
                                                     
                                                                         
                                                                          
 
                                                                                 
                                                                               
                                                                        
                                                                              
                                                                            
                                                                            
                                                                            
                                                                               
                                                                               
                                                                      

#if defined(__APPLE__)
#define DECL(x) _##x
#else
#define DECL(x) x
#endif

	.arch armv8-a
	.text
	.global DECL(randomx_program_aarch64)
	.global DECL(randomx_program_aarch64_main_loop)
	.global DECL(randomx_program_aarch64_vm_instructions)
	.global DECL(randomx_program_aarch64_imul_rcp_literals_end)
	.global DECL(randomx_program_aarch64_vm_instructions_end)
	.global DECL(randomx_program_aarch64_cacheline_align_mask1)
	.global DECL(randomx_program_aarch64_cacheline_align_mask2)
	.global DECL(randomx_program_aarch64_update_spMix1)
	.global DECL(randomx_program_aarch64_vm_instructions_end_light)
	.global DECL(randomx_program_aarch64_light_cacheline_align_mask)
	.global DECL(randomx_program_aarch64_light_dataset_offset)
	.global DECL(randomx_init_dataset_aarch64)
	.global DECL(randomx_init_dataset_aarch64_end)
	.global DECL(randomx_calc_dataset_item_aarch64)
	.global DECL(randomx_calc_dataset_item_aarch64_prefetch)
	.global DECL(randomx_calc_dataset_item_aarch64_mix)
	.global DECL(randomx_calc_dataset_item_aarch64_store_result)
	.global DECL(randomx_calc_dataset_item_aarch64_end)

#include "configuration.h"

 

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 

	.balign 4
DECL(randomx_program_aarch64):
	 
	sub	sp, sp, 192
	stp	x16, x17, [sp]
	str	x19, [sp, 16]
	stp	x20, x21, [sp, 32]
	stp	x22, x23, [sp, 48]
	stp	x24, x25, [sp, 64]
	stp	x26, x27, [sp, 80]
	stp	x28, x29, [sp, 96]
	stp	x8, x30, [sp, 112]
	stp	d8, d9, [sp, 128]
	stp	d10, d11, [sp, 144]
	stp	d12, d13, [sp, 160]
	stp	d14, d15, [sp, 176]

	 
	mov	x4, xzr
	mov	x5, xzr
	mov	x6, xzr
	mov	x7, xzr
	mov	x12, xzr
	mov	x13, xzr
	mov	x14, xzr
	mov	x15, xzr

	 
	ldp	x9, x1, [x1]

	 
	mov	x10, x9

	 
	ldp	q24, q25, [x0, 192]
	ldp	q26, q27, [x0, 224]

	 
	mov	x16, 0x00FFFFFFFFFFFFFF
	ins	v29.d[0], x16
	ins	v29.d[1], x16

	 
	ldr	q30, [x0, 64]

	 
	mov	x16, 0x80f0000000000000
	ins	v31.d[0], x16
	ins	v31.d[1], x16

	 
	mrs	x8, fpcr
	rbit	x8, x8

	 
	str	x0, [sp, -16]!

	 
	ldr	x0, literal_x0
	ldr	x11, literal_x11
	ldr	x21, literal_x21
	ldr	x22, literal_x22
	ldr	x23, literal_x23
	ldr	x24, literal_x24
	ldr	x25, literal_x25
	ldr	x26, literal_x26
	ldr	x27, literal_x27
	ldr	x28, literal_x28
	ldr	x29, literal_x29
	ldr	x30, literal_x30

	ldr	q0, literal_v0
	ldr	q1, literal_v1
	ldr	q2, literal_v2
	ldr	q3, literal_v3
	ldr	q4, literal_v4
	ldr	q5, literal_v5
	ldr	q6, literal_v6
	ldr	q7, literal_v7
	ldr	q8, literal_v8
	ldr	q9, literal_v9
	ldr	q10, literal_v10
	ldr	q11, literal_v11
	ldr	q12, literal_v12
	ldr	q13, literal_v13
	ldr	q14, literal_v14
	ldr	q15, literal_v15

DECL(randomx_program_aarch64_main_loop):
	 
	 
	lsr	x20, x10, 32

	 
	and	w16, w10, 1
	and	w17, w20, 1

	 
	 
	add	x16, x16, x2
	add	x17, x17, x2

	 
	ldp	x20, x19, [x16]
	eor	x4, x4, x20
	eor	x5, x5, x19
	ldp	x20, x19, [x16, 16]
	eor	x6, x6, x20
	eor	x7, x7, x19
	ldp	x20, x19, [x16, 32]
	eor	x12, x12, x20
	eor	x13, x13, x19
	ldp	x20, x19, [x16, 48]
	eor	x14, x14, x20
	eor	x15, x15, x19

	 
	ldpsw	x20, x19, [x17]
	ins	v16.d[0], x20
	ins	v16.d[1], x19
	ldpsw	x20, x19, [x17, 8]
	ins	v17.d[0], x20
	ins	v17.d[1], x19
	ldpsw	x20, x19, [x17, 16]
	ins	v18.d[0], x20
	ins	v18.d[1], x19
	ldpsw	x20, x19, [x17, 24]
	ins	v19.d[0], x20
	ins	v19.d[1], x19
	scvtf	v16.2d, v16.2d
	scvtf	v17.2d, v17.2d
	scvtf	v18.2d, v18.2d
	scvtf	v19.2d, v19.2d

	 
	ldpsw	x20, x19, [x17, 32]
	ins	v20.d[0], x20
	ins	v20.d[1], x19
	ldpsw	x20, x19, [x17, 40]
	ins	v21.d[0], x20
	ins	v21.d[1], x19
	ldpsw	x20, x19, [x17, 48]
	ins	v22.d[0], x20
	ins	v22.d[1], x19
	ldpsw	x20, x19, [x17, 56]
	ins	v23.d[0], x20
	ins	v23.d[1], x19
	scvtf	v20.2d, v20.2d
	scvtf	v21.2d, v21.2d
	scvtf	v22.2d, v22.2d
	scvtf	v23.2d, v23.2d
	and	v20.16b, v20.16b, v29.16b
	and	v21.16b, v21.16b, v29.16b
	and	v22.16b, v22.16b, v29.16b
	and	v23.16b, v23.16b, v29.16b
	orr	v20.16b, v20.16b, v30.16b
	orr	v21.16b, v21.16b, v30.16b
	orr	v22.16b, v22.16b, v30.16b
	orr	v23.16b, v23.16b, v30.16b

	 
DECL(randomx_program_aarch64_vm_instructions):

	 
	 
	.fill RANDOMX_PROGRAM_SIZE*12,4,0

literal_x0:  .fill 1,8,0
literal_x11: .fill 1,8,0
literal_x21: .fill 1,8,0
literal_x22: .fill 1,8,0
literal_x23: .fill 1,8,0
literal_x24: .fill 1,8,0
literal_x25: .fill 1,8,0
literal_x26: .fill 1,8,0
literal_x27: .fill 1,8,0
literal_x28: .fill 1,8,0
literal_x29: .fill 1,8,0
literal_x30: .fill 1,8,0
DECL(randomx_program_aarch64_imul_rcp_literals_end):

literal_v0:  .fill 2,8,0
literal_v1:  .fill 2,8,0
literal_v2:  .fill 2,8,0
literal_v3:  .fill 2,8,0
literal_v4:  .fill 2,8,0
literal_v5:  .fill 2,8,0
literal_v6:  .fill 2,8,0
literal_v7:  .fill 2,8,0
literal_v8:  .fill 2,8,0
literal_v9:  .fill 2,8,0
literal_v10: .fill 2,8,0
literal_v11: .fill 2,8,0
literal_v12: .fill 2,8,0
literal_v13: .fill 2,8,0
literal_v14: .fill 2,8,0
literal_v15: .fill 2,8,0

DECL(randomx_program_aarch64_vm_instructions_end):
	 
	 
	lsr	x10, x9, 32

	 
	eor	x9, x9, x20

	 
	mov	w20, w9
DECL(randomx_program_aarch64_cacheline_align_mask1):
	 
	and	x20, x20, 1
	add	x20, x20, x1

	 
	prfm	pldl2strm, [x20]

	 
	ror	x9, x9, 32

DECL(randomx_program_aarch64_cacheline_align_mask2):
	 
	and	x10, x10, 1
	add	x10, x10, x1

DECL(randomx_program_aarch64_xor_with_dataset_line):
rx_program_xor_with_dataset_line:
	 
	ldp	x20, x19, [x10]
	eor	x4, x4, x20
	eor	x5, x5, x19
	ldp	x20, x19, [x10, 16]
	eor	x6, x6, x20
	eor	x7, x7, x19
	ldp	x20, x19, [x10, 32]
	eor	x12, x12, x20
	eor	x13, x13, x19
	ldp	x20, x19, [x10, 48]
	eor	x14, x14, x20
	eor	x15, x15, x19

DECL(randomx_program_aarch64_update_spMix1):
	 
	eor	x10, x0, x0

	 
	stp	x4, x5, [x17, 0]
	stp	x6, x7, [x17, 16]
	stp	x12, x13, [x17, 32]
	stp	x14, x15, [x17, 48]

	 
	eor	v16.16b, v16.16b, v20.16b
	eor	v17.16b, v17.16b, v21.16b
	eor	v18.16b, v18.16b, v22.16b
	eor	v19.16b, v19.16b, v23.16b

	 
	stp	q16, q17, [x16, 0]
	stp	q18, q19, [x16, 32]

	subs	x3, x3, 1
	bne	DECL(randomx_program_aarch64_main_loop)
	
	 
	ldr	x0, [sp], 16

	 
	stp	x4, x5, [x0, 0]
	stp	x6, x7, [x0, 16]
	stp	x12, x13, [x0, 32]
	stp	x14, x15, [x0, 48]

	 
	stp	q16, q17, [x0, 64]
	stp	q18, q19, [x0, 96]
	stp	q20, q21, [x0, 128]
	stp	q22, q23, [x0, 160]

	 
	ldp	x16, x17, [sp]
	ldr	x19, [sp, 16]
	ldp	x20, x21, [sp, 32]
	ldp	x22, x23, [sp, 48]
	ldp	x24, x25, [sp, 64]
	ldp	x26, x27, [sp, 80]
	ldp	x28, x29, [sp, 96]
	ldp	x8, x30, [sp, 112]
	ldp	d8, d9, [sp, 128]
	ldp	d10, d11, [sp, 144]
	ldp	d12, d13, [sp, 160]
	ldp	d14, d15, [sp, 176]
	add	sp, sp, 192

	ret

DECL(randomx_program_aarch64_vm_instructions_end_light):
	sub	sp, sp, 96
	stp	x0, x1, [sp, 64]
	stp	x2, x30, [sp, 80]

	 
	eor	x9, x9, x20

	 
	ror	x9, x9, 32

	 
	mov	x0, x1

	 
	mov	x1, sp

DECL(randomx_program_aarch64_light_cacheline_align_mask):
	 
	and	w2, w9, 1

	 
	lsr	x2, x2, 6

DECL(randomx_program_aarch64_light_dataset_offset):
	 
	add	x2, x2, 0
	add	x2, x2, 0

	bl	rx_calc_dataset_item

	mov	x10, sp
	ldp	x0, x1, [sp, 64]
	ldp	x2, x30, [sp, 80]
	add	sp, sp, 96

	b	rx_program_xor_with_dataset_line



 
 
 
 
 
 

DECL(randomx_init_dataset_aarch64):
	 
	stp	x20, x30, [sp, -16]!

	 
	ldr	x0, [x0]

DECL(randomx_init_dataset_aarch64_main_loop):
	bl	rx_calc_dataset_item
	add	x1, x1, 64
	add	x2, x2, 1
	cmp	x2, x3
	bne	DECL(randomx_init_dataset_aarch64_main_loop)

	 
	ldp	x20, x30, [sp], 16

	ret

DECL(randomx_init_dataset_aarch64_end):

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 

DECL(randomx_calc_dataset_item_aarch64):
rx_calc_dataset_item:
	sub	sp, sp, 112
	stp	x0, x1, [sp]
	stp	x2, x3, [sp, 16]
	stp	x4, x5, [sp, 32]
	stp	x6, x7, [sp, 48]
	stp	x8, x9, [sp, 64]
	stp	x10, x11, [sp, 80]
	stp	x12, x13, [sp, 96]

	ldr	x12, superscalarMul0

	mov	x8, x0
	mov	x9, x1
	mov	x10, x2

	 
	madd	x0, x2, x12, x12

	 
	ldr	x12, superscalarAdd1
	eor	x1, x0, x12

	 
	ldr	x12, superscalarAdd2
	eor	x2, x0, x12

	 
	ldr	x12, superscalarAdd3
	eor	x3, x0, x12

	 
	ldr	x12, superscalarAdd4
	eor	x4, x0, x12

	 
	ldr	x12, superscalarAdd5
	eor	x5, x0, x12

	 
	ldr	x12, superscalarAdd6
	eor	x6, x0, x12

	 
	ldr	x12, superscalarAdd7
	eor	x7, x0, x12

	b	rx_calc_dataset_item_prefetch

superscalarMul0: .quad 6364136223846793005
superscalarAdd1: .quad 9298411001130361340
superscalarAdd2: .quad 12065312585734608966
superscalarAdd3: .quad 9306329213124626780
superscalarAdd4: .quad 5281919268842080866
superscalarAdd5: .quad 10536153434571861004
superscalarAdd6: .quad 3398623926847679864
superscalarAdd7: .quad 9549104520008361294

 

DECL(randomx_calc_dataset_item_aarch64_prefetch):
rx_calc_dataset_item_prefetch:
	 
	and	x11, x10, 1
	add	x11, x8, x11, lsl 6
	prfm	pldl2strm, [x11]

	 

DECL(randomx_calc_dataset_item_aarch64_mix):
	ldp	x12, x13, [x11]
	eor	x0, x0, x12
	eor	x1, x1, x13
	ldp	x12, x13, [x11, 16]
	eor	x2, x2, x12
	eor	x3, x3, x13
	ldp	x12, x13, [x11, 32]
	eor	x4, x4, x12
	eor	x5, x5, x13
	ldp	x12, x13, [x11, 48]
	eor	x6, x6, x12
	eor	x7, x7, x13

DECL(randomx_calc_dataset_item_aarch64_store_result):
	stp	x0, x1, [x9]
	stp	x2, x3, [x9, 16]
	stp	x4, x5, [x9, 32]
	stp	x6, x7, [x9, 48]

	ldp	x0, x1, [sp]
	ldp	x2, x3, [sp, 16]
	ldp	x4, x5, [sp, 32]
	ldp	x6, x7, [sp, 48]
	ldp	x8, x9, [sp, 64]
	ldp	x10, x11, [sp, 80]
	ldp	x12, x13, [sp, 96]
	add	sp, sp, 112

	ret

DECL(randomx_calc_dataset_item_aarch64_end):
