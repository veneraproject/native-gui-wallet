  
                                                    

                    

                                                                  
                                                                           
                                                                 
                                                                
                                                                    
                                                                      
                                                                       
                                                   
                                                                       
                                                                        

                                                                               
                                                                             
                                                                      
                                                                            
                                                                          
                                                                          
                                                                          
                                                                             
                                                                             
                                                                    
  

#pragma once

#include <cstdint>
#include <vector>
#include <type_traits>
#include "common.hpp"
#include "superscalar_program.hpp"
#include "allocator.hpp"
#include "argon2.h"

 
struct randomx_dataset {
	uint8_t* memory = nullptr;
	randomx::DatasetDeallocFunc* dealloc;
};

 
struct randomx_cache {
	uint8_t* memory = nullptr;
	randomx::CacheDeallocFunc* dealloc;
	randomx::JitCompiler* jit;
	randomx::CacheInitializeFunc* initialize;
	randomx::DatasetInitFunc* datasetInit;
	randomx::SuperscalarProgram programs[RANDOMX_CACHE_ACCESSES];
	std::vector<uint64_t> reciprocalCache;
	std::string cacheKey;
	randomx_argon2_impl* argonImpl;

	bool isInitialized() {
		return programs[0].getSize() != 0;
	}
};

 
static_assert(std::is_standard_layout<randomx_dataset>(), "randomx_dataset must be a standard-layout struct");

 
#if defined(_MSC_VER) && !defined(__INTEL_COMPILER) && defined(_DEBUG)
#define TO_STR(x) #x
#define STR(x) TO_STR(x)
#pragma message ( __FILE__ "(" STR(__LINE__) ") warning: check std::is_standard_layout<randomx_cache>() is disabled for Debug configuration. JIT mode will crash." )
#undef STR
#undef TO_STR
#else
static_assert(std::is_standard_layout<randomx_cache>(), "randomx_cache must be a standard-layout struct");
#endif

namespace randomx {

	using DefaultAllocator = AlignedAllocator<CacheLineSize>;

	template<class Allocator>
	void deallocDataset(randomx_dataset* dataset) {
		if (dataset->memory != nullptr)
			Allocator::freeMemory(dataset->memory, DatasetSize);
	}

	template<class Allocator>
	void deallocCache(randomx_cache* cache);

	void initCache(randomx_cache*, const void*, size_t);
	void initCacheCompile(randomx_cache*, const void*, size_t);
	void initDatasetItem(randomx_cache* cache, uint8_t* out, uint64_t blockNumber);
	void initDataset(randomx_cache* cache, uint8_t* dataset, uint32_t startBlock, uint32_t endBlock);

	inline randomx_argon2_impl* selectArgonImpl(randomx_flags flags) {
		if (flags & RANDOMX_FLAG_ARGON2_AVX2) {
			return randomx_argon2_impl_avx2();
		}
		if (flags & RANDOMX_FLAG_ARGON2_SSSE3) {
			return randomx_argon2_impl_ssse3();
		}
		return &randomx_argon2_fill_segment_ref;
	}
}
