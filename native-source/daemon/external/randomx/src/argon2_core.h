  
                                                    

                    

                                                                  
                                                                           
                                                                 
                                                                
                                                                    
                                                                      
                                                                       
                                                   
                                                                       
                                                                        

                                                                               
                                                                             
                                                                      
                                                                            
                                                                          
                                                                          
                                                                          
                                                                             
                                                                             
                                                                    
  

                                                                                 
                                             
                 
                                                                             
  

#ifndef ARGON2_CORE_H
#define ARGON2_CORE_H

#include <stdint.h>
#include "argon2.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define CONST_CAST(x) (x)(uintptr_t)

  

enum argon2_core_constants {
	 
	ARGON2_BLOCK_SIZE = 1024,
	ARGON2_QWORDS_IN_BLOCK = ARGON2_BLOCK_SIZE / 8,
	ARGON2_OWORDS_IN_BLOCK = ARGON2_BLOCK_SIZE / 16,
	ARGON2_HWORDS_IN_BLOCK = ARGON2_BLOCK_SIZE / 32,
	ARGON2_512BIT_WORDS_IN_BLOCK = ARGON2_BLOCK_SIZE / 64,

	 
 
 
	ARGON2_ADDRESSES_IN_BLOCK = 128,

	 
	ARGON2_PREHASH_DIGEST_LENGTH = 64,
	ARGON2_PREHASH_SEED_LENGTH = 72
};

 

 
 
 
 
 
typedef struct block_ { uint64_t v[ARGON2_QWORDS_IN_BLOCK]; } block;

 
 
 
 
 
 
typedef struct Argon2_instance_t {
	block *memory;           
	uint32_t version;
	uint32_t passes;         
	uint32_t memory_blocks;  
	uint32_t segment_length;
	uint32_t lane_length;
	uint32_t lanes;
	uint32_t threads;
	argon2_type type;
	int print_internals;  
	argon2_context *context_ptr;  
	randomx_argon2_impl *impl;
} argon2_instance_t;

 
 
 
 
typedef struct Argon2_position_t {
	uint32_t pass;
	uint32_t lane;
	uint8_t slice;
	uint32_t index;
} argon2_position_t;

 
typedef struct Argon2_thread_data {
	argon2_instance_t *instance_ptr;
	argon2_position_t pos;
} argon2_thread_data;

 

 
 
 
 
 
 
 
 
 
 
uint32_t randomx_argon2_index_alpha(const argon2_instance_t *instance,
	const argon2_position_t *position, uint32_t pseudo_rand,
	int same_lane);

 
 
 
 
 
 
 
int randomx_argon2_validate_inputs(const argon2_context *context);

 
 
 
 
 
 
 
 
 
 
int randomx_argon2_initialize(argon2_instance_t *instance, argon2_context *context);

 
 
 
 
 
 
int randomx_argon2_fill_memory_blocks(argon2_instance_t* instance);

#if defined(__cplusplus)
}
#endif

#endif
