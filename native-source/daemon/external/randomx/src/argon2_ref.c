  
                                                    

                    

                                                                  
                                                                           
                                                                 
                                                                
                                                                    
                                                                      
                                                                       
                                                   
                                                                       
                                                                        

                                                                               
                                                                             
                                                                      
                                                                            
                                                                          
                                                                          
                                                                          
                                                                             
                                                                             
                                                                    
  

                                                                                 
                                             
                 
                                                                             
  

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "argon2.h"
#include "argon2_core.h"

#include "blake2/blamka-round-ref.h"
#include "blake2/blake2-impl.h"
#include "blake2/blake2.h"

static void copy_block(block* dst, const block* src) {
	memcpy(dst->v, src->v, sizeof(uint64_t) * ARGON2_QWORDS_IN_BLOCK);
}

static void xor_block(block* dst, const block* src) {
	int i;
	for (i = 0; i < ARGON2_QWORDS_IN_BLOCK; ++i) {
		dst->v[i] ^= src->v[i];
	}
}

  
 
 
 
 
 
 
 
 
static void fill_block(const block *prev_block, const block *ref_block,
	block *next_block, int with_xor) {
	block blockR, block_tmp;
	unsigned i;

	copy_block(&blockR, ref_block);
	xor_block(&blockR, prev_block);
	copy_block(&block_tmp, &blockR);
	 
	if (with_xor) {
		 
		xor_block(&block_tmp, next_block);
		 
 
	}

	 
 
	for (i = 0; i < 8; ++i) {
		BLAKE2_ROUND_NOMSG(
			blockR.v[16 * i], blockR.v[16 * i + 1], blockR.v[16 * i + 2],
			blockR.v[16 * i + 3], blockR.v[16 * i + 4], blockR.v[16 * i + 5],
			blockR.v[16 * i + 6], blockR.v[16 * i + 7], blockR.v[16 * i + 8],
			blockR.v[16 * i + 9], blockR.v[16 * i + 10], blockR.v[16 * i + 11],
			blockR.v[16 * i + 12], blockR.v[16 * i + 13], blockR.v[16 * i + 14],
			blockR.v[16 * i + 15]);
	}

	 
 
	for (i = 0; i < 8; i++) {
		BLAKE2_ROUND_NOMSG(
			blockR.v[2 * i], blockR.v[2 * i + 1], blockR.v[2 * i + 16],
			blockR.v[2 * i + 17], blockR.v[2 * i + 32], blockR.v[2 * i + 33],
			blockR.v[2 * i + 48], blockR.v[2 * i + 49], blockR.v[2 * i + 64],
			blockR.v[2 * i + 65], blockR.v[2 * i + 80], blockR.v[2 * i + 81],
			blockR.v[2 * i + 96], blockR.v[2 * i + 97], blockR.v[2 * i + 112],
			blockR.v[2 * i + 113]);
	}

	copy_block(next_block, &block_tmp);
	xor_block(next_block, &blockR);
}

void randomx_argon2_fill_segment_ref(const argon2_instance_t *instance,
	argon2_position_t position) {
	block *ref_block = NULL, *curr_block = NULL;
	block address_block, input_block, zero_block;
	uint64_t pseudo_rand, ref_index, ref_lane;
	uint32_t prev_offset, curr_offset;
	uint32_t starting_index;
	uint32_t i;

	if (instance == NULL) {
		return;
	}

	starting_index = 0;

	if ((0 == position.pass) && (0 == position.slice)) {
		starting_index = 2;  
	}

	 
	curr_offset = position.lane * instance->lane_length +
		position.slice * instance->segment_length + starting_index;

	if (0 == curr_offset % instance->lane_length) {
		 
		prev_offset = curr_offset + instance->lane_length - 1;
	}
	else {
		 
		prev_offset = curr_offset - 1;
	}

	for (i = starting_index; i < instance->segment_length;
		++i, ++curr_offset, ++prev_offset) {
		 
		if (curr_offset % instance->lane_length == 1) {
			prev_offset = curr_offset - 1;
		}

		 
		 
		pseudo_rand = instance->memory[prev_offset].v[0];

		 
		ref_lane = ((pseudo_rand >> 32)) % instance->lanes;

		if ((position.pass == 0) && (position.slice == 0)) {
			 
			ref_lane = position.lane;
		}

		 
 
 
		position.index = i;
		ref_index = randomx_argon2_index_alpha(instance, &position, pseudo_rand & 0xFFFFFFFF,
			ref_lane == position.lane);

		 
		ref_block =
			instance->memory + instance->lane_length * ref_lane + ref_index;
		curr_block = instance->memory + curr_offset;
		if (ARGON2_VERSION_10 == instance->version) {
			 
			fill_block(instance->memory + prev_offset, ref_block, curr_block, 0);
		}
		else {
			if (0 == position.pass) {
				fill_block(instance->memory + prev_offset, ref_block,
					curr_block, 0);
			}
			else {
				fill_block(instance->memory + prev_offset, ref_block,
					curr_block, 1);
			}
		}
	}
}
