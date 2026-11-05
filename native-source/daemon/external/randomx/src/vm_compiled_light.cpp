  
                                                    

                    

                                                                  
                                                                           
                                                                 
                                                                
                                                                    
                                                                      
                                                                       
                                                   
                                                                       
                                                                        

                                                                               
                                                                             
                                                                      
                                                                            
                                                                          
                                                                          
                                                                          
                                                                             
                                                                             
                                                                    
  

#include "vm_compiled_light.hpp"
#include "common.hpp"
#include <stdexcept>

namespace randomx {

	template<class Allocator, bool softAes, bool secureJit>
	void CompiledLightVm<Allocator, softAes, secureJit>::setCache(randomx_cache* cache) {
		cachePtr = cache;
		mem.memory = cache->memory;
		if (secureJit) {
			compiler.enableWriting();
		}
		compiler.generateSuperscalarHash(cache->programs, cache->reciprocalCache);
		if (secureJit) {
			compiler.enableExecution();
		}
	}

	template<class Allocator, bool softAes, bool secureJit>
	void CompiledLightVm<Allocator, softAes, secureJit>::run(void* seed) {
		VmBase<Allocator, softAes>::generateProgram(seed);
		randomx_vm::initialize();
		if (secureJit) {
			compiler.enableWriting();
		}
		compiler.generateProgramLight(program, config, datasetOffset);
		if (secureJit) {
			compiler.enableExecution();
		}
		CompiledVm<Allocator, softAes, secureJit>::execute();
	}

	template class CompiledLightVm<AlignedAllocator<CacheLineSize>, false, false>;
	template class CompiledLightVm<AlignedAllocator<CacheLineSize>, true, false>;
	template class CompiledLightVm<LargePageAllocator, false, false>;
	template class CompiledLightVm<LargePageAllocator, true, false>;
	template class CompiledLightVm<AlignedAllocator<CacheLineSize>, false, true>;
	template class CompiledLightVm<AlignedAllocator<CacheLineSize>, true, true>;
	template class CompiledLightVm<LargePageAllocator, false, true>;
	template class CompiledLightVm<LargePageAllocator, true, true>;
}