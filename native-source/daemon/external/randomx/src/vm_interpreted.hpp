  
                                                    

                    

                                                                  
                                                                           
                                                                 
                                                                
                                                                    
                                                                      
                                                                       
                                                   
                                                                       
                                                                        

                                                                               
                                                                             
                                                                      
                                                                            
                                                                          
                                                                          
                                                                          
                                                                             
                                                                             
                                                                    
  

#pragma once

#include <new>
#include <vector>
#include "common.hpp"
#include "virtual_machine.hpp"
#include "bytecode_machine.hpp"
#include "intrin_portable.h"
#include "allocator.hpp"

namespace randomx {

	template<class Allocator, bool softAes>
	class InterpretedVm : public VmBase<Allocator, softAes>, public BytecodeMachine {
	public:
		using VmBase<Allocator, softAes>::mem;
		using VmBase<Allocator, softAes>::scratchpad;
		using VmBase<Allocator, softAes>::program;
		using VmBase<Allocator, softAes>::config;
		using VmBase<Allocator, softAes>::reg;
		using VmBase<Allocator, softAes>::datasetPtr;
		using VmBase<Allocator, softAes>::datasetOffset;
		void* operator new(size_t size) {
			void* ptr = AlignedAllocator<CacheLineSize>::allocMemory(size);
			if (ptr == nullptr)
				throw std::bad_alloc();
			return ptr;
		}
		void operator delete(void* ptr) {
			AlignedAllocator<CacheLineSize>::freeMemory(ptr, sizeof(InterpretedVm));
		}
		void run(void* seed) override;
		void setDataset(randomx_dataset* dataset) override;
	protected:
		virtual void datasetRead(uint64_t blockNumber, int_reg_t(&r)[RegistersCount]);
		virtual void datasetPrefetch(uint64_t blockNumber);
	private:
		void execute();

		InstructionByteCode bytecode[RANDOMX_PROGRAM_SIZE];
	};

	using InterpretedVmDefault = InterpretedVm<AlignedAllocator<CacheLineSize>, true>;
	using InterpretedVmHardAes = InterpretedVm<AlignedAllocator<CacheLineSize>, false>;
	using InterpretedVmLargePage = InterpretedVm<LargePageAllocator, true>;
	using InterpretedVmLargePageHardAes = InterpretedVm<LargePageAllocator, false>;
}