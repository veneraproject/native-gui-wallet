                                      
                                             
 
                      
 
                                                                                      
                                                           
 
                                                                                        
                                             
 
                                                                                        
                                                                                 
                                              
 
                                                                                      
                                                                                    
                              
 
                                                                                     
                                                                                         
                                                                                        
                                                                                      
                                                                              
                                                                                         
                                                                                   
                                                                                         
                                                                              

set(multisig_sources
  multisig.cpp
  multisig_account.cpp
  multisig_account_kex_impl.cpp
  multisig_clsag_context.cpp
  multisig_kex_msg.cpp
  multisig_tx_builder_ringct.cpp)

set(multisig_headers)

monero_find_all_headers(multisig_private_headers "${CMAKE_CURRENT_SOURCE_DIR}")

monero_private_headers(multisig
  ${multisig_private_headers})

monero_add_library(multisig
  ${multisig_sources}
  ${multisig_headers}
  ${multisig_private_headers})

target_link_libraries(multisig
  PUBLIC
    ringct
    cryptonote_basic
    cryptonote_core
    common
    cncrypto
  PRIVATE
    ${EXTRA_LIBRARIES})
