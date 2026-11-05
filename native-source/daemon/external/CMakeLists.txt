                                      
                                             
  
                      
  
                                                                                      
                                                           
  
                                                                                        
                                             
  
                                                                                        
                                                                                 
                                              
  
                                                                                      
                                                                                    
                              
  
                                                                                     
                                                                                         
                                                                                        
                                                                                      
                                                                              
                                                                                         
                                                                                   
                                                                                         
                                                                              
  
                                                                                     


 
 
 
 
 

find_package(Miniupnpc REQUIRED)

message(STATUS "Using in-tree miniupnpc")
set(UPNPC_NO_INSTALL TRUE CACHE BOOL "Disable miniupnp installation" FORCE)
set(UPNPC_BUILD_STATIC TRUE CACHE BOOL "Build required static library" FORCE)
set(UPNPC_BUILD_SHARED FALSE CACHE BOOL "No unused shared library" FORCE)
set(UPNPC_BUILD_TESTS FALSE CACHE BOOL "No standalone test tools" FORCE)
set(UPNPC_BUILD_SAMPLE FALSE CACHE BOOL "No sample clients" FORCE)
add_subdirectory(miniupnp/miniupnpc)
set_property(TARGET libminiupnpc-static PROPERTY FOLDER "external")
set_property(TARGET libminiupnpc-static PROPERTY POSITION_INDEPENDENT_CODE ON)
if(MSVC)
  set_property(TARGET libminiupnpc-static APPEND_STRING PROPERTY COMPILE_FLAGS " -wd4244 -wd4267")
elseif(NOT MSVC)
  set_property(TARGET libminiupnpc-static APPEND_STRING PROPERTY COMPILE_FLAGS " -Wno-undef -Wno-unused-result -Wno-unused-value")
endif()
if(CMAKE_SYSTEM_NAME MATCHES "NetBSD")
	set_property(TARGET libminiupnpc-static APPEND_STRING PROPERTY COMPILE_FLAGS " -D_NETBSD_SOURCE")
endif()

set(UPNP_LIBRARIES "libminiupnpc-static" PARENT_SCOPE)

find_package(Unbound)

if(NOT UNBOUND_INCLUDE_DIR)
  die("Could not find libunbound")
else()
  message(STATUS "Found libunbound include (unbound.h) in ${UNBOUND_INCLUDE_DIR}")
  if(UNBOUND_LIBRARIES)
    message(STATUS "Found libunbound library")
  else()
    die("Found libunbound includes, but could not find libunbound library. Please make sure you have installed libunbound or libunbound-dev or the equivalent")
  endif()
endif()

add_subdirectory(db_drivers)
add_subdirectory(easylogging++)
add_subdirectory(randomx EXCLUDE_FROM_ALL)
