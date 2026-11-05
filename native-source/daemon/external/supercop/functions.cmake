                                      
                                        
 
                      
 
                                                                                      
                                                           
 
                                                                                        
                                             
 
                                                                                        
                                                                                 
                                              
 
                                                                                      
                                                                                    
                              
 
                                                                                     
                                                                                         
                                                                                        
                                                                                      
                                                                              
                                                                                         
                                                                                   
                                                                                         
                                                                              

set(MONERO_CRYPTO_DIR ${CMAKE_CURRENT_LIST_DIR})
add_subdirectory("${MONERO_CRYPTO_DIR}/src" ${CMAKE_CURRENT_BINARY_DIR}/monero_crypto_src)

 
function(monero_crypto_libraries OUT)
  set(${OUT} "amd64-64-24k" "amd64-51-30k" PARENT_SCOPE)
endfunction (monero_crypto_libraries)

 
function(monero_crypto_valid CRYPTO_LIBRARY OUT)
  monero_crypto_libraries(ALL_LIBRARIES)
  list(FIND ALL_LIBRARIES ${CRYPTO_LIBRARY} FOUND)
  if (FOUND LESS 0)
    set(${OUT} 0 PARENT_SCOPE)
  else ()
    set(${OUT} 1 PARENT_SCOPE)
  endif ()
endfunction (monero_crypto_valid)

 
function(monero_crypto_get_namespace CRYPTO_LIBRARY OUT)
  string(REPLACE "-" "_" TEMP "${CRYPTO_LIBRARY}")
  set(${OUT} ${TEMP} PARENT_SCOPE)
endfunction (monero_crypto_get_namespace)

 
function(monero_crypto_get_target CRYPTO_LIBRARY OUT)
  set(${OUT} "monero-crypto-${CRYPTO_LIBRARY}" PARENT_SCOPE)
endfunction (monero_crypto_get_target)

 
 
function(monero_crypto_autodetect AVAILABLE BEST)
  include(CheckLanguage)
  check_language(ASM-ATT)
  if (CMAKE_ASM-ATT_COMPILER AND UNIX AND (CMAKE_SYSTEM_PROCESSOR MATCHES "amd64.*|AMD64.*|x86_64.*"))
    set(${AVAILABLE} "amd64-64-24k" "amd64-51-30k" PARENT_SCOPE)
    set(${BEST} "amd64-64-24k" PARENT_SCOPE)
  else ()
    message("Monero crypto autodetect failed to find any libraries for target platform")
    unset(${AVAILABLE} PARENT_SCOPE)
    unset(${BEST} PARENT_SCOPE)
  endif ()
endfunction (monero_crypto_autodetect)

 
 
 
 
 
 
 
 
function(monero_crypto_generate_header MONERO_CRYPTO_LIBRARY HEADER_FILE)
  monero_crypto_valid(${MONERO_CRYPTO_LIBRARY} VALID)
  if (NOT VALID)
    monero_crypto_libraries(ALL_LIBRARIES)
    string(REPLACE ";" " " ALL_LIBRARIES "${ALL_LIBRARIES}")
    message(FATAL_ERROR "Library ${MONERO_CRYPTO_LIBRARY} is not valid. Must be one of: ${ALL_LIBRARIES}")
  endif ()
  monero_crypto_get_namespace(${MONERO_CRYPTO_LIBRARY} MONERO_CRYPTO_NAMESPACE)
  configure_file("${MONERO_CRYPTO_DIR}/src/crypto.h.in" ${HEADER_FILE})
endfunction (monero_crypto_generate_header)

