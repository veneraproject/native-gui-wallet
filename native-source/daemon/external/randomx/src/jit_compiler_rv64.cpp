  
                                              

                    

                                                                  
                                                                           
                                                                 
                                                                
                                                                    
                                                                      
                                                                       
                                                   
                                                                       
                                                                        

                                                                               
                                                                             
                                                                      
                                                                            
                                                                          
                                                                          
                                                                          
                                                                             
                                                                             
                                                                    
  

#include <stdexcept>
#include <cstring>
#include <climits>
#include <cassert>
#include "jit_compiler_rv64.hpp"
#include "jit_compiler_rv64_static.hpp"
#include "superscalar.hpp"
#include "program.hpp"
#include "reciprocal.h"
#include "virtual_memory.h"


namespace {
#define HANDLER_ARGS randomx::CompilerState& state, randomx::Instruction isn, int i
	using InstructionHandler = void(HANDLER_ARGS);
	extern InstructionHandler* opcodeMap1[256];
}

namespace rv64 {
	constexpr uint16_t C_LUI    =     0x6001;
	constexpr uint32_t LUI      = 0x00000037;
	constexpr uint16_t C_ADDI   =     0x0001;
	constexpr uint32_t ADDI     = 0x00000013;
	constexpr uint32_t ADDIW    = 0x0000001b;
	constexpr uint16_t C_ADD    =     0x9002;
	constexpr uint32_t ADD      = 0x00000033;
	constexpr uint32_t SHXADD   = 0x20000033;  
	constexpr uint32_t SLL      = 0x00001033;
	constexpr uint32_t SRL      = 0x00005033;
	constexpr uint32_t SLLI     = 0x00001013;
	constexpr uint32_t C_SLLI   =     0x0002;
	constexpr uint32_t SRLI     = 0x00005013;
	constexpr uint32_t AND      = 0x00007033;
	constexpr uint32_t ANDI     = 0x00007013;
	constexpr uint16_t C_AND    =     0x8c61;
	constexpr uint16_t C_ANDI   =     0x8801;
	constexpr uint32_t OR       = 0x00006033;
	constexpr uint16_t C_OR     =     0x8c41;
	constexpr uint32_t XOR      = 0x00004033;
	constexpr uint16_t C_XOR    =     0x8c21;
	constexpr uint32_t LD       = 0x00003003;
	constexpr uint16_t C_LD     =     0x6000;
	constexpr uint16_t C_LW     =     0x4000;
	constexpr uint32_t SD       = 0x00003023;
	constexpr uint32_t SUB      = 0x40000033;
	constexpr uint16_t C_SUB    =     0x8c01;
	constexpr uint32_t MUL      = 0x02000033;
	constexpr uint32_t MULHU    = 0x02003033;
	constexpr uint32_t MULH     = 0x02001033;
	constexpr uint16_t C_MV     =     0x8002;
	constexpr uint32_t ROR      = 0x60005033;  
	constexpr uint32_t RORI     = 0x60005013;  
	constexpr uint32_t ROL      = 0x60001033;  
	constexpr uint32_t FMV_X_D  = 0xe2000053;
	constexpr uint32_t FMV_D_X  = 0xf2000053;
	constexpr uint32_t FMV_D    = 0x22000053;
	constexpr uint32_t FADD_D   = 0x02007053;
	constexpr uint32_t FSUB_D   = 0x0a007053;
	constexpr uint32_t FMUL_D   = 0x12007053;
	constexpr uint32_t FDIV_D   = 0x1a007053;
	constexpr uint32_t FSQRT_D  = 0x5a007053;
	constexpr uint32_t FCVT_D_W = 0xd2000053;
	constexpr uint32_t FSRM     = 0x00201073;
	constexpr uint16_t C_BEQZ   =     0xc001;
	constexpr uint32_t BEQ      = 0x00000063;
	constexpr uint16_t C_BNEZ   =     0xe001;
	constexpr uint32_t JAL      = 0x0000006f;
	constexpr uint16_t C_RET    =     0x8082;
}

namespace randomx {

	constexpr size_t MaxRandomXInstrCodeSize = 56;      
	constexpr size_t MaxSuperscalarInstrSize = 12;      
	constexpr size_t SuperscalarProgramHeader = 136;    
	constexpr size_t CodeAlign = 4096;                  
	constexpr size_t LiteralPoolSize = CodeAlign;
	constexpr size_t SuperscalarLiteraPoolSize = RANDOMX_CACHE_ACCESSES * CodeAlign;
	constexpr size_t ReserveCodeSize = CodeAlign;   

	constexpr size_t RandomXCodeSize = alignSize(LiteralPoolSize + ReserveCodeSize + MaxRandomXInstrCodeSize * RANDOMX_PROGRAM_SIZE, CodeAlign);
	constexpr size_t SuperscalarSize = alignSize(SuperscalarLiteraPoolSize + ReserveCodeSize + (SuperscalarProgramHeader + MaxSuperscalarInstrSize * SuperscalarMaxSize) * RANDOMX_CACHE_ACCESSES, CodeAlign);

	static_assert(RandomXCodeSize < INT32_MAX / 2, "RandomXCodeSize is too large");
	static_assert(SuperscalarSize < INT32_MAX / 2, "SuperscalarSize is too large");

	constexpr uint32_t CodeSize = RandomXCodeSize + SuperscalarSize;
	constexpr uint32_t ExecutableSize = CodeSize - LiteralPoolSize;

	constexpr int32_t LiteralPoolOffset = LiteralPoolSize / 2;
	constexpr int32_t SuperScalarLiteralPoolOffset = RandomXCodeSize;
	constexpr int32_t SuperScalarLiteralPoolRefOffset = RandomXCodeSize + (RANDOMX_CACHE_ACCESSES - 1) * LiteralPoolSize + LiteralPoolOffset;
	constexpr int32_t SuperScalarHashOffset = SuperScalarLiteralPoolOffset + SuperscalarLiteraPoolSize;

	constexpr int maskLog2(uint32_t x, int prev) {
		return x == 1 ? prev : maskLog2(x >> 1, prev + 1);
	}

	constexpr int32_t unsigned32ToSigned2sCompl(uint32_t x) {
		return (-1 == ~0) ? (int32_t)x : (x > INT32_MAX ? (-(int32_t)(UINT32_MAX - x) - 1) : (int32_t)x);
	}

	constexpr int MaskL1Shift = 32 - maskLog2(RANDOMX_SCRATCHPAD_L1, 0);
	constexpr int MaskL2Shift = 32 - maskLog2(RANDOMX_SCRATCHPAD_L2, 0);
	constexpr int MaskL3Shift = 32 - maskLog2(RANDOMX_SCRATCHPAD_L3, 0);

	constexpr int RcpLiteralsOffset = 144;

	constexpr int LiteralPoolReg = 3;  
	constexpr int SpadReg = 5;   
	constexpr int DataReg = 6;   
	constexpr int SuperscalarReg = 7;  
	constexpr int SshTmp1Reg = 28;  
	constexpr int SshTmp2Reg = 29;  
	constexpr int SshPoolReg = 30;  
	constexpr int SshRcpReg = 31;  
	constexpr int Tmp1Reg = 8;   
	constexpr int Tmp2Reg = 9;   
	constexpr int Tmp1RegF = 24;   
	constexpr int Tmp2RegF = 25;   
	constexpr int MaskL1Reg = 10;  
	constexpr int MaskL2Reg = 11;  
	constexpr int MaskFscalReg = 12;  
	constexpr int MaskEclear = 13;  
	constexpr int MaskEsetLo = 14;  
	constexpr int MaskEsetHi = 15;  
	constexpr int MaskL3Reg = 1;  
	constexpr int ReturnReg = 1;  
	constexpr int SpAddr0Reg = 26;  
	constexpr int OffsetXC = -8;  
	constexpr int OffsetR = 16;  
	constexpr int OffsetF = 0;   
	constexpr int OffsetE = 8;  
	constexpr int OffsetA = 16;   
	constexpr int OffsetRcp = 28;  
	constexpr int OffsetRcpF = 22;  
	constexpr int OffsetSsh = 8;  

	 
	constexpr int rvrd(int reg) {
		return reg << 7;
	}

	 
	constexpr int rvrs1(int reg) {
		return reg << 15;
	}

	 
	constexpr int rvrs2(int reg) {
		return reg << 20;
	}

	 
	constexpr int rvcrs(int reg) {
		return reg << 2;
	}

	 
	constexpr uint32_t rvi(uint32_t op, int rd, int rs1, int rs2 = 0) {
		return op | rvrs2(rs2) | rvrs1(rs1) | rvrd(rd);
	}

	 
	constexpr uint16_t rvc(uint16_t op, int rd, int rs) {
		return op | rvrd(rd) | rvcrs(rs);
	}

	 
	constexpr uint16_t rvc(uint16_t op, int imm5, int rd, int imm40) {
		return op | (imm5 << 12) | rvrd(rd) | (imm40 << 2);
	}

	constexpr int regR(int reg) {
		return reg + OffsetR;
	}

	constexpr int regLoA(int reg) {
		return 2 * reg + OffsetA;
	}

	constexpr int regHiA(int reg) {
		return 2 * reg + OffsetA + 1;
	}

	constexpr int regLoF(int reg) {
		return 2 * reg + OffsetF;
	}

	constexpr int regHiF(int reg) {
		return 2 * reg + OffsetF + 1;
	}

	constexpr int regLoE(int reg) {
		return 2 * reg + OffsetE;
	}

	constexpr int regHiE(int reg) {
		return 2 * reg + OffsetE + 1;
	}

	constexpr int regRcp(int reg) {
		return reg + OffsetRcp;
	}

	constexpr int regRcpF(int reg) {
		return reg + OffsetRcpF;
	}

	constexpr int regSS(int reg) {
		return reg + OffsetSsh;
	}

	static const uint8_t* codeLiterals = (uint8_t*)&randomx_riscv64_literals;
	static const uint8_t* codeLiteralsEnd = (uint8_t*)&randomx_riscv64_literals_end;
	static const uint8_t* codeDataInit = (uint8_t*)&randomx_riscv64_data_init;
	static const uint8_t* codeFixDataCall = (uint8_t*)&randomx_riscv64_fix_data_call;
	static const uint8_t* codePrologue = (uint8_t*)&randomx_riscv64_prologue;
	static const uint8_t* codeLoopBegin = (uint8_t*)&randomx_riscv64_loop_begin;
	static const uint8_t* codeDataRead = (uint8_t*)&randomx_riscv64_data_read;
	static const uint8_t* codeDataReadLight = (uint8_t*)&randomx_riscv64_data_read_light;
	static const uint8_t* codeFixLoopCall = (uint8_t*)&randomx_riscv64_fix_loop_call;
	static const uint8_t* codeSpadStore = (uint8_t*)&randomx_riscv64_spad_store;
	static const uint8_t* codeSpadStoreHardAes = (uint8_t*)&randomx_riscv64_spad_store_hardaes;
	static const uint8_t* codeSpadStoreSoftAes = (uint8_t*)&randomx_riscv64_spad_store_softaes;
	static const uint8_t* codeLoopEnd = (uint8_t*)&randomx_riscv64_loop_end;
	static const uint8_t* codeFixContinueLoop = (uint8_t*)&randomx_riscv64_fix_continue_loop;
	static const uint8_t* codeEpilogue = (uint8_t*)&randomx_riscv64_epilogue;
	static const uint8_t* codeSoftAes = (uint8_t*)&randomx_riscv64_softaes;
	static const uint8_t* codeProgramEnd = (uint8_t*)&randomx_riscv64_program_end;
	static const uint8_t* codeSshInit = (uint8_t*)&randomx_riscv64_ssh_init;
	static const uint8_t* codeSshLoad = (uint8_t*)&randomx_riscv64_ssh_load;
	static const uint8_t* codeSshPrefetch = (uint8_t*)&randomx_riscv64_ssh_prefetch;
	static const uint8_t* codeSshEnd = (uint8_t*)&randomx_riscv64_ssh_end;

	static const int32_t sizeLiterals = codeLiteralsEnd - codeLiterals;
	static const int32_t sizeDataInit = codePrologue - codeDataInit;
	static const int32_t sizePrologue = codeLoopBegin - codePrologue;
	static const int32_t sizeLoopBegin = codeDataRead - codeLoopBegin;
	static const int32_t sizeDataRead = codeDataReadLight - codeDataRead;
	static const int32_t sizeDataReadLight = codeSpadStore - codeDataReadLight;
	static const int32_t sizeSpadStore = codeSpadStoreHardAes - codeSpadStore;
	static const int32_t sizeSpadStoreSoftAes = codeLoopEnd - codeSpadStoreSoftAes;
	static const int32_t sizeLoopEnd = codeEpilogue - codeLoopEnd;
	static const int32_t sizeEpilogue = codeSoftAes - codeEpilogue;
	static const int32_t sizeSoftAes = codeProgramEnd - codeSoftAes;
	static const int32_t sizeSshInit = codeSshLoad - codeSshInit;
	static const int32_t sizeSshLoad = codeSshPrefetch - codeSshLoad;
	static const int32_t sizeSshPrefetch = codeSshEnd - codeSshPrefetch;

	static const int32_t offsetFixDataCall = codeFixDataCall - codeDataInit;
	static const int32_t offsetFixLoopCall = codeFixLoopCall - codeDataReadLight;
	static const int32_t offsetFixContinueLoop = codeFixContinueLoop - codeLoopEnd;

	static const int32_t LoopTopPos = LiteralPoolSize + sizeDataInit + sizePrologue;
	static const int32_t RandomXCodePos = LoopTopPos + sizeLoopBegin;

	static void clearCache(CodeBuffer& buf) {
#ifdef __GNUC__
		__builtin___clear_cache((char*)buf.code, (char*)(buf.code + CodeSize));
#endif
	}

	 
	 
	static void emitImm32(CodeBuffer& buf, int32_t imm, int dst, int src = 0, int tmp = 0) {

		 
		int32_t limm = (imm << 20) >> 20;
		 
		int32_t uimm = (imm >> 12) + (limm < 0);

		 
		 
		if (uimm == 0) {
			 
			buf.emit(rvi(rv64::ADDI, dst, src, limm));
			return;
		}

		 
		int dst1 = src != dst ? dst : tmp;
		assert(dst1 != 0);
		 
		int src1 = src != dst ? src : dst1;

		 
		if (uimm >= -32 && uimm <= 31) {
			 
			buf.emit(rvc(rv64::C_LUI, (uimm < 0), dst1, (uimm & 31)));
		}
		else {
			 
			buf.emit(rv64::LUI | (uimm << 12) | rvrd(dst1));
		}
		 
		if (limm != 0) {
			 
			 
			 
			buf.emit(rvi(rv64::ADDIW, dst1, dst1, limm));
		}
		 
		if (src1 != 0) {
			 
			buf.emit(rvc(rv64::C_ADD, dst, src1));
		}
	}

	 
	 
	static void genAddressRegImm(CodeBuffer& buf, const Instruction& isn) {
		 
		int32_t imm = unsigned32ToSigned2sCompl(isn.getImm32()) & ScratchpadL3Mask;
		 
		emitImm32(buf, imm, Tmp2Reg, SpadReg, Tmp1Reg);
	}

	 
	 
	static void genAddressReg(CodeBuffer& buf, const Instruction& isn) {
		int shift, maskReg;
		if (isn.getModMem()) {
			shift = MaskL1Shift;
			maskReg = MaskL1Reg;
		}
		else {
			shift = MaskL2Shift;
			maskReg = MaskL2Reg;
		}
		int32_t imm = unsigned32ToSigned2sCompl(isn.getImm32());
		imm = (imm << shift) >> shift;
		 
		emitImm32(buf, imm, Tmp2Reg, regR(isn.src), Tmp1Reg);
		 
		buf.emit(rvc(rv64::C_AND, (Tmp2Reg + OffsetXC), (maskReg + OffsetXC)));
		 
		buf.emit(rvc(rv64::C_ADD, Tmp2Reg, SpadReg));
	}

	 
	static void loadFromScratchpad(CodeBuffer& buf, const Instruction& isn) {
		if (isn.src != isn.dst) {
			 
			genAddressReg(buf, isn);
		}
		else {
			 
			genAddressRegImm(buf, isn);
		}
		 
		buf.emit(rvc(rv64::C_LD, Tmp2Reg + OffsetXC, Tmp1Reg + OffsetXC));
	}

	 
	 
	static void genAddressRegDst(CodeBuffer& buf, const Instruction& isn) {
		if (isn.getModCond() < StoreL3Condition) {
			int shift, maskReg;
			if (isn.getModMem()) {
				shift = MaskL1Shift;
				maskReg = MaskL1Reg;
			}
			else {
				shift = MaskL2Shift;
				maskReg = MaskL2Reg;
			}
			int32_t imm = unsigned32ToSigned2sCompl(isn.getImm32());
			imm = (imm << shift) >> shift;
			 
			emitImm32(buf, imm, Tmp2Reg, regR(isn.dst), Tmp1Reg);
			 
			buf.emit(rvc(rv64::C_AND, Tmp2Reg + OffsetXC, maskReg + OffsetXC));
			 
			buf.emit(rvc(rv64::C_ADD, Tmp2Reg, SpadReg));
		}
		else {
			int shift = MaskL3Shift;
			int32_t imm = unsigned32ToSigned2sCompl(isn.getImm32());
			imm = (imm << shift) >> shift;
			 
			emitImm32(buf, imm, Tmp2Reg, regR(isn.dst), Tmp1Reg);
			 
			buf.emit(rvi(rv64::AND, Tmp2Reg, Tmp2Reg, MaskL3Reg));
			 
			buf.emit(rvc(rv64::C_ADD, Tmp2Reg, SpadReg));
		}
	}

	static void emitRcpLiteral1(CodeBuffer& buf, uint64_t literal) {
		 
		if (buf.rcpCount < 238) {
			buf.emitAt(LiteralPoolOffset + RcpLiteralsOffset + buf.rcpCount * 8, literal);
			buf.rcpCount++;
		}
		 
		else if (buf.rcpCount < 494) {
			buf.emitAt(buf.rcpCount * 8 - (2048 - RcpLiteralsOffset), literal);
			buf.rcpCount++;
		}
		else {
			 
			throw std::runtime_error("Literal pool overflow");
		}
	}

	static void emitRcpLiteral2(CodeBuffer& buf, uint64_t literal, int32_t numLiterals) {
		 
		int32_t offset = 2040 - buf.rcpCount * 8;
		buf.emitAt(SuperScalarLiteralPoolRefOffset + offset, literal);
		buf.rcpCount++;
		if (buf.rcpCount >= numLiterals) {
			return;
		}
		 
		offset -= 8;
		int32_t imm = offset & 0xfff;
		 
		buf.emit(rvi(rv64::LD, SshRcpReg, SshPoolReg, imm));
		if (imm == 0x800) {
			 
			 
			buf.emit(rvc(rv64::C_LUI, 1, SshTmp2Reg, 31));
			 
			buf.emit(rvc(rv64::C_ADD, SshPoolReg, SshTmp2Reg));
		}
	}

	static void emitJump(CodeBuffer& buf, int dst, int32_t codePos, int32_t targetPos) {
		int32_t imm = targetPos - codePos;
		int32_t imm20 = (imm < 0) << 11;
		int32_t imm1912 = (imm >> 7) & 8160;
		int32_t imm11 = (imm >> 11) & 1;
		int32_t imm101 = imm & 2046;
		 
		buf.emitAt(codePos, rvi(rv64::JAL, dst + imm1912, 0, imm20 + imm101 + imm11));
	}

	static void emitInstruction(CompilerState& state, Instruction isn, int i) {
		state.instructionOffsets[i] = state.codePos;
		opcodeMap1[isn.opcode](state, isn, i);
	}

	static void emitProgramPrefix(CompilerState& state, Program& prog, ProgramConfiguration& pcfg) {
		state.codePos = RandomXCodePos;
		state.rcpCount = 0;
		state.emitAt(LiteralPoolOffset + sizeLiterals, pcfg.eMask[0]);
		state.emitAt(LiteralPoolOffset + sizeLiterals + 8, pcfg.eMask[1]);
		for (unsigned i = 0; i < RegistersCount; ++i) {
			state.registerUsage[i] = -1;
		}
		for (unsigned i = 0; i < prog.getSize(); ++i) {
			Instruction instr = prog(i);
			instr.src %= RegistersCount;
			instr.dst %= RegistersCount;
			emitInstruction(state, instr, i);
		}
	}

	static void emitProgramSuffix(CompilerState& state, ProgramConfiguration& pcfg) {
		state.emit(codeSpadStore, sizeSpadStore);
		int32_t fixPos = state.codePos;
		state.emit(codeLoopEnd, sizeLoopEnd);
		 
		state.emitAt(fixPos, rvi(rv64::XOR, SpAddr0Reg, regR(pcfg.readReg0), regR(pcfg.readReg1)));
		fixPos += offsetFixContinueLoop;
		 
		emitJump(state, 0, fixPos, LoopTopPos);
		state.emit(codeEpilogue, sizeEpilogue);
	}

	static void generateSuperscalarCode(CodeBuffer& buf, Instruction isn, const std::vector<uint64_t>& reciprocalCache) {
		switch ((SuperscalarInstructionType)isn.opcode)
		{
		case randomx::SuperscalarInstructionType::ISUB_R:
			 
			buf.emit(rvc(rv64::C_SUB, regSS(isn.dst) + OffsetXC, regSS(isn.src) + OffsetXC));
			break;
		case randomx::SuperscalarInstructionType::IXOR_R:
			 
			buf.emit(rvc(rv64::C_XOR, regSS(isn.dst) + OffsetXC, regSS(isn.src) + OffsetXC));
			break;
		case randomx::SuperscalarInstructionType::IADD_RS:
			{
				int shift = isn.getModShift();
				if (shift == 0) {
					 
					buf.emit(rvc(rv64::C_ADD, regSS(isn.dst), regSS(isn.src)));
				}
				else {
#ifdef __riscv_zba
				 
				buf.emit(rv64::SHXADD | rvrs2(regSS(isn.dst)) | rvrs1(regSS(isn.src)) | (shift << 13) | rvrd(regSS(isn.dst)));
#else
				 
				buf.emit(rvi(rv64::SLLI, SshTmp1Reg, regSS(isn.src), shift));
				 
				buf.emit(rvc(rv64::C_ADD, regSS(isn.dst), SshTmp1Reg));
#endif
				}
			}
			break;
		case randomx::SuperscalarInstructionType::IMUL_R:
			 
			buf.emit(rvi(rv64::MUL, regSS(isn.dst), regSS(isn.dst), regSS(isn.src)));
			break;
		case randomx::SuperscalarInstructionType::IROR_C:
			{
#ifdef __riscv_zbb
				int32_t imm = isn.getImm32() & 63;
				 
				buf.emit(rvi(rv64::RORI, regSS(isn.dst), regSS(isn.dst), imm));
#else
				int32_t immr = isn.getImm32() & 63;
				int32_t imml = -immr & 63;
				int32_t imml5 = imml >> 5;
				int32_t imml40 = imml & 31;
				 
				buf.emit(rvi(rv64::SRLI, SshTmp1Reg, regSS(isn.dst), immr));
				 
				buf.emit(rvc(rv64::C_SLLI, imml5, regSS(isn.dst), imml40));
				 
				buf.emit(rvi(rv64::OR, regSS(isn.dst), regSS(isn.dst), SshTmp1Reg));
#endif
			}
			break;
		case randomx::SuperscalarInstructionType::IADD_C7:
		case randomx::SuperscalarInstructionType::IADD_C8:
		case randomx::SuperscalarInstructionType::IADD_C9:
			{
				int32_t imm = unsigned32ToSigned2sCompl(isn.getImm32());
				 
				emitImm32(buf, imm, regSS(isn.dst), regSS(isn.dst), SshTmp1Reg);
			}
			break;
		case randomx::SuperscalarInstructionType::IXOR_C7:
		case randomx::SuperscalarInstructionType::IXOR_C8:
		case randomx::SuperscalarInstructionType::IXOR_C9:
			{
				int32_t imm = unsigned32ToSigned2sCompl(isn.getImm32());
				 
				emitImm32(buf, imm, SshTmp1Reg);
				 
				buf.emit(rvi(rv64::XOR, regSS(isn.dst), regSS(isn.dst), SshTmp1Reg));
			}
			break;
		case randomx::SuperscalarInstructionType::IMULH_R:
			 
			buf.emit(rvi(rv64::MULHU, regSS(isn.dst), regSS(isn.dst), regSS(isn.src)));
			break;
		case randomx::SuperscalarInstructionType::ISMULH_R:
			 
			buf.emit(rvi(rv64::MULH, regSS(isn.dst), regSS(isn.dst), regSS(isn.src)));
			break;
		case randomx::SuperscalarInstructionType::IMUL_RCP:
			 
			buf.emit(rvi(rv64::MUL, regSS(isn.dst), regSS(isn.dst), SshRcpReg));
			 
			emitRcpLiteral2(buf, reciprocalCache[isn.getImm32()], reciprocalCache.size());
			break;
		default:
			UNREACHABLE;
		}
	}

	size_t JitCompilerRV64::getCodeSize() {
		return CodeSize;
	}

	JitCompilerRV64::JitCompilerRV64() {
		state.code = (uint8_t*)allocMemoryPages(CodeSize);
		if (state.code == nullptr)
			throw std::runtime_error("allocMemoryPages");
		state.emitAt(LiteralPoolOffset, codeLiterals, sizeLiterals);
		state.emitAt(LiteralPoolSize, codeDataInit, sizeDataInit + sizePrologue + sizeLoopBegin);
		entryDataInit = state.code + LiteralPoolSize;
		entryProgram = state.code + LiteralPoolSize + sizeDataInit;
		 
		emitJump(state, ReturnReg, LiteralPoolSize + offsetFixDataCall, SuperScalarHashOffset);
	}

	JitCompilerRV64::~JitCompilerRV64() {
		freePagedMemory(state.code, CodeSize);
	}

	void JitCompilerRV64::enableAll() {
		setPagesRWX(entryDataInit, ExecutableSize);
	}

	void JitCompilerRV64::enableWriting() {
		setPagesRW(entryDataInit, ExecutableSize);
	}

	void JitCompilerRV64::enableExecution() {
		setPagesRX(entryDataInit, ExecutableSize);
	}

	void JitCompilerRV64::generateProgram(Program& prog, ProgramConfiguration& pcfg) {
		emitProgramPrefix(state, prog, pcfg);
		int32_t fixPos = state.codePos;
		state.emit(codeDataRead, sizeDataRead);
		 
		state.emitAt(fixPos, rvi(rv64::XOR, Tmp1Reg, regR(pcfg.readReg2), regR(pcfg.readReg3)));
		emitProgramSuffix(state, pcfg);
		clearCache(state);
	}

	void JitCompilerRV64::generateProgramLight(Program& prog, ProgramConfiguration& pcfg, uint32_t datasetOffset) {
		emitProgramPrefix(state, prog, pcfg);
		int32_t fixPos = state.codePos;
		state.emit(codeDataReadLight, sizeDataReadLight);
		 
		state.emitAt(fixPos, rvi(rv64::XOR, Tmp1Reg, regR(pcfg.readReg2), regR(pcfg.readReg3)));
		int32_t imm = datasetOffset / CacheLineSize;
		int32_t limm = (imm << 20) >> 20;
		int32_t uimm = (imm >> 12) + (limm < 0);
		 
		state.emitAt(fixPos + 4, rv64::LUI | (uimm << 12) | rvrd(Tmp2Reg));
		 
		state.emitAt(fixPos + 8, rvi(rv64::ADDI, Tmp2Reg, Tmp2Reg, limm));
		fixPos += offsetFixLoopCall;
		 
		emitJump(state, ReturnReg, fixPos, SuperScalarHashOffset);
		emitProgramSuffix(state, pcfg);
		clearCache(state);
	}

	void JitCompilerRV64::generateSuperscalarHash(SuperscalarProgram programs[RANDOMX_CACHE_ACCESSES], std::vector<uint64_t>& reciprocalCache) {
		state.codePos = SuperScalarHashOffset;
		state.rcpCount = 0;
		state.emit(codeSshInit, sizeSshInit);
		for (unsigned j = 0; j < RANDOMX_CACHE_ACCESSES; ++j) {
			SuperscalarProgram& prog = programs[j];
			for (unsigned i = 0; i < prog.getSize(); ++i) {
				Instruction instr = prog(i);
				generateSuperscalarCode(state, instr, reciprocalCache);
			}
			state.emit(codeSshLoad, sizeSshLoad);
			if (j < RANDOMX_CACHE_ACCESSES - 1) {
				int32_t fixPos = state.codePos;
				state.emit(codeSshPrefetch, sizeSshPrefetch);
				 
				state.emitAt(fixPos, rvi(rv64::AND, SuperscalarReg, regSS(prog.getAddressRegister()), SuperscalarReg));
			}
		}
		state.emit(rvc(rv64::C_RET, 0, 0));
		clearCache(state);
	}

	static void v1_IADD_RS(HANDLER_ARGS) {
		state.registerUsage[isn.dst] = i;
		int shift = isn.getModShift();
		if (shift == 0) {
			 
			state.emit(rvc(rv64::C_ADD, regR(isn.dst), regR(isn.src)));
		}
		else {
#ifdef __riscv_zba
			 
			state.emit(rv64::SHXADD | rvrs2(regR(isn.dst)) | rvrs1(regR(isn.src)) | (shift << 13) | rvrd(regR(isn.dst)));
#else
			 
			state.emit(rvi(rv64::SLLI, Tmp1Reg, regR(isn.src), shift));
			 
			state.emit(rvc(rv64::C_ADD, regR(isn.dst), Tmp1Reg));
#endif
		}
		if (isn.dst == RegisterNeedsDisplacement) {
			int32_t imm = unsigned32ToSigned2sCompl(isn.getImm32());
			 
			emitImm32(state, imm, regR(isn.dst), regR(isn.dst), Tmp1Reg);
		}
	}

	static void v1_IADD_M(HANDLER_ARGS) {
		state.registerUsage[isn.dst] = i;
		loadFromScratchpad(state, isn);
		 
		state.emit(rvc(rv64::C_ADD, regR(isn.dst), Tmp1Reg));
	}

	static void v1_ISUB_R(HANDLER_ARGS) {
		state.registerUsage[isn.dst] = i;
		if (isn.src != isn.dst) {
			 
			state.emit(rvi(rv64::SUB, regR(isn.dst), regR(isn.dst), regR(isn.src)));
		}
		else {
			int32_t imm = unsigned32ToSigned2sCompl(-isn.getImm32());  
			 
			emitImm32(state, imm, regR(isn.dst), regR(isn.dst), Tmp1Reg);
		}
	}

	static void v1_ISUB_M(HANDLER_ARGS) {
		state.registerUsage[isn.dst] = i;
		loadFromScratchpad(state, isn);
		 
		state.emit(rvi(rv64::SUB, regR(isn.dst), regR(isn.dst), Tmp1Reg));
	}

	static void v1_IMUL_R(HANDLER_ARGS) {
		state.registerUsage[isn.dst] = i;
		if (isn.src != isn.dst) {
			 
			state.emit(rvi(rv64::MUL, regR(isn.dst), regR(isn.dst), regR(isn.src)));
		}
		else {
			int32_t imm = unsigned32ToSigned2sCompl(isn.getImm32());
			 
			emitImm32(state, imm, Tmp1Reg);
			 
			state.emit(rvi(rv64::MUL, regR(isn.dst), regR(isn.dst), Tmp1Reg));
		}
	}

	static void v1_IMUL_M(HANDLER_ARGS) {
		state.registerUsage[isn.dst] = i;
		loadFromScratchpad(state, isn);
		 
		state.emit(rvi(rv64::MUL, regR(isn.dst), regR(isn.dst), Tmp1Reg));
	}

	static void v1_IMULH_R(HANDLER_ARGS) {
		state.registerUsage[isn.dst] = i;
		 
		state.emit(rvi(rv64::MULHU, regR(isn.dst), regR(isn.dst), regR(isn.src)));
	}

	static void v1_IMULH_M(HANDLER_ARGS) {
		state.registerUsage[isn.dst] = i;
		loadFromScratchpad(state, isn);
		 
		state.emit(rvi(rv64::MULHU, regR(isn.dst), regR(isn.dst), Tmp1Reg));
	}

	static void v1_ISMULH_R(HANDLER_ARGS) {
		state.registerUsage[isn.dst] = i;
		 
		state.emit(rvi(rv64::MULH, regR(isn.dst), regR(isn.dst), regR(isn.src)));
	}

	static void v1_ISMULH_M(HANDLER_ARGS) {
		state.registerUsage[isn.dst] = i;
		loadFromScratchpad(state, isn);
		 
		state.emit(rvi(rv64::MULH, regR(isn.dst), regR(isn.dst), Tmp1Reg));
	}

	static void v1_IMUL_RCP(HANDLER_ARGS) {
		const uint32_t divisor = isn.getImm32();
		if (!isZeroOrPowerOf2(divisor)) {
			state.registerUsage[isn.dst] = i;
			if (state.rcpCount < 4) {
				 
				state.emit(rvi(rv64::MUL, regR(isn.dst), regR(isn.dst), regRcp(state.rcpCount)));
			}
			else if (state.rcpCount < 10) {
				 
				state.emit(rvi(rv64::FMV_X_D, Tmp1Reg, regRcpF(state.rcpCount)));
				 
				state.emit(rvi(rv64::MUL, regR(isn.dst), regR(isn.dst), Tmp1Reg));
			}
			else {
				int32_t offset = RcpLiteralsOffset + state.rcpCount * 8;
				 
				state.emit(rvi(rv64::LD, Tmp1Reg, LiteralPoolReg, offset));
				 
				state.emit(rvi(rv64::MUL, regR(isn.dst), regR(isn.dst), Tmp1Reg));
			}
			emitRcpLiteral1(state, randomx_reciprocal_fast(divisor));
		}
	}

	static void v1_INEG_R(HANDLER_ARGS) {
		state.registerUsage[isn.dst] = i;
		 
		state.emit(rvi(rv64::SUB, regR(isn.dst), 0, regR(isn.dst)));
	}

	static void v1_IXOR_R(HANDLER_ARGS) {
		state.registerUsage[isn.dst] = i;
		if (isn.src != isn.dst) {
			 
			state.emit(rvi(rv64::XOR, regR(isn.dst), regR(isn.dst), regR(isn.src)));
		}
		else {
			int32_t imm = unsigned32ToSigned2sCompl(isn.getImm32());
			 
			emitImm32(state, imm, Tmp1Reg);
			 
			state.emit(rvi(rv64::XOR, regR(isn.dst), regR(isn.dst), Tmp1Reg));
		}
	}

	static void v1_IXOR_M(HANDLER_ARGS) {
		state.registerUsage[isn.dst] = i;
		loadFromScratchpad(state, isn);
		 
		state.emit(rvi(rv64::XOR, regR(isn.dst), regR(isn.dst), Tmp1Reg));
	}

	static void v1_IROR_R(HANDLER_ARGS) {
		state.registerUsage[isn.dst] = i;
#ifdef __riscv_zbb
		if (isn.src != isn.dst) {
			 
			state.emit(rvi(rv64::ROR, regR(isn.dst), regR(isn.dst), regR(isn.src)));
		}
		else {
			int32_t imm = isn.getImm32() & 63;
			 
			state.emit(rvi(rv64::RORI, regR(isn.dst), regR(isn.dst), imm));
		}
#else
		if (isn.src != isn.dst) {
			 
			state.emit(rvi(rv64::SUB, Tmp1Reg, 0, regR(isn.src)));
			 
			state.emit(rvi(rv64::SRL, Tmp2Reg, regR(isn.dst), regR(isn.src)));
			 
			state.emit(rvi(rv64::SLL, regR(isn.dst), regR(isn.dst), Tmp1Reg));
			 
			state.emit(rvi(rv64::OR, regR(isn.dst), regR(isn.dst), Tmp2Reg));
		}
		else {
			int32_t immr = isn.getImm32() & 63;
			int32_t imml = -immr & 63;
			int32_t imml5 = imml >> 5;
			int32_t imml40 = imml & 31;
			 
			state.emit(rvi(rv64::SRLI, Tmp1Reg, regR(isn.dst), immr));
			 
			state.emit(rvc(rv64::C_SLLI, imml5, regR(isn.dst), imml40));
			 
			state.emit(rvi(rv64::OR, regR(isn.dst), regR(isn.dst), Tmp1Reg));
		}
#endif
	}

	static void v1_IROL_R(HANDLER_ARGS) {
		state.registerUsage[isn.dst] = i;
#ifdef __riscv_zbb
		if (isn.src != isn.dst) {
			 
			state.emit(rvi(rv64::ROL, regR(isn.dst), regR(isn.dst), regR(isn.src)));
		}
		else {
			int32_t imm = -isn.getImm32() & 63;
			 
			state.emit(rvi(rv64::RORI, regR(isn.dst), regR(isn.dst), imm));
		}
#else
		if (isn.src != isn.dst) {
			 
			state.emit(rvi(rv64::SUB, Tmp1Reg, 0, regR(isn.src)));
			 
			state.emit(rvi(rv64::SLL, Tmp2Reg, regR(isn.dst), regR(isn.src)));
			 
			state.emit(rvi(rv64::SRL, regR(isn.dst), regR(isn.dst), Tmp1Reg));
			 
			state.emit(rvi(rv64::OR, regR(isn.dst), regR(isn.dst), Tmp2Reg));
		}
		else {
			int32_t imml = isn.getImm32() & 63;
			int32_t immr = -imml & 63;
			int32_t imml5 = imml >> 5;
			int32_t imml40 = imml & 31;
			 
			state.emit(rvi(rv64::SRLI, Tmp1Reg, regR(isn.dst), immr));
			 
			state.emit(rvc(rv64::C_SLLI, imml5, regR(isn.dst), imml40));
			 
			state.emit(rvi(rv64::OR, regR(isn.dst), regR(isn.dst), Tmp1Reg));
		}
#endif
	}

	static void v1_ISWAP_R(HANDLER_ARGS) {
		if (isn.src != isn.dst) {
			state.registerUsage[isn.dst] = i;
			state.registerUsage[isn.src] = i;
			 
			state.emit(rvc(rv64::C_MV, Tmp1Reg, regR(isn.dst)));
			 
			state.emit(rvc(rv64::C_MV, regR(isn.dst), regR(isn.src)));
			 
			state.emit(rvc(rv64::C_MV, regR(isn.src), Tmp1Reg));
		}
	}

	static void v1_FSWAP_R(HANDLER_ARGS) {
		 
		state.emit(rvi(rv64::FMV_D, Tmp1RegF, regLoF(isn.dst), regLoF(isn.dst)));
		 
		state.emit(rvi(rv64::FMV_D, regLoF(isn.dst), regHiF(isn.dst), regHiF(isn.dst)));
		 
		state.emit(rvi(rv64::FMV_D, regHiF(isn.dst), Tmp1RegF, Tmp1RegF));
	}

	static void v1_FADD_R(HANDLER_ARGS) {
		isn.dst %= RegisterCountFlt;
		isn.src %= RegisterCountFlt;
		 
		state.emit(rvi(rv64::FADD_D, regLoF(isn.dst), regLoF(isn.dst), regLoA(isn.src)));
		 
		state.emit(rvi(rv64::FADD_D, regHiF(isn.dst), regHiF(isn.dst), regHiA(isn.src)));
	}

	static void v1_FADD_M(HANDLER_ARGS) {
		isn.dst %= RegisterCountFlt;
		 
		genAddressReg(state, isn);
		 
		state.emit(rvc(rv64::C_LW, Tmp2Reg + OffsetXC, Tmp1Reg + OffsetXC));
		 
		state.emit(rvc(rv64::C_LW, Tmp2Reg + OffsetXC, 16 + Tmp2Reg + OffsetXC));
		 
		state.emit(rvi(rv64::FCVT_D_W, Tmp1RegF, Tmp1Reg));
		 
		state.emit(rvi(rv64::FCVT_D_W, Tmp2RegF, Tmp2Reg));
		 
		state.emit(rvi(rv64::FADD_D, regLoF(isn.dst), regLoF(isn.dst), Tmp1RegF));
		 
		state.emit(rvi(rv64::FADD_D, regHiF(isn.dst), regHiF(isn.dst), Tmp2RegF));
	}

	static void v1_FSUB_R(HANDLER_ARGS) {
		isn.dst %= RegisterCountFlt;
		isn.src %= RegisterCountFlt;
		 
		state.emit(rvi(rv64::FSUB_D, regLoF(isn.dst), regLoF(isn.dst), regLoA(isn.src)));
		 
		state.emit(rvi(rv64::FSUB_D, regHiF(isn.dst), regHiF(isn.dst), regHiA(isn.src)));
	}

	static void v1_FSUB_M(HANDLER_ARGS) {
		isn.dst %= RegisterCountFlt;
		 
		genAddressReg(state, isn);
		 
		state.emit(rvc(rv64::C_LW, Tmp2Reg + OffsetXC, Tmp1Reg + OffsetXC));
		 
		state.emit(rvc(rv64::C_LW, Tmp2Reg + OffsetXC, 16 + Tmp2Reg + OffsetXC));
		 
		state.emit(rvi(rv64::FCVT_D_W, Tmp1RegF, Tmp1Reg));
		 
		state.emit(rvi(rv64::FCVT_D_W, Tmp2RegF, Tmp2Reg));
		 
		state.emit(rvi(rv64::FSUB_D, regLoF(isn.dst), regLoF(isn.dst), Tmp1RegF));
		 
		state.emit(rvi(rv64::FSUB_D, regHiF(isn.dst), regHiF(isn.dst), Tmp2RegF));
	}

	static void v1_FSCAL_R(HANDLER_ARGS) {
		isn.dst %= RegisterCountFlt;
		 
		state.emit(rvi(rv64::FMV_X_D, Tmp1Reg, regLoF(isn.dst)));
		 
		state.emit(rvi(rv64::FMV_X_D, Tmp2Reg, regHiF(isn.dst)));
		 
		state.emit(rvc(rv64::C_XOR, Tmp1Reg + OffsetXC, MaskFscalReg + OffsetXC));
		 
		state.emit(rvc(rv64::C_XOR, Tmp2Reg + OffsetXC, MaskFscalReg + OffsetXC));
		 
		state.emit(rvi(rv64::FMV_D_X, regLoF(isn.dst), Tmp1Reg));
		 
		state.emit(rvi(rv64::FMV_D_X, regHiF(isn.dst), Tmp2Reg));
	}

	static void v1_FMUL_R(HANDLER_ARGS) {
		isn.dst %= RegisterCountFlt;
		isn.src %= RegisterCountFlt;
		 
		state.emit(rvi(rv64::FMUL_D, regLoE(isn.dst), regLoE(isn.dst), regLoA(isn.src)));
		 
		state.emit(rvi(rv64::FMUL_D, regHiE(isn.dst), regHiE(isn.dst), regHiA(isn.src)));
	}

	static void v1_FDIV_M(HANDLER_ARGS) {
		isn.dst %= RegisterCountFlt;
		 
		genAddressReg(state, isn);
		 
		state.emit(rvc(rv64::C_LW, Tmp2Reg + OffsetXC, Tmp1Reg + OffsetXC));
		 
		state.emit(rvc(rv64::C_LW, Tmp2Reg + OffsetXC, 16 + Tmp2Reg + OffsetXC));
		 
		state.emit(rvi(rv64::FCVT_D_W, Tmp1RegF, Tmp1Reg));
		 
		state.emit(rvi(rv64::FCVT_D_W, Tmp2RegF, Tmp2Reg));
		 
		state.emit(rvi(rv64::FMV_X_D, Tmp1Reg, Tmp1RegF));
		 
		state.emit(rvi(rv64::FMV_X_D, Tmp2Reg, Tmp2RegF));
		 
		state.emit(rvc(rv64::C_AND, Tmp1Reg + OffsetXC, MaskEclear + OffsetXC));
		 
		state.emit(rvc(rv64::C_AND, Tmp2Reg + OffsetXC, MaskEclear + OffsetXC));
		 
		state.emit(rvc(rv64::C_OR, Tmp1Reg + OffsetXC, MaskEsetLo + OffsetXC));
		 
		state.emit(rvc(rv64::C_OR, Tmp2Reg + OffsetXC, MaskEsetHi + OffsetXC));
		 
		state.emit(rvi(rv64::FMV_D_X, Tmp1RegF, Tmp1Reg));
		 
		state.emit(rvi(rv64::FMV_D_X, Tmp2RegF, Tmp2Reg));
		 
		state.emit(rvi(rv64::FDIV_D, regLoE(isn.dst), regLoE(isn.dst), Tmp1RegF));
		 
		state.emit(rvi(rv64::FDIV_D, regHiE(isn.dst), regHiE(isn.dst), Tmp2RegF));
	}

	static void v1_FSQRT_R(HANDLER_ARGS) {
		isn.dst %= RegisterCountFlt;
		 
		state.emit(rvi(rv64::FSQRT_D, regLoE(isn.dst), regLoE(isn.dst)));
		 
		state.emit(rvi(rv64::FSQRT_D, regHiE(isn.dst), regHiE(isn.dst)));
	}

	static void v1_CBRANCH(HANDLER_ARGS) {
		int reg = isn.dst;
		int target = state.registerUsage[reg] + 1;
		int shift = isn.getModCond() + ConditionOffset;
		int32_t imm = unsigned32ToSigned2sCompl(isn.getImm32());
		imm |= (1UL << shift);
		if (ConditionOffset > 0 || shift > 0)
			imm &= ~(1UL << (shift - 1));
		 
		emitImm32(state, (int32_t)ConditionMask << shift, Tmp1Reg);
		 
		emitImm32(state, imm, regR(isn.dst), regR(isn.dst), Tmp2Reg);
		 
		state.emit(rvi(rv64::AND, Tmp1Reg, Tmp1Reg, regR(isn.dst)));
		int32_t targetPos = state.instructionOffsets[target];
		int offset = targetPos - state.codePos;
		if (offset >= -256) {  
			 
			int imm8 = 1;  
			int imm21 = offset & 6;  
			int imm5 = (offset >> 5) & 1;  
			int imm43 = offset & 24;  
			int imm76 = (offset >> 3) & 24;  
			state.emit(rvc(rv64::C_BEQZ, imm8, imm43 + (Tmp1Reg + OffsetXC), imm76 + imm21 + imm5));
		}
		else if (offset >= -4096) {  
			 
			int imm12 = 1 << 11;  
			int imm105 = offset & 2016;  
			int imm41 = offset & 30;  
			int imm11 = (offset >> 11) & 1;  
			state.emit(rvi(rv64::BEQ, imm41 + imm11, Tmp1Reg, imm12 + imm105));
		}
		else {
			 
			state.emit(rvc(rv64::C_BNEZ, Tmp1Reg + OffsetXC, 6));
			 
			emitJump(state, 0, state.codePos, targetPos);
			state.codePos += 4;
		}
		 
		for (unsigned j = 0; j < RegistersCount; ++j) {
			state.registerUsage[j] = i;
		}
	}

	static void v1_CFROUND(HANDLER_ARGS) {
		int32_t imm = (isn.getImm32() - 2) & 63;  
		if (imm != 0) {
#ifdef __riscv_zbb
			 
			state.emit(rvi(rv64::RORI, Tmp1Reg, regR(isn.src), imm));
#else
			int32_t imml = -imm & 63;
			 
			state.emit(rvi(rv64::SRLI, Tmp1Reg, regR(isn.src), imm));
			 
			state.emit(rvi(rv64::SLLI, Tmp2Reg, regR(isn.src), imml));
			 
			state.emit(rvc(rv64::C_OR, Tmp1Reg + OffsetXC, Tmp2Reg + OffsetXC));
#endif
			 
			state.emit(rvc(rv64::C_ANDI, Tmp1Reg + OffsetXC, 12));
		}
		else {
			 
			state.emit(rvi(rv64::ANDI, Tmp1Reg, regR(isn.src), 12));
		}
		 
		state.emit(rvc(rv64::C_ADD, Tmp1Reg, LiteralPoolReg));
		 
		state.emit(rvc(rv64::C_LW, Tmp1Reg + OffsetXC, 8 + Tmp1Reg + OffsetXC));
		 
		state.emit(rvi(rv64::FSRM, 0, Tmp1Reg, 0));
	}

	static void v1_ISTORE(HANDLER_ARGS) {
		genAddressRegDst(state, isn);
		 
		state.emit(rvi(rv64::SD, 0, Tmp2Reg, regR(isn.src)));
	}

	static void v1_NOP(HANDLER_ARGS) {
	}
}

#include "instruction_weights.hpp"

namespace {

#define INST_HANDLE1(x) REPN(&randomx::v1_##x, WT(x))
#define INST_HANDLE2(x) REPN(&randomx::v2_##x, WT(x))

	InstructionHandler* opcodeMap1[256] = {
		INST_HANDLE1(IADD_RS)
		INST_HANDLE1(IADD_M)
		INST_HANDLE1(ISUB_R)
		INST_HANDLE1(ISUB_M)
		INST_HANDLE1(IMUL_R)
		INST_HANDLE1(IMUL_M)
		INST_HANDLE1(IMULH_R)
		INST_HANDLE1(IMULH_M)
		INST_HANDLE1(ISMULH_R)
		INST_HANDLE1(ISMULH_M)
		INST_HANDLE1(IMUL_RCP)
		INST_HANDLE1(INEG_R)
		INST_HANDLE1(IXOR_R)
		INST_HANDLE1(IXOR_M)
		INST_HANDLE1(IROR_R)
		INST_HANDLE1(IROL_R)
		INST_HANDLE1(ISWAP_R)
		INST_HANDLE1(FSWAP_R)
		INST_HANDLE1(FADD_R)
		INST_HANDLE1(FADD_M)
		INST_HANDLE1(FSUB_R)
		INST_HANDLE1(FSUB_M)
		INST_HANDLE1(FSCAL_R)
		INST_HANDLE1(FMUL_R)
		INST_HANDLE1(FDIV_M)
		INST_HANDLE1(FSQRT_R)
		INST_HANDLE1(CBRANCH)
		INST_HANDLE1(CFROUND)
		INST_HANDLE1(ISTORE)
		INST_HANDLE1(NOP)
	};
}