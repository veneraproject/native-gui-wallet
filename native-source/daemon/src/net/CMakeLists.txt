                                      
                                             

 
                      
 
                                                                                      
                                                           
 
                                                                                        
                                             
 
                                                                                        
                                                                                 
                                              
 
                                                                                      
                                                                                    
                              
 
                                                                                     
                                                                                         
                                                                                        
                                                                                      
                                                                              
                                                                                         
                                                                                   
                                                                                         
                                                                              

set(net_sources dandelionpp.cpp error.cpp http.cpp i2p_address.cpp parse.cpp resolve.cpp
    socks.cpp socks_connect.cpp tor_address.cpp zmq.cpp)
monero_find_all_headers(net_headers "${CMAKE_CURRENT_SOURCE_DIR}")

monero_add_library(net ${net_sources} ${net_headers})
target_link_libraries(net common epee ${ZMQ_LIB} ${Boost_ASIO_LIBRARY})

