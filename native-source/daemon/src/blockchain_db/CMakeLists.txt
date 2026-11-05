                                      
                                             
 
                      
 
                                                                                      
                                                           
 
                                                                                        
                                             
 
                                                                                        
                                                                                 
                                              
 
                                                                                      
                                                                                    
                              
 
                                                                                     
                                                                                         
                                                                                        
                                                                                      
                                                                              
                                                                                         
                                                                                   
                                                                                         
                                                                              

set(blockchain_db_sources
  blockchain_db.cpp
  lmdb/db_lmdb.cpp
  )

set(blockchain_db_headers)

monero_find_all_headers(blockchain_db_private_headers "${CMAKE_CURRENT_SOURCE_DIR}")

monero_private_headers(blockchain_db
  ${crypto_private_headers})
monero_add_library(blockchain_db
  ${blockchain_db_sources}
  ${blockchain_db_headers}
  ${blockchain_db_private_headers})
target_link_libraries(blockchain_db
  PUBLIC
    common
    cncrypto
    ringct
    ${LMDB_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
  PRIVATE
    ${EXTRA_LIBRARIES})
