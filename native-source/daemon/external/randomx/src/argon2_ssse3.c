  
                                                    

                    

                                                                  
                                                                           
                                                                 
                                                                
                                                                    
                                                                      
                                                                       
                                                   
                                                                       
                                                                        

                                                                               
                                                                             
                                                                      
                                                                            
                                                                          
                                                                          
                                                                          
                                                                             
                                                                             
                                                                    
  

                                                                                 
                                             
                 
                                                                             
  

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "argon2.h"

#if defined(_MSC_VER)  
#define __SSSE3__
#endif

void randomx_argon2_fill_segment_ssse3(const argon2_instance_t* instance,
	argon2_position_t position);

randomx_argon2_impl* randomx_argon2_impl_ssse3() {
#if defined(__SSSE3__)
	return &randomx_argon2_fill_segment_ssse3;
#endif
	return NULL;
}

#if defined(__SSSE3__)

#include <tmmintrin.h>  

#include "argon2_core.h"

#include "blake2/blamka-round-ssse3.h"
#include "blake2/blake2-impl.h"
#include "blake2/blake2.h"

static void fill_block(__m128i* state, const block* ref_block,
	block* next_block, int with_xor) {
	__m128i block_XY[ARGON2_OWORDS_IN_BLOCK];
	unsigned int i;

	if (with_xor) {
		for (i = 0; i < ARGON2_OWORDS_IN_BLOCK; i++) {
			state[i] = _mm_xor_si128(
				state[i], _mm_loadu_si128((const __m128i*)ref_block->v + i));
			block_XY[i] = _mm_xor_si128(
				state[i], _mm_loadu_si128((const __m128i*)next_block->v + i));
		}
	}
	else {
		for (i = 0; i < ARGON2_OWORDS_IN_BLOCK; i++) {
			block_XY[i] = state[i] = _mm_xor_si128(
				state[i], _mm_loadu_si128((const __m128i*)ref_block->v + i));
		}
	}

	for (i = 0; i < 8; ++i) {
		BLAKE2_ROUND(state[8 * i + 0], state[8 * i + 1], state[8 * i + 2],
			state[8 * i + 3], state[8 * i + 4], state[8 * i + 5],
			state[8 * i + 6], state[8 * i + 7]);
	}

	for (i = 0; i < 8; ++i) {
		BLAKE2_ROUND(state[8 * 0 + i], state[8 * 1 + i], state[8 * 2 + i],
			state[8 * 3 + i], state[8 * 4 + i], state[8 * 5 + i],
			state[8 * 6 + i], state[8 * 7 + i]);
	}

	for (i = 0; i < ARGON2_OWORDS_IN_BLOCK; i++) {
		state[i] = _mm_xor_si128(state[i], block_XY[i]);
		_mm_storeu_si128((__m128i*)next_block->v + i, state[i]);
	}
}

void randomx_argon2_fill_segment_ssse3(const argon2_instance_t* instance,
	argon2_position_t position) {
	block* ref_block = NULL, * curr_block = NULL;
	block address_block, input_block;
	uint64_t pseudo_rand, ref_index, ref_lane;
	uint32_t prev_offset, curr_offset;
	uint32_t starting_index, i;
	__m128i state[ARGON2_OWORDS_IN_BLOCK];

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
