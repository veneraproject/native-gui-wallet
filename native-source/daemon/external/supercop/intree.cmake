                                      
                                        
 
                      
 
                                                                                      
                                                           
 
                                                                                        
                                             
 
                                                                                        
                                                                                 
                                              
 
                                                                                      
                                                                                    
                              
 
                                                                                     
                                                                                         
                                                                                        
                                                                                      
                                                                              
                                                                                         
                                                                                   
                                                                                         
                                                                              

include(functions.cmake)

set(MONERO_CRYPTO_LIBRARY "auto" CACHE STRING "Select a crypto library backend")

if (${MONERO_CRYPTO_LIBRARY} STREQUAL "auto")
  monero_crypto_autodetect(AVAILABLE BEST)
  if (NOT DEFINED BEST)
    message(FATAL_ERROR "No crypto library available for target platform")
  endif ()
  message("Using ${BEST} crypto backend")
  set(MONERO_CRYPTO_LIBRARY ${BEST})
endif ()

 
monero_crypto_generate_header(${MONERO_CRYPTO_LIBRARY} "${CMAKE_BINARY_DIR}/include/monero/crypto.h")

monero_crypto_get_target(${MONERO_CRYPTO_LIBRARY} CRYPTO_TARGET)
add_library(monero-crypto-intree ALIAS ${CRYPTO_TARGET})

