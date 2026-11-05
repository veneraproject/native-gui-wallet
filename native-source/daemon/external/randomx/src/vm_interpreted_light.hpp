  
                                                    

                    

                                                                  
                                                                           
                                                                 
                                                                
                                                                    
                                                                      
                                                                       
                                                   
                                                                       
                                                                        

                                                                               
                                                                             
                                                                      
                                                                            
                                                                          
                                                                          
                                                                          
                                                                             
                                                                             
                                                                    
  

#pragma once

#include <new>
#include "vm_interpreted.hpp"

namespace randomx {

	template<class Allocator, bool softAes>
	class InterpretedLightVm : public InterpretedVm<Allocator, softAes> {
	public:
		using VmBase<Allocator, softAes>::mem;
		using VmBase<Allocator, softAes>::cachePtr;
		void* operator new(size_t size) {
			void* ptr = AlignedAllocator<CacheLineSize>::allocMemory(size);
			if (ptr == nullptr)
				throw std::bad_alloc();
			return ptr;
		}
		void operator delete(void* ptr) {
			AlignedAllocator<CacheLineSize>::freeMemory(ptr, sizeof(InterpretedLightVm));
		}
		void setDataset(randomx_dataset* dataset) override { }
		void setCache(randomx_cache* cache) override;
	protected:
		void datasetRead(uint64_t address, int_reg_t(&r)[8]) override;
		void datasetPrefetch(uint64_t address) override { }
	};

	using InterpretedLightVmDefault = InterpretedLightVm<AlignedAllocator<CacheLineSize>, true>;
	using InterpretedLightVmHardAes = InterpretedLightVm<AlignedAllocator<CacheLineSize>, false>;
	using InterpretedLightVmLargePage = InterpretedLightVm<LargePageAllocator, true>;
	using InterpretedLightVmLargePageHardAes = InterpretedLightVm<LargePageAllocator, false>;
}
