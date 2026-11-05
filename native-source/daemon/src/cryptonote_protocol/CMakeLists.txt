                                      
                                             
 
                      
 
                                                                                      
                                                           
 
                                                                                        
                                             
 
                                                                                        
                                                                                 
                                              
 
                                                                                      
                                                                                    
                              
 
                                                                                     
                                                                                         
                                                                                        
                                                                                      
                                                                              
                                                                                         
                                                                                   
                                                                                         
                                                                              

cmake_minimum_required (VERSION 3.5)
project (monero CXX)

file(GLOB CRYPTONOTE_PROTOCOL *)
source_group(cryptonote_protocol FILES ${CRYPTONOTE_PROTOCOL})

 
monero_add_library(cryptonote_protocol ${CRYPTONOTE_PROTOCOL})
target_link_libraries(cryptonote_protocol
  PUBLIC
    p2p
  PRIVATE
    ${EXTRA_LIBRARIES})
