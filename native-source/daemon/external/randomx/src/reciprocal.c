  
                                                    

                    

                                                                  
                                                                           
                                                                 
                                                                
                                                                    
                                                                      
                                                                       
                                                   
                                                                       
                                                                        

                                                                               
                                                                             
                                                                      
                                                                            
                                                                          
                                                                          
                                                                          
                                                                             
                                                                             
                                                                    
  

#include <assert.h>
#include "reciprocal.h"

 
 
 

 

 
 
 
 
 
 
 

 
uint64_t randomx_reciprocal(uint32_t divisor) {

	assert(divisor != 0);

	const uint64_t p2exp63 = 1ULL << 63;
	const uint64_t q = p2exp63 / divisor;
	const uint64_t r = p2exp63 % divisor;

#ifdef __GNUC__
	const uint32_t shift = 64 - __builtin_clzll(divisor);
#else
	uint32_t shift = 32;
	for (uint32_t k = 1U << 31; (k & divisor) == 0; k >>= 1)
		--shift;
#endif

	return (q << shift) + ((r << shift) / divisor);
}

#if !RANDOMX_HAVE_FAST_RECIPROCAL

uint64_t randomx_reciprocal_fast(uint32_t divisor) {
	return randomx_reciprocal(divisor);
}

#endif
