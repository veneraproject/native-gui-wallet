                                      
                                        
 
                      
 
                                                                                      
                                                           
 
                                                                                        
                                             
 
                                                                                        
                                                                                 
                                              
 
                                                                                      
                                                                                    
                              
 
                                                                                     
                                                                                         
                                                                                        
                                                                                      
                                                                              
                                                                                         
                                                                                   
                                                                                         
                                                                              

cmake_minimum_required(VERSION 2.8.7)
project(monero-crypto)
enable_language(ASM C)
include(intree.cmake)

add_library(monero-crypto $<TARGET_OBJECTS:monero-crypto-intree>)

INSTALL(FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/monero/crypto/${MONERO_CRYPTO_LIBRARY}.h" DESTINATION "include/monero/crypto")
INSTALL(FILES "${CMAKE_BINARY_DIR}/include/monero/crypto.h" DESTINATION "include/monero")
INSTALL(TARGETS monero-crypto LIBRARY)
