 
 
 
 
 
 
 

                                                          
 
                                                                              
                                                              

find_path(LIBUNWIND_INCLUDE_DIR libunwind.h
  /usr/include
  /usr/local/include
)

find_library(LIBUNWIND_LIBRARIES NAMES unwind )
if(NOT LIBUNWIND_LIBRARIES STREQUAL "LIBUNWIND_LIBRARIES-NOTFOUND")
  if (CMAKE_COMPILER_IS_GNUCC)
    set(LIBUNWIND_LIBRARIES "gcc_eh;${LIBUNWIND_LIBRARIES}")
  endif()
endif()

 
 
 
 
message(STATUS "looking for liblzma")
find_library(LIBLZMA_LIBRARIES lzma )
if(NOT LIBLZMA_LIBRARIES STREQUAL "LIBLZMA_LIBRARIES-NOTFOUND")
  message(STATUS "liblzma found")
  set(LIBUNWIND_LIBRARIES "${LIBUNWIND_LIBRARIES};${LIBLZMA_LIBRARIES}")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Libunwind "Could not find libunwind" LIBUNWIND_INCLUDE_DIR LIBUNWIND_LIBRARIES)
 
mark_as_advanced(LIBUNWIND_INCLUDE_DIR LIBUNWIND_LIBRARIES )

