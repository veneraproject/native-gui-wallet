  
                                                    

                    

                                                                  
                                                                           
                                                                 
                                                                
                                                                    
                                                                      
                                                                       
                                                   
                                                                       
                                                                        

                                                                               
                                                                             
                                                                      
                                                                            
                                                                          
                                                                          
                                                                          
                                                                             
                                                                             
                                                                    
  

#include "vm_interpreted_light.hpp"
#include "dataset.hpp"

namespace randomx {

	template<class Allocator, bool softAes>
	void InterpretedLightVm<Allocator, softAes>::setCache(randomx_cache* cache) {
		cachePtr = cache;
		mem.memory = cache->memory;
	}

	template<class Allocator, bool softAes>
	void InterpretedLightVm<Allocator, softAes>::datasetRead(uint64_t address, int_reg_t(&r)[8]) {
		uint32_t itemNumber = address / CacheLineSize;
		int_reg_t rl[8];
		
		initDatasetItem(cachePtr, (uint8_t*)rl, itemNumber);

		for (unsigned q = 0; q < 8; ++q)
			r[q] ^= rl[q];
	}

	template class InterpretedLightVm<AlignedAllocator<CacheLineSize>, false>;
	template class InterpretedLightVm<AlignedAllocator<CacheLineSize>, true>;
	template class InterpretedLightVm<LargePageAllocator, false>;
	template class InterpretedLightVm<LargePageAllocator, true>;
}
