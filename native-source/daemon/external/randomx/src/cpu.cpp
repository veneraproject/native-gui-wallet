  
                                               

                    

                                                                  
                                                                           
                                                                 
                                                                
                                                                    
                                                                      
                                                                       
                                                   
                                                                       
                                                                        

                                                                               
                                                                             
                                                                      
                                                                            
                                                                          
                                                                          
                                                                          
                                                                             
                                                                             
                                                                    
  

#include "cpu.hpp"

#if defined(_M_X64) || defined(__x86_64__)
	#define HAVE_CPUID
	#if defined(_MSC_VER)
		#include <intrin.h>
		#define cpuid(info, x) __cpuidex(info, x, 0)
	#else  
		#include <cpuid.h>
		void cpuid(int info[4], int InfoType) {
			__cpuid_count(InfoType, 0, info[0], info[1], info[2], info[3]);
		}
	#endif
#endif

#if defined(HAVE_HWCAP)
	#include <sys/auxv.h>
	#include <asm/hwcap.h>
#endif

namespace randomx {

	Cpu::Cpu() : aes_(false), ssse3_(false), avx2_(false) {
#ifdef HAVE_CPUID
		int info[4];
		cpuid(info, 0);
		int nIds = info[0];
		if (nIds >= 0x00000001) {
			cpuid(info, 0x00000001);
			ssse3_ = (info[2] & (1 << 9)) != 0;
			aes_ = (info[2] & (1 << 25)) != 0;
		}
		if (nIds >= 0x00000007) {
			cpuid(info, 0x00000007);
			avx2_ = (info[1] & (1 << 5)) != 0;
		}
#elif defined(__aarch64__)
	#if defined(HWCAP_AES)
		long hwcaps = getauxval(AT_HWCAP);
		aes_ = (hwcaps & HWCAP_AES) != 0;
	#elif defined(__APPLE__)
		aes_ = true;
	#endif
#endif
		 
	}

}
