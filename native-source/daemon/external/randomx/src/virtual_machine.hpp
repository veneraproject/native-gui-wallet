  
                                                    

                    

                                                                  
                                                                           
                                                                 
                                                                
                                                                    
                                                                      
                                                                       
                                                   
                                                                       
                                                                        

                                                                               
                                                                             
                                                                      
                                                                            
                                                                          
                                                                          
                                                                          
                                                                             
                                                                             
                                                                    
  

#pragma once

#include <cstdint>
#include "common.hpp"
#include "program.hpp"

 
class randomx_vm {
public:
	virtual ~randomx_vm() = 0;
	virtual void allocate() = 0;
	virtual void getFinalResult(void* out, size_t outSize) = 0;
	virtual void hashAndFill(void* out, size_t outSize, uint64_t *fill_state) = 0;
	virtual void setDataset(randomx_dataset* dataset) { }
	virtual void setCache(randomx_cache* cache) { }
	virtual void initScratchpad(void* seed) = 0;
	virtual void run(void* seed) = 0;
	void resetRoundingMode();
	randomx::RegisterFile *getRegisterFile() {
		return &reg;
	}
	const void* getScratchpad() {
		return scratchpad;
	}
	const randomx::Program& getProgram()
	{
		return program;
	}
	const uint8_t* getMemory() const {
		return mem.memory;
	}
protected:
	void initialize();
	alignas(64) randomx::Program program;
	alignas(64) randomx::RegisterFile reg;
	alignas(16) randomx::ProgramConfiguration config;
	randomx::MemoryRegisters mem;
	uint8_t* scratchpad = nullptr;
	union {
		randomx_cache* cachePtr = nullptr;
		randomx_dataset* datasetPtr;
	};
	uint64_t datasetOffset;
public:
	std::string cacheKey;
	alignas(16) uint64_t tempHash[8];  
};

namespace randomx {

	template<class Allocator, bool softAes>
	class VmBase : public randomx_vm {
	public:
		~VmBase() override;
		void allocate() override;
		void initScratchpad(void* seed) override;
		void getFinalResult(void* out, size_t outSize) override;
		void hashAndFill(void* out, size_t outSize, uint64_t *fill_state) override;
	protected:
		void generateProgram(void* seed);
	};

}
