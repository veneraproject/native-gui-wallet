  
                                                    

                    

                                                                  
                                                                           
                                                                 
                                                                
                                                                    
                                                                      
                                                                       
                                                   
                                                                       
                                                                        

                                                                               
                                                                             
                                                                      
                                                                            
                                                                          
                                                                          
                                                                          
                                                                             
                                                                             
                                                                    
  

#pragma once

#include <stdint.h>
#include "intrin_portable.h"

rx_vec_i128 soft_aesenc(rx_vec_i128 in, rx_vec_i128 key);

rx_vec_i128 soft_aesdec(rx_vec_i128 in, rx_vec_i128 key);

template<bool soft>
inline rx_vec_i128 aesenc(rx_vec_i128 in, rx_vec_i128 key) {
	return soft ? soft_aesenc(in, key) : rx_aesenc_vec_i128(in, key);
}

template<bool soft>
inline rx_vec_i128 aesdec(rx_vec_i128 in, rx_vec_i128 key) {
	return soft ? soft_aesdec(in, key) : rx_aesdec_vec_i128(in, key);
}