  
                                                    

                    

                                                                  
                                                                           
                                                                 
                                                                
                                                                    
                                                                      
                                                                       
                                                   
                                                                       
                                                                        

                                                                               
                                                                             
                                                                      
                                                                            
                                                                          
                                                                          
                                                                          
                                                                             
                                                                             
                                                                    
  

#include <new>
#include "allocator.hpp"
#include "intrin_portable.h"
#include "virtual_memory.h"
#include "common.hpp"

namespace randomx {

	template<size_t alignment>
	void* AlignedAllocator<alignment>::allocMemory(size_t count) {
		void *mem = rx_aligned_alloc(count, alignment);
		if (mem == nullptr)
			throw std::bad_alloc();
		return mem;
	}

	template<size_t alignment>
	void AlignedAllocator<alignment>::freeMemory(void* ptr, size_t count) {
		rx_aligned_free(ptr);
	}

	template struct AlignedAllocator<CacheLineSize>;

	void* LargePageAllocator::allocMemory(size_t count) {
		void *mem = allocLargePagesMemory(count);
		if (mem == nullptr)
			throw std::bad_alloc();
		return mem;
	}

	void LargePageAllocator::freeMemory(void* ptr, size_t count) {
		freePagedMemory(ptr, count);
	};

}
