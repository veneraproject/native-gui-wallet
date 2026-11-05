  
                                                    

                    

                                                                  
                                                                           
                                                                 
                                                                
                                                                    
                                                                      
                                                                       
                                                   
                                                                       
                                                                        

                                                                               
                                                                             
                                                                      
                                                                            
                                                                          
                                                                          
                                                                          
                                                                             
                                                                             
                                                                    
  

#pragma once

#include <new>
#include "vm_compiled.hpp"

namespace randomx {

	template<class Allocator, bool softAes, bool secureJit>
	class CompiledLightVm : public CompiledVm<Allocator, softAes, secureJit> {
	public:
		void* operator new(size_t size) {
			void* ptr = AlignedAllocator<CacheLineSize>::allocMemory(size);
			if (ptr == nullptr)
				throw std::bad_alloc();
			return ptr;
		}
		void operator delete(void* ptr) {
			AlignedAllocator<CacheLineSize>::freeMemory(ptr, sizeof(CompiledLightVm));
		}
		void setCache(randomx_cache* cache) override;
		void setDataset(randomx_dataset* dataset) override { }
		void run(void* seed) override;

		using CompiledVm<Allocator, softAes, secureJit>::mem;
		using CompiledVm<Allocator, softAes, secureJit>::compiler;
		using CompiledVm<Allocator, softAes, secureJit>::program;
		using CompiledVm<Allocator, softAes, secureJit>::config;
		using CompiledVm<Allocator, softAes, secureJit>::cachePtr;
		using CompiledVm<Allocator, softAes, secureJit>::datasetOffset;
	};

	using CompiledLightVmDefault = CompiledLightVm<AlignedAllocator<CacheLineSize>, true, false>;
	using CompiledLightVmHardAes = CompiledLightVm<AlignedAllocator<CacheLineSize>, false, false>;
	using CompiledLightVmLargePage = CompiledLightVm<LargePageAllocator, true, false>;
	using CompiledLightVmLargePageHardAes = CompiledLightVm<LargePageAllocator, false, false>;
	using CompiledLightVmDefaultSecure = CompiledLightVm<AlignedAllocator<CacheLineSize>, true, true>;
	using CompiledLightVmHardAesSecure = CompiledLightVm<AlignedAllocator<CacheLineSize>, false, true>;
	using CompiledLightVmLargePageSecure = CompiledLightVm<LargePageAllocator, true, true>;
	using CompiledLightVmLargePageHardAesSecure = CompiledLightVm<LargePageAllocator, false, true>;
}