  
                                                    

                    

                                                                  
                                                                           
                                                                 
                                                                
                                                                    
                                                                      
                                                                       
                                                   
                                                                       
                                                                        

                                                                               
                                                                             
                                                                      
                                                                            
                                                                          
                                                                          
                                                                          
                                                                             
                                                                             
                                                                    
  

#pragma once

#include <cstdint>
#include <vector>
#include "superscalar_program.hpp"
#include "blake2_generator.hpp"

namespace randomx {
	                                               
	enum class SuperscalarInstructionType {        
		ISUB_R = 0,                                
		IXOR_R = 1,                                
		IADD_RS = 2,                               
		IMUL_R = 3,                                
		IROR_C = 4,                                
		IADD_C7 = 5,                               
		IXOR_C7 = 6,                               
		IADD_C8 = 7,                               
		IXOR_C8 = 8,                               
		IADD_C9 = 9,                               
		IXOR_C9 = 10,                              
		IMULH_R = 11,                              
		ISMULH_R = 12,                             
		IMUL_RCP = 13,                             

		COUNT = 14,
		INVALID = -1
	};

	void generateSuperscalar(SuperscalarProgram& prog, Blake2Generator& gen);
	void executeSuperscalar(uint64_t(&r)[8], SuperscalarProgram& prog, std::vector<uint64_t> *reciprocals = nullptr);
}