                                      
                                             
 
                      
 
                                                                                      
                                                           
 
                                                                                        
                                             
 
                                                                                        
                                                                                 
                                              
 
                                                                                      
                                                                                    
                              
 
                                                                                     
                                                                                         
                                                                                        
                                                                                      
                                                                              
                                                                                         
                                                                                   
                                                                                         
                                                                              

cmake_minimum_required (VERSION 3.5)
project (monero CXX)

file(GLOB P2P *)
source_group(p2p FILES ${P2P})

 

 
monero_add_library(p2p ${P2P})
target_link_libraries(p2p
  PUBLIC
    version
    cryptonote_core
    net
    ${UPNP_LIBRARIES}
    ${Boost_CHRONO_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${Boost_SERIALIZATION_LIBRARY}
  PRIVATE
    ${EXTRA_LIBRARIES})
