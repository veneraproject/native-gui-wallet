  
                                                    

                    

                                                                  
                                                                           
                                                                 
                                                                
                                                                    
                                                                      
                                                                       
                                                   
                                                                       
                                                                        

                                                                               
                                                                             
                                                                      
                                                                            
                                                                          
                                                                          
                                                                          
                                                                             
                                                                             
                                                                    
  

#pragma once

#include <cstdint>
#include <vector>
#include <stdexcept>
#include "common.hpp"

namespace randomx {

	class Program;
	struct ProgramConfiguration;
	class SuperscalarProgram;

	class JitCompilerFallback {
	public:
		JitCompilerFallback() {
			throw std::runtime_error("JIT compilation is not supported on this platform");
		}
		void generateProgram(Program&, ProgramConfiguration&) {

		}
		void generateProgramLight(Program&, ProgramConfiguration&, uint32_t) {

		}
		template<size_t N>
		void generateSuperscalarHash(SuperscalarProgram(&programs)[N], std::vector<uint64_t> &) {

		}
		void generateDatasetInitCode() {

		}
		ProgramFunc* getProgramFunc() {
			return nullptr;
		}
		DatasetInitFunc* getDatasetInitFunc() {
			return nullptr;
		}
		uint8_t* getCode() {
			return nullptr;
		}
		size_t getCodeSize() {
			return 0;
		}
		void enableWriting() {}
		void enableExecution() {}
		void enableAll() {}
	};
}