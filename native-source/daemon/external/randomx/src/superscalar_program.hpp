  
                                                    

                    

                                                                  
                                                                           
                                                                 
                                                                
                                                                    
                                                                      
                                                                       
                                                   
                                                                       
                                                                        

                                                                               
                                                                             
                                                                      
                                                                            
                                                                          
                                                                          
                                                                          
                                                                             
                                                                             
                                                                    
  

#pragma once

#include <cstdint>
#include "instruction.hpp"
#include "common.hpp"

namespace randomx {

	class SuperscalarProgram {
	public:
		Instruction& operator()(int pc) {
			return programBuffer[pc];
		}
		friend std::ostream& operator<<(std::ostream& os, const SuperscalarProgram& p) {
			p.print(os);
			return os;
		}
		uint32_t getSize() {
			return size;
		}
		void setSize(uint32_t val) {
			size = val;
		}
		int getAddressRegister() {
			return addrReg;
		}
		void setAddressRegister(int val) {
			addrReg = val;
		}

		Instruction programBuffer[SuperscalarMaxSize];
		uint32_t size
#ifndef NDEBUG
			= 0
#endif
			;
		int addrReg;
		double ipc;
		int codeSize;
		int macroOps;
		int decodeCycles;
		int cpuLatency;
		int asicLatency;
		int mulCount;
		int cpuLatencies[8];
		int asicLatencies[8];
	private:
		void print(std::ostream& os) const {
			for (unsigned i = 0; i < size; ++i) {
				auto instr = programBuffer[i];
				os << instr;
			}
		}
	};

}