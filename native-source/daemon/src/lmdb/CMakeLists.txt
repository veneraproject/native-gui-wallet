                                      
                                             
  
                      
  
                                                                                      
                                                           
  
                                                                                        
                                             
  
                                                                                        
                                                                                 
                                              
  
                                                                                      
                                                                                    
                              
  
                                                                                     
                                                                                         
                                                                                        
                                                                                      
                                                                              
                                                                                         
                                                                                   
                                                                                         
                                                                              

set(lmdb_sources database.cpp error.cpp table.cpp value_stream.cpp)
monero_find_all_headers(lmdb_headers "${CMAKE_CURRENT_SOURCE_DIR}")

monero_add_library(lmdb_lib ${lmdb_sources} ${lmdb_headers})
target_link_libraries(lmdb_lib common ${LMDB_LIBRARY})
