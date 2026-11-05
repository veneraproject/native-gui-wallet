  
                                                    

                    

                                                                  
                                                                           
                                                                 
                                                                
                                                                    
                                                                      
                                                                       
                                                   
                                                                       
                                                                        

                                                                               
                                                                             
                                                                      
                                                                            
                                                                          
                                                                          
                                                                          
                                                                             
                                                                             
                                                                    
  

#pragma once

 
#define RANDOMX_ARGON_MEMORY       262144

 
#define RANDOMX_ARGON_ITERATIONS   3

 
#define RANDOMX_ARGON_LANES        1

 
#define RANDOMX_ARGON_SALT         "RandomX\x03"

 
#define RANDOMX_CACHE_ACCESSES     8

 
#define RANDOMX_SUPERSCALAR_LATENCY   170

 
#define RANDOMX_DATASET_BASE_SIZE  2147483648

 
#define RANDOMX_DATASET_EXTRA_SIZE 33554368

 
#define RANDOMX_PROGRAM_SIZE       256

 
#define RANDOMX_PROGRAM_ITERATIONS 2048

 
#define RANDOMX_PROGRAM_COUNT      8

 
#define RANDOMX_SCRATCHPAD_L3      2097152

 
#define RANDOMX_SCRATCHPAD_L2      262144

 
#define RANDOMX_SCRATCHPAD_L1      16384

 
#define RANDOMX_JUMP_BITS          8

 
#define RANDOMX_JUMP_OFFSET        8

 
 
 
 

 
#define RANDOMX_FREQ_IADD_RS       16
#define RANDOMX_FREQ_IADD_M         7
#define RANDOMX_FREQ_ISUB_R        16
#define RANDOMX_FREQ_ISUB_M         7
#define RANDOMX_FREQ_IMUL_R        16
#define RANDOMX_FREQ_IMUL_M         4
#define RANDOMX_FREQ_IMULH_R        4
#define RANDOMX_FREQ_IMULH_M        1
#define RANDOMX_FREQ_ISMULH_R       4
#define RANDOMX_FREQ_ISMULH_M       1
#define RANDOMX_FREQ_IMUL_RCP       8
#define RANDOMX_FREQ_INEG_R         2
#define RANDOMX_FREQ_IXOR_R        15
#define RANDOMX_FREQ_IXOR_M         5
#define RANDOMX_FREQ_IROR_R         8
#define RANDOMX_FREQ_IROL_R         2
#define RANDOMX_FREQ_ISWAP_R        4

 
#define RANDOMX_FREQ_FSWAP_R        4
#define RANDOMX_FREQ_FADD_R        16
#define RANDOMX_FREQ_FADD_M         5
#define RANDOMX_FREQ_FSUB_R        16
#define RANDOMX_FREQ_FSUB_M         5
#define RANDOMX_FREQ_FSCAL_R        6
#define RANDOMX_FREQ_FMUL_R        32
#define RANDOMX_FREQ_FDIV_M         4
#define RANDOMX_FREQ_FSQRT_R        6

 
#define RANDOMX_FREQ_CBRANCH       25
#define RANDOMX_FREQ_CFROUND        1

 
#define RANDOMX_FREQ_ISTORE        16

 
#define RANDOMX_FREQ_NOP            0
 
 
 
