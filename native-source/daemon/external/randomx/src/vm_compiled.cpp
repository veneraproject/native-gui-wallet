  
                                                    

                    

                                                                  
                                                                           
                                                                 
                                                                
                                                                    
                                                                      
                                                                       
                                                   
                                                                       
                                                                        

                                                                               
                                                                             
                                                                      
                                                                            
                                                                          
                                                                          
                                                                          
                                                                             
                                                                             
                                                                    
  

#include "vm_compiled.hpp"
#include "common.hpp"

namespace randomx {

	static_assert(sizeof(MemoryRegisters) == 2 * sizeof(addr_t) + sizeof(uintptr_t), "Invalid alignment of struct randomx::MemoryRegisters");
	static_assert(sizeof(RegisterFile) == 256, "Invalid alignment of struct randomx::RegisterFile");

	template<class Allocator, bool softAes, bool secureJit>
	CompiledVm<Allocator, softAes, secureJit>::CompiledVm() {
		if (!secureJit) {
			compiler.enableAll();  
		}
	}

	template<class Allocator, bool softAes, bool secureJit>
	void CompiledVm<Allocator, softAes, secureJit>::setDataset(randomx_dataset* dataset) {
		datasetPtr = dataset;
	}

	template<class Allocator, bool softAes, bool secureJit>
	void CompiledVm<Allocator, softAes, secureJit>::run(void* seed) {
		VmBase<Allocator, softAes>::generateProgram(seed);
		randomx_vm::initialize();
		if (secureJit) {
			compiler.enableWriting();
		}
		compiler.generateProgram(program, config);
		if (secureJit) {
			compiler.enableExecution();
		}
		mem.memory = datasetPtr->memory + datasetOffset;
		execute();
	}

	template<class Allocator, bool softAes, bool secureJit>
	void CompiledVm<Allocator, softAes, secureJit>::execute() {
#ifdef __aarch64__
		memcpy(reg.f, config.eMask, sizeof(config.eMask));
#endif
		compiler.getProgramFunc()(reg, mem, scratchpad, RANDOMX_PROGRAM_ITERATIONS);
	}

	template class CompiledVm<AlignedAllocator<CacheLineSize>, false, false>;
	template class CompiledVm<AlignedAllocator<CacheLineSize>, true, false>;
	template class CompiledVm<LargePageAllocator, false, false>;
	template class CompiledVm<LargePageAllocator, true, false>;
	template class CompiledVm<AlignedAllocator<CacheLineSize>, false, true>;
	template class CompiledVm<AlignedAllocator<CacheLineSize>, true, true>;
	template class CompiledVm<LargePageAllocator, false, true>;
	template class CompiledVm<LargePageAllocator, true, true>;
}