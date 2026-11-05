  
                                                    

                    

                                                                  
                                                                           
                                                                 
                                                                
                                                                    
                                                                      
                                                                       
                                                   
                                                                       
                                                                        

                                                                               
                                                                             
                                                                      
                                                                            
                                                                          
                                                                          
                                                                          
                                                                             
                                                                             
                                                                    
  

#pragma once

#include <cstddef>

template<bool softAes>
void hashAes1Rx4(const void *input, size_t inputSize, void *hash);

template<bool softAes>
void fillAes1Rx4(void *state, size_t outputSize, void *buffer);

template<bool softAes>
void fillAes4Rx4(void *state, size_t outputSize, void *buffer);

template<bool softAes>
void hashAndFillAes1Rx4(void *scratchpad, size_t scratchpadSize, void *hash, void* fill_state);
