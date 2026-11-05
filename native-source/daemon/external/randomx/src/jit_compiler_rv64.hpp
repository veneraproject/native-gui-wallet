  
                                              

                    

                                                                  
                                                                           
                                                                 
                                                                
                                                                    
                                                                      
                                                                       
                                                   
                                                                       
                                                                        

                                                                               
                                                                             
                                                                      
                                                                            
                                                                          
                                                                          
                                                                          
                                                                             
                                                                             
                                                                    
  

#pragma once

#include <cstdint>
#include <cstring>
#include <vector>
#include "jit_compiler.hpp"

namespace randomx {

	class Program;
	struct ProgramConfiguration;
	class SuperscalarProgram;
	class Instruction;

	class JitCompilerRV64 {
	public:
		JitCompilerRV64();
		~JitCompilerRV64();
		void generateProgram(Program&, ProgramConfiguration&);
		void generateProgramLight(Program&, ProgramConfiguration&, uint32_t);
		void generateSuperscalarHash(SuperscalarProgram programs[RANDOMX_CACHE_ACCESSES], std::vector<uint64_t>&);
		void generateDatasetInitCode() {}
		ProgramFunc* getProgramFunc() {
			return (ProgramFunc*)entryProgram;
		}
		DatasetInitFunc* getDatasetInitFunc() {
			return (DatasetInitFunc*)entryDataInit;
		}
		uint8_t* getCode() {
			return state.code;
		}
		size_t getCodeSize();
		void enableWriting();
		void enableExecution();
		void enableAll();
	private:
		CompilerState state;
		void* entryDataInit;
		void* entryProgram;
	};
}
