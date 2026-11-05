                                      
                                             

 
                      
 
                                                                                      
                                                           
 
                                                                                        
                                             
 
                                                                                        
                                                                                 
                                              
 
                                                                                      
                                                                                    
                              
 
                                                                                     
                                                                                         
                                                                                        
                                                                                      
                                                                              
                                                                                         
                                                                                   
                                                                                         
                                                                              

 
 
 
set(MONERO_WALLET_CRYPTO_LIBRARY "auto" CACHE STRING "Select a wallet crypto library")

 
 
 
if (${MONERO_WALLET_CRYPTO_LIBRARY} STREQUAL "auto")
   monero_crypto_autodetect(AVAILABLE BEST)
  if (DEFINED BEST)
    message("Wallet crypto is using ${BEST} backend")
    set(MONERO_WALLET_CRYPTO_LIBRARY ${BEST})
  else ()
    message("Defaulting to internal crypto library for wallet")
    set(MONERO_WALLET_CRYPTO_LIBRARY "cn")
  endif ()
endif ()

 
 
 
 
if (${MONERO_WALLET_CRYPTO_LIBRARY} STREQUAL "cn")
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/empty.h.in ${MONERO_GENERATED_HEADERS_DIR}/crypto/wallet/ops.h)
  add_library(wallet-crypto ALIAS cncrypto)
else ()
  monero_crypto_generate_header(${MONERO_WALLET_CRYPTO_LIBRARY} "${MONERO_GENERATED_HEADERS_DIR}/crypto/wallet/ops.h")
  monero_crypto_get_target(${MONERO_WALLET_CRYPTO_LIBRARY} CRYPTO_TARGET)
  add_library(wallet-crypto $<TARGET_OBJECTS:${CRYPTO_TARGET}>)
  target_link_libraries(wallet-crypto cncrypto)
endif ()


