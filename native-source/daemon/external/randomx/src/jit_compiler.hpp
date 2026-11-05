  
                                                    

                    

                                                                  
                                                                           
                                                                 
                                                                
                                                                    
                                                                      
                                                                       
                                                   
                                                                       
                                                                        

                                                                               
                                                                             
                                                                      
                                                                            
                                                                          
                                                                          
                                                                          
                                                                             
                                                                             
                                                                    
  

#pragma once

#include "common.hpp"

namespace randomx {

	struct CodeBuffer {
		uint8_t* code;
		int32_t codePos;
		int32_t rcpCount;

		void emit(const uint8_t* src, int32_t len) {
			memcpy(&code[codePos], src, len);
			codePos += len;
		}

		template<typename T>
		void emit(T src) {
			memcpy(&code[codePos], &src, sizeof(src));
			codePos += sizeof(src);
		}

		void emitAt(int32_t codePos, const uint8_t* src, int32_t len) {
			memcpy(&code[codePos], src, len);
		}

		template<typename T>
		void emitAt(int32_t codePos, T src) {
			memcpy(&code[codePos], &src, sizeof(src));
		}
	};

	struct CompilerState : public CodeBuffer {
		int32_t instructionOffsets[RANDOMX_PROGRAM_SIZE];
		int registerUsage[RegistersCount];
	};
}

#if defined(RANDOMX_COMPILER_X86)
#include "jit_compiler_x86.hpp"
#elif defined(RANDOMX_COMPILER_A64)
#include "jit_compiler_a64.hpp"
#elif defined(RANDOMX_COMPILER_RV64)
#include "jit_compiler_rv64.hpp"
#else
#include "jit_compiler_fallback.hpp"
#endif

#if defined(__OpenBSD__) || defined(__NetBSD__) || (defined(__APPLE__) && defined(__aarch64__))
#define RANDOMX_FORCE_SECURE
#endif
