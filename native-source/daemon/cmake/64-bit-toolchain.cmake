                                      
                                             
  
                      
  
                                                                                      
                                                           
  
                                                                                        
                                             
  
                                                                                        
                                                                                 
                                              
  
                                                                                      
                                                                                    
                              
  
                                                                                     
                                                                                         
                                                                                        
                                                                                      
                                                                              
                                                                                         
                                                                                   
                                                                                         
                                                                              

if (NOT CMAKE_HOST_WIN32)
  set (CMAKE_SYSTEM_NAME Windows)
endif()

set (GCC_PREFIX x86_64-w64-mingw32)
set (CMAKE_C_COMPILER ${GCC_PREFIX}-gcc)
set (CMAKE_CXX_COMPILER ${GCC_PREFIX}-g++)
set (CMAKE_AR ar CACHE FILEPATH "" FORCE)
set (CMAKE_NM nm CACHE FILEPATH "" FORCE)
set (CMAKE_LINKER ld CACHE FILEPATH "" FORCE)
 
set (CMAKE_RC_COMPILER windres)

set (CMAKE_FIND_ROOT_PATH "${MSYS2_FOLDER}/mingw64")

 
set (CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)  
set (CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)  
set (CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)  

set (MINGW_FLAG "-m64")
set (USE_LTO_DEFAULT false)
