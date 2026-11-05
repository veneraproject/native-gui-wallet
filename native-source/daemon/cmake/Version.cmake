                                      
                                             
  
                      
  
                                                                                      
                                                           
  
                                                                                        
                                             
  
                                                                                        
                                                                                 
                                              
  
                                                                                      
                                                                                    
                              
  
                                                                                     
                                                                                         
                                                                                        
                                                                                      
                                                                              
                                                                                         
                                                                                   
                                                                                         
                                                                              

function (write_version tag)
  set(VERSIONTAG "${tag}" CACHE STRING "The tag portion of the Monero software version" FORCE)
  configure_file("${CMAKE_CURRENT_LIST_DIR}/../src/version.cpp.in" "${CMAKE_BINARY_DIR}/version.cpp")
endfunction ()

set(VERSION_IS_RELEASE "false")
write_version("local-sync-1854-no-mining")
add_custom_target(genversion ALL
  DEPENDS "${CMAKE_BINARY_DIR}/version.cpp")
