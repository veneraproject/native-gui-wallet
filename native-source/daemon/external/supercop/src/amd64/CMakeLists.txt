                                      
                                        
 
                      
 
                                                                                      
                                                           
 
                                                                                        
                                             
 
                                                                                        
                                                                                 
                                              
 
                                                                                      
                                                                                    
                              
 
                                                                                     
                                                                                         
                                                                                        
                                                                                      
                                                                              
                                                                                         
                                                                                   
                                                                                         
                                                                              

function(add_amd64_library LIBNAME LIBFOLDER)
  set(MULTIVARS SOURCES)
  cmake_parse_arguments(AMD64_PERF "" "" "${MULTIVARS}" ${ARGN})

  add_library("monero-crypto-${LIBNAME}" OBJECT
    "${LIBNAME}.c" "${LIBNAME}-choose_tp.s"
    "${LIBFOLDER}/choose_t.s"
    "${LIBFOLDER}/consts.s"
    "${LIBFOLDER}/fe25519_getparity.c"
    "${LIBFOLDER}/fe25519_freeze.s"
    "${LIBFOLDER}/fe25519_invert.c"
    "${LIBFOLDER}/fe25519_iseq.c"
    "${LIBFOLDER}/fe25519_mul.s"
    "${LIBFOLDER}/fe25519_neg.c"
    "${LIBFOLDER}/fe25519_pack.c"
    "${LIBFOLDER}/fe25519_pow2523.c"
    "${LIBFOLDER}/fe25519_setint.c"
    "${LIBFOLDER}/fe25519_square.s"
    "${LIBFOLDER}/fe25519_unpack.c"
    "${LIBFOLDER}/ge25519_add.c"
    "${LIBFOLDER}/ge25519_add_p1p1.s"
    "${LIBFOLDER}/ge25519_dbl_p1p1.s"
    "${LIBFOLDER}/ge25519_double.c"
    "${LIBFOLDER}/ge25519_nielsadd_p1p1.s"
    "${LIBFOLDER}/ge25519_nielsadd2.s"
    "${LIBFOLDER}/ge25519_pack.c"
    "${LIBFOLDER}/ge25519_p1p1_to_p2.s"
    "${LIBFOLDER}/ge25519_p1p1_to_p3.s"
    "${LIBFOLDER}/ge25519_pnielsadd_p1p1.s"
    "${LIBFOLDER}/ge25519_scalarmult_base.c"
    "${LIBFOLDER}/ge25519_unpackneg.c"
    "${LIBFOLDER}/sc25519_from32bytes.c"
    "${LIBFOLDER}/sc25519_window4.c"
    ${AMD64_PERF_SOURCES})
  target_include_directories("monero-crypto-${LIBNAME}" PRIVATE ${LIBFOLDER})
  set_source_files_properties("${LIBFOLDER}/consts.s" PROPERTIES COMPILE_FLAGS "-x assembler-with-cpp" LANGUAGE "C")
endfunction (add_amd64_library)

add_amd64_library(
  "amd64-64-24k"
  "${CMAKE_CURRENT_SOURCE_DIR}/../../crypto_sign/ed25519/amd64-64-24k"
  SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/../../crypto_sign/ed25519/amd64-64-24k/fe25519_add.s"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../crypto_sign/ed25519/amd64-64-24k/fe25519_sub.s")

add_amd64_library(
  "amd64-51-30k"
  "${CMAKE_CURRENT_SOURCE_DIR}/../../crypto_sign/ed25519/amd64-51-30k"
  SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/../../crypto_sign/ed25519/amd64-51-30k/fe25519_add.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../crypto_sign/ed25519/amd64-51-30k/fe25519_nsquare.s"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../crypto_sign/ed25519/amd64-51-30k/fe25519_sub.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../crypto_sign/ed25519/amd64-51-30k/ge25519_p1p1_to_pniels.s")
