                                                                            
                                                                

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 

include(CMakePushCheckState)
include(CheckSymbolExists)
include(FindPackageHandleStandardArgs)

 
set(_Backtrace_STD_ARGS Backtrace_INCLUDE_DIR)

if(Backtrace_HEADER)
  set(_Backtrace_HEADER_TRY "${Backtrace_HEADER}")
else(Backtrace_HEADER)
  set(_Backtrace_HEADER_TRY "execinfo.h")
endif(Backtrace_HEADER)

find_path(Backtrace_INCLUDE_DIR "${_Backtrace_HEADER_TRY}")
set(Backtrace_INCLUDE_DIRS ${Backtrace_INCLUDE_DIR})

if (NOT DEFINED Backtrace_LIBRARY)
   
  cmake_push_check_state(RESET)
  set(CMAKE_REQUIRED_INCLUDES ${Backtrace_INCLUDE_DIRS})
  set(CMAKE_REQUIRED_QUIET ${Backtrace_FIND_QUIETLY})
  check_symbol_exists("backtrace" "${_Backtrace_HEADER_TRY}" _Backtrace_SYM_FOUND)
  cmake_pop_check_state()
endif()

if(_Backtrace_SYM_FOUND)
   
  if(NOT Backtrace_FIND_QUIETLY AND NOT DEFINED Backtrace_LIBRARY)
    message(STATUS "backtrace facility detected in default set of libraries")
  endif()
  set(Backtrace_LIBRARY "" CACHE FILEPATH "Library providing backtrace(3), empty for default set of libraries")
else()
   
  if(Backtrace_INCLUDE_DIR)
     
    find_library(Backtrace_LIBRARY "execinfo")
  elseif()      
    set(_Backtrace_HEADER_TRY "backtrace.h")
    find_path(Backtrace_INCLUDE_DIR ${_Backtrace_HEADER_TRY})
    find_library(Backtrace_LIBRARY "backtrace")
  endif()

   
  set(_Backtrace_STD_ARGS Backtrace_LIBRARY ${_Backtrace_STD_ARGS})
endif()

message(STATUS "Backtrace_LIBRARY: ${Backtrace_LIBRARY}")
if(Backtrace_LIBRARY STREQUAL "NOTFOUND")
  set(Backtrace_LIBRARY "")
endif()
if(Backtrace_LIBRARY STREQUAL "Backtrace_LIBRARY-NOTFOUND")
  set(Backtrace_LIBRARY "")
endif()

set(Backtrace_LIBRARIES ${Backtrace_LIBRARY})
set(Backtrace_HEADER "${_Backtrace_HEADER_TRY}" CACHE STRING "Header providing backtrace(3) facility")

find_package_handle_standard_args(Backtrace FOUND_VAR Backtrace_FOUND REQUIRED_VARS ${_Backtrace_STD_ARGS})
mark_as_advanced(Backtrace_HEADER Backtrace_INCLUDE_DIR Backtrace_LIBRARY)
