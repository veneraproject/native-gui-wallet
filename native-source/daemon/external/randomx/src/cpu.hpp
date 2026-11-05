  
                                               

                    

                                                                  
                                                                           
                                                                 
                                                                
                                                                    
                                                                      
                                                                       
                                                   
                                                                       
                                                                        

                                                                               
                                                                             
                                                                      
                                                                            
                                                                          
                                                                          
                                                                          
                                                                             
                                                                             
                                                                    
  

#pragma once

namespace randomx {

	class Cpu {
	public:
		Cpu();
		bool hasAes() const {
			return aes_;
		}
		bool hasSsse3() const {
			return ssse3_;
		}
		bool hasAvx2() const {
			return avx2_;
		}
	private:
		bool aes_, ssse3_, avx2_;
	};

}
