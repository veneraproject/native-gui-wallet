  
                                                    

                    

                                                                  
                                                                           
                                                                 
                                                                
                                                                    
                                                                      
                                                                       
                                                   
                                                                       
                                                                        

                                                                               
                                                                             
                                                                      
                                                                            
                                                                          
                                                                          
                                                                          
                                                                             
                                                                             
                                                                    
  

                                                                                 
                                             
                 
                                                                             
  

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "argon2.h"

void randomx_argon2_fill_segment_avx2(const argon2_instance_t* instance,
	argon2_position_t position);

randomx_argon2_impl* randomx_argon2_impl_avx2() {
#if defined(__AVX2__)
	return &randomx_argon2_fill_segment_avx2;
#endif
	return NULL;
}

#if defined(__AVX2__)

#include "argon2_core.h"

#include "blake2/blamka-round-avx2.h"
#include "blake2/blake2-impl.h"
#include "blake2/blake2.h"

static void fill_block(__m256i* state, const block* ref_block,
	block* next_block, int with_xor) {
	__m256i block_XY[ARGON2_HWORDS_IN_BLOCK];
	unsigned int i;

	if (with_xor) {
		for (i = 0; i < ARGON2_HWORDS_IN_BLOCK; i++) {
			state[i] = _mm256_xor_si256(
				state[i], _mm256_loadu_si256((const __m256i*)ref_block->v + i));
			block_XY[i] = _mm256_xor_si256(
				state[i], _mm256_loadu_si256((const __m256i*)next_block->v + i));
		}
	}
	else {
		for (i = 0; i < ARGON2_HWORDS_IN_BLOCK; i++) {
			block_XY[i] = state[i] = _mm256_xor_si256(
				state[i], _mm256_loadu_si256((const __m256i*)ref_block->v + i));
		}
	}

	for (i = 0; i < 4; ++i) {
		BLAKE2_ROUND_1(state[8 * i + 0], state[8 * i + 4], state[8 * i + 1], state[8 * i + 5],
			state[8 * i + 2], state[8 * i + 6], state[8 * i + 3], state[8 * i + 7]);
	}

	for (i = 0; i < 4; ++i) {
		BLAKE2_ROUND_2(state[0 + i], state[4 + i], state[8 + i], state[12 + i],
			state[16 + i], state[20 + i], state[24 + i], state[28 + i]);
	}

	for (i = 0; i < ARGON2_HWORDS_IN_BLOCK; i++) {
		state[i] = _mm256_xor_si256(state[i], block_XY[i]);
		_mm256_storeu_si256((__m256i*)next_block->v + i, state[i]);
	}
}

void randomx_argon2_fill_segment_avx2(const argon2_instance_t* instance,
	argon2_position_t position) {
	block* ref_block = NULL, * curr_block = NULL;
	block address_block, input_block;
	uint64_t pseudo_rand, ref_index, ref_lane;
	uint32_t prev_offset, curr_offset;
	uint32_t starting_index, i;
	__m256i state[ARGON2_HWORDS_IN_BLOCK];

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

	memcpy(state, ((instance->memory + prev_offset)->v), ARGON2_BLOCK_SIZE);

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
			 
			fill_block(state, ref_block, curr_block, 0);
		}
		else {
			if (0 == position.pass) {
				fill_block(state, ref_block, curr_block, 0);
			}
			else {
				fill_block(state, ref_block, curr_block, 1);
			}
		}
	}
}

#endif
