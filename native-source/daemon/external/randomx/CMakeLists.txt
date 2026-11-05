                                      
                                        
  
                      
  
                                                                                      
                                                           
  
                                                                                        
                                             
  
                                                                                        
                                                                                 
                                              
  
                                                                                      
                                                                                    
                              
  
                                                                                     
                                                                                         
                                                                                        
                                                                                      
                                                                              
                                                                                         
                                                                                   
                                                                                         
                                                                              

cmake_minimum_required(VERSION 3.5)

project(RandomX)

set(randomx_sources
src/aes_hash.cpp
src/argon2_ref.c
src/argon2_ssse3.c
src/argon2_avx2.c
src/bytecode_machine.cpp
src/cpu.cpp
src/dataset.cpp
src/soft_aes.cpp
src/virtual_memory.c
src/vm_interpreted.cpp
src/allocator.cpp
src/assembly_generator_x86.cpp
src/instruction.cpp
src/randomx.cpp
src/superscalar.cpp
src/vm_compiled.cpp
src/vm_interpreted_light.cpp
src/argon2_core.c
src/blake2_generator.cpp
src/instructions_portable.cpp
src/reciprocal.c
src/virtual_machine.cpp
src/vm_compiled_light.cpp
src/blake2/blake2b.c)

if(NOT ARCH_ID)
   
  if(CMAKE_SYSTEM_PROCESSOR STREQUAL "")
    set(CMAKE_SYSTEM_PROCESSOR ${CMAKE_HOST_SYSTEM_PROCESSOR})
  endif()
  string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" ARCH_ID)
endif()

if(NOT ARM_ID)
  set(ARM_ID "${ARCH_ID}")
endif()

if(NOT ARCH)
  set(ARCH "default")
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
  message(STATUS "Setting default build type: ${CMAKE_BUILD_TYPE}")
endif()

include(CheckCXXCompilerFlag)
include(CheckCCompilerFlag)

function(add_flag flag)
  string(REPLACE "-" "_" supported_cxx ${flag}_cxx)
  check_cxx_compiler_flag(${flag} ${supported_cxx})
  if(${${supported_cxx}})
    message(STATUS "Setting CXX flag ${flag}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" PARENT_SCOPE)
  endif()
  string(REPLACE "-" "_" supported_c ${flag}_c)
  check_c_compiler_flag(${flag} ${supported_c})
  if(${${supported_c}})
    message(STATUS "Setting C flag ${flag}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${flag}" PARENT_SCOPE)
  endif()
endfunction()

 
if ((CMAKE_SIZEOF_VOID_P EQUAL 8) AND (ARCH_ID STREQUAL "x86_64" OR ARCH_ID STREQUAL "x86-64" OR ARCH_ID STREQUAL "amd64"))
  list(APPEND randomx_sources
    src/jit_compiler_x86.cpp)

  if(MSVC)
    enable_language(ASM_MASM)
    list(APPEND randomx_sources src/jit_compiler_x86_static.asm)

    set_property(SOURCE src/jit_compiler_x86_static.asm PROPERTY LANGUAGE ASM_MASM)

    set_source_files_properties(src/argon2_avx2.c COMPILE_FLAGS /arch:AVX2)

    set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} /DRELWITHDEBINFO")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /DRELWITHDEBINFO")

    add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/src/asm/configuration.asm
      COMMAND powershell -ExecutionPolicy Bypass -File h2inc.ps1 ..\\src\\configuration.h > ..\\src\\asm\\configuration.asm SET ERRORLEVEL = 0
      COMMENT "Generating configuration.asm at ${CMAKE_CURRENT_SOURCE_DIR}"
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/vcxproj)
    add_custom_target(generate-asm
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/asm/configuration.asm)
  else()
    list(APPEND randomx_sources src/jit_compiler_x86_static.S)

     
    set_property(SOURCE src/jit_compiler_x86_static.S PROPERTY LANGUAGE C)
    set_property(SOURCE src/jit_compiler_x86_static.S PROPERTY XCODE_EXPLICIT_FILE_TYPE sourcecode.asm)

    if(ARCH STREQUAL "native")
      add_flag("-march=native")
    else()
       
      add_flag("-maes")
      check_c_compiler_flag(-mssse3 HAVE_SSSE3)
      if(HAVE_SSSE3)
        set_source_files_properties(src/argon2_ssse3.c COMPILE_FLAGS -mssse3)
      endif()
      check_c_compiler_flag(-mavx2 HAVE_AVX2)
      if(HAVE_AVX2)
        set_source_files_properties(src/argon2_avx2.c COMPILE_FLAGS -mavx2)
      endif()
    endif()
  endif()
endif()

 
if(ARCH_ID STREQUAL "ppc64" OR ARCH_ID STREQUAL "ppc64le")
  if(ARCH STREQUAL "native")
    add_flag("-mcpu=native")
  endif()
   
endif()

 
if(ARM_ID STREQUAL "aarch64" OR ARM_ID STREQUAL "arm64" OR ARM_ID STREQUAL "armv8-a")
  list(APPEND randomx_sources
    src/jit_compiler_a64_static.S
    src/jit_compiler_a64.cpp)
   
  set_property(SOURCE src/jit_compiler_a64_static.S PROPERTY LANGUAGE C)
  set_property(SOURCE src/jit_compiler_a64_static.S PROPERTY XCODE_EXPLICIT_FILE_TYPE sourcecode.asm)

   
  include(CheckIncludeFile)
  check_include_file(asm/hwcap.h HAVE_HWCAP)
  if(HAVE_HWCAP)
    add_definitions(-DHAVE_HWCAP)
  endif()

  if(ARCH STREQUAL "native")
    add_flag("-march=native")
  else()
     
    add_flag("-march=armv8-a+crypto")
  endif()
endif()

 
if(ARCH_ID STREQUAL "riscv64")
  list(APPEND randomx_sources
    src/jit_compiler_rv64_static.S
    src/jit_compiler_rv64.cpp)
   
  set_property(SOURCE src/jit_compiler_rv64_static.S PROPERTY LANGUAGE C)
  set_property(SOURCE src/jit_compiler_rv64_static.S PROPERTY XCODE_EXPLICIT_FILE_TYPE sourcecode.asm)

   
  set(RVARCH "rv64gc")

   
  if(ARCH STREQUAL "native")
    enable_language(ASM)
    try_run(RANDOMX_ZBA_RUN_FAIL
        RANDOMX_ZBA_COMPILE_OK
        ${CMAKE_CURRENT_BINARY_DIR}/
        ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/riscv64_zba.s
        COMPILE_DEFINITIONS "-march=rv64gc_zba")
    if (RANDOMX_ZBA_COMPILE_OK AND NOT RANDOMX_ZBA_RUN_FAIL)
      set(RVARCH "${RVARCH}_zba")
    endif()
    try_run(RANDOMX_ZBB_RUN_FAIL
        RANDOMX_ZBB_COMPILE_OK
        ${CMAKE_CURRENT_BINARY_DIR}/
        ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/riscv64_zbb.s
        COMPILE_DEFINITIONS "-march=rv64gc_zbb")
    if (RANDOMX_ZBB_COMPILE_OK AND NOT RANDOMX_ZBB_RUN_FAIL)
      set(RVARCH "${RVARCH}_zbb")
    endif()
  endif()

  add_flag("-march=${RVARCH}")
endif()

set(RANDOMX_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/src" CACHE STRING "RandomX Include path")

add_library(randomx ${randomx_sources})

if(TARGET generate-asm)
  add_dependencies(randomx generate-asm)
endif()

set_property(TARGET randomx PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET randomx PROPERTY CXX_STANDARD 11)
set_property(TARGET randomx PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET randomx PROPERTY PUBLIC_HEADER src/randomx.h)

include(GNUInstallDirs)
install(TARGETS randomx
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

