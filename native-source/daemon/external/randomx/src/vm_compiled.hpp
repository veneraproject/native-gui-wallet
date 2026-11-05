  
                                                    

                    

                                                                  
                                                                           
                                                                 
                                                                
                                                                    
                                                                      
                                                                       
                                                   
                                                                       
                                                                        

                                                                               
                                                                             
                                                                      
                                                                            
                                                                          
                                                                          
                                                                          
                                                                             
                                                                             
                                                                    
  

#pragma once

#include <new>
#include <cstdint>
#include "virtual_machine.hpp"
#include "jit_compiler.hpp"
#include "allocator.hpp"
#include "dataset.hpp"

namespace randomx {

	template<class Allocator, bool softAes, bool secureJit>
	class CompiledVm : public VmBase<Allocator, softAes> {
	public:
		void* operator new(size_t size) {
			void* ptr = AlignedAllocator<CacheLineSize>::allocMemory(size);
			if (ptr == nullptr)
				throw std::bad_alloc();
			return ptr;
		}
		void operator delete(void* ptr) {
			AlignedAllocator<CacheLineSize>::freeMemory(ptr, sizeof(CompiledVm));
		}
		CompiledVm();
		void setDataset(randomx_dataset* dataset) override;
		void run(void* seed) override;

		using VmBase<Allocator, softAes>::mem;
		using VmBase<Allocator, softAes>::program;
		using VmBase<Allocator, softAes>::config;
		using VmBase<Allocator, softAes>::reg;
		using VmBase<Allocator, softAes>::scratchpad;
		using VmBase<Allocator, softAes>::datasetPtr;
		using VmBase<Allocator, softAes>::datasetOffset;
	protected:
		void execute();

		JitCompiler compiler;
	};

	using CompiledVmDefault = CompiledVm<AlignedAllocator<CacheLineSize>, true, false>;
	using CompiledVmHardAes = CompiledVm<AlignedAllocator<CacheLineSize>, false, false>;
	using CompiledVmLargePage = CompiledVm<LargePageAllocator, true, false>;
	using CompiledVmLargePageHardAes = CompiledVm<LargePageAllocator, false, false>;
	using CompiledVmDefaultSecure = CompiledVm<AlignedAllocator<CacheLineSize>, true, true>;
	using CompiledVmHardAesSecure = CompiledVm<AlignedAllocator<CacheLineSize>, false, true>;
	using CompiledVmLargePageSecure = CompiledVm<LargePageAllocator, true, true>;
	using CompiledVmLargePageHardAesSecure = CompiledVm<LargePageAllocator, false, true>;
}
