                                      
                                             
  
                      
  
                                                                                      
                                                           
  
                                                                                        
                                             
  
                                                                                        
                                                                                 
                                              
  
                                                                                      
                                                                                    
                              
  
                                                                                     
                                                                                         
                                                                                        
                                                                                      
                                                                              
                                                                                         
                                                                                   
                                                                                         
                                                                              

 
 
message(STATUS "Using ${ARCH_WIDTH}-bit LMDB from source tree")
add_subdirectory(liblmdb)
set(LMDB_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/liblmdb" CACHE STRING "LMDB Include path")
set(LMDB_LIBRARY "lmdb" CACHE STRING "LMDB Library name")
if(CMAKE_SYSTEM_NAME MATCHES "NetBSD")
	set_property(TARGET lmdb APPEND_STRING PROPERTY COMPILE_FLAGS " -D_SEM_SEMUN_UNDEFINED")
elseif(CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
        set_property(TARGET lmdb APPEND_STRING PROPERTY COMPILE_FLAGS " -D_WANT_SEMUN")
endif()
