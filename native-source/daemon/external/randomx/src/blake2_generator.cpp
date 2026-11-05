  
                                                    

                    

                                                                  
                                                                           
                                                                 
                                                                
                                                                    
                                                                      
                                                                       
                                                   
                                                                       
                                                                        

                                                                               
                                                                             
                                                                      
                                                                            
                                                                          
                                                                          
                                                                          
                                                                             
                                                                             
                                                                    
  

#include <stddef.h>
#include "blake2/blake2.h"
#include "blake2/endian.h"
#include "blake2_generator.hpp"

namespace randomx {

	constexpr int maxSeedSize = 60;

	Blake2Generator::Blake2Generator(const void* seed, size_t seedSize, int nonce) : dataIndex(sizeof(data)) {
		memset(data, 0, sizeof(data));
		memcpy(data, seed, seedSize > maxSeedSize ? maxSeedSize : seedSize);
		store32(&data[maxSeedSize], nonce);
	}

	uint8_t Blake2Generator::getByte() {
		checkData(1);
		return data[dataIndex++];
	}

	uint32_t Blake2Generator::getUInt32() {
		checkData(4);
		auto ret = load32(&data[dataIndex]);
		dataIndex += 4;
		return ret;
	}

	void Blake2Generator::checkData(const size_t bytesNeeded) {
		if (dataIndex + bytesNeeded > sizeof(data)) {
			blake2b(data, sizeof(data), data, sizeof(data), nullptr, 0);
			dataIndex = 0;
		}
	}
}