                                      
                                             
 
                      
 
                                                                                      
                                                           
 
                                                                                        
                                             
 
                                                                                        
                                                                                 
                                              
 
                                                                                      
                                                                                    
                              
 
                                                                                     
                                                                                         
                                                                                        
                                                                                      
                                                                              
                                                                                         
                                                                                   
                                                                                         
                                                                              
                                                    
 
                                                                      
 
                                  
 
                     
                                                                                                                 
 
                                     
 
                                                                           
                                                                           

find_program(CCACHE_FOUND ccache)
if (CCACHE_FOUND)
	 
	set(TEST_PROJECT "${CMAKE_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/CMakeTmp")
	file(WRITE "${TEST_PROJECT}/CMakeLists.txt" [=[
cmake_minimum_required(VERSION 3.5)
project(test)
option (CCACHE "")
file(WRITE "${CMAKE_SOURCE_DIR}/test.cpp" "int main() { return 0; }")
set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE}")
set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK    "${CCACHE}")
add_executable(main test.cpp)
]=])
	try_compile(RET "${TEST_PROJECT}/build" "${TEST_PROJECT}" "test" CMAKE_FLAGS -DCCACHE="${CCACHE_FOUND}")
	unset(TEST_PROJECT)
	if (${RET})
		 
		message(STATUS "Found usable ccache: ${CCACHE_FOUND}")
		set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_FOUND}")
		set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK    "${CCACHE_FOUND}")
	else()
		message(STATUS "Found ccache ${CCACHE_FOUND}, but is UNUSABLE! Return code: ${RET}")
	endif()
else()
	message(STATUS "ccache NOT found! Please install it for faster rebuilds.")
endif()
