  
                                                    

                    

                                                                  
                                                                           
                                                                 
                                                                
                                                                    
                                                                      
                                                                       
                                                   
                                                                       
                                                                        

                                                                               
                                                                             
                                                                      
                                                                            
                                                                          
                                                                          
                                                                          
                                                                             
                                                                             
                                                                    
  

#pragma once

#include "common.hpp"
#include <sstream>

namespace randomx {

	class Program;
	class SuperscalarProgram;
	class AssemblyGeneratorX86;
	class Instruction;

	typedef void(AssemblyGeneratorX86::*InstructionGenerator)(Instruction&, int);

	class AssemblyGeneratorX86 {
	public:
		void generateProgram(Program& prog);
		void generateAsm(SuperscalarProgram& prog);
		void generateC(SuperscalarProgram& prog);
		void printCode(std::ostream& os) {
			os << asmCode.rdbuf();
		}
	private:
		void genAddressReg(Instruction&, const char*);
		void genAddressRegDst(Instruction&, int);
		int32_t genAddressImm(Instruction&);
		void generateCode(Instruction&, int);
		void traceint(Instruction&);
		void traceflt(Instruction&);
		void tracenop(Instruction&);
		void h_IADD_RS(Instruction&, int);
		void h_IADD_M(Instruction&, int);
		void h_ISUB_R(Instruction&, int);
		void h_ISUB_M(Instruction&, int);
		void h_IMUL_R(Instruction&, int);
		void h_IMUL_M(Instruction&, int);
		void h_IMULH_R(Instruction&, int);
		void h_IMULH_M(Instruction&, int);
		void h_ISMULH_R(Instruction&, int);
		void h_ISMULH_M(Instruction&, int);
		void h_IMUL_RCP(Instruction&, int);
		void h_INEG_R(Instruction&, int);
		void h_IXOR_R(Instruction&, int);
		void h_IXOR_M(Instruction&, int);
		void h_IROR_R(Instruction&, int);
		void h_IROL_R(Instruction&, int);
		void h_ISWAP_R(Instruction&, int);
		void h_FSWAP_R(Instruction&, int);
		void h_FADD_R(Instruction&, int);
		void h_FADD_M(Instruction&, int);
		void h_FSUB_R(Instruction&, int);
		void h_FSUB_M(Instruction&, int);
		void h_FSCAL_R(Instruction&, int);
		void h_FMUL_R(Instruction&, int);
		void h_FDIV_M(Instruction&, int);
		void h_FSQRT_R(Instruction&, int);
		void h_CBRANCH(Instruction&, int);
		void h_CFROUND(Instruction&, int);
		void h_ISTORE(Instruction&, int);
		void h_NOP(Instruction&, int);

		static InstructionGenerator engine[256];
		std::stringstream asmCode;
		int registerUsage[RegistersCount];
	};
}