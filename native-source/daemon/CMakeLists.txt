                                      
                                             
 
                      
 
                                                                                      
                                                           
 
                                                                                        
                                             
 
                                                                                        
                                                                                 
                                              
 
                                                                                      
                                                                                    
                              
 
                                                                                     
                                                                                         
                                                                                        
                                                                                      
                                                                              
                                                                                         
                                                                                   
                                                                                         
                                                                              
 
                                                                                     

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(STATUS "Venera non-mining sync build; public approval verification enabled")

set(VENERA_BLOCK_APPROVAL_HEIGHT "574969" CACHE STRING "Future block-approval activation height")
set(VENERA_BLOCK_APPROVAL_PUBKEY "807595e10f4d4a0c033434740dd14af448ea0315eaea07a19013a6e5bda2985d" CACHE STRING "Central block-signing public key (64 hex digits)")
if(NOT VENERA_BLOCK_APPROVAL_HEIGHT MATCHES "^[1-9][0-9]*$" OR VENERA_BLOCK_APPROVAL_HEIGHT LESS 2)
  message(FATAL_ERROR "Set -DVENERA_BLOCK_APPROVAL_HEIGHT to an agreed future height (>1)")
endif()

string(LENGTH "${VENERA_BLOCK_APPROVAL_HEIGHT}" approval_height_length)
if(approval_height_length GREATER 20 OR (approval_height_length EQUAL 20 AND VENERA_BLOCK_APPROVAL_HEIGHT STRGREATER "18446744073709551615"))
  message(FATAL_ERROR "VENERA_BLOCK_APPROVAL_HEIGHT must fit uint64")
endif()
string(LENGTH "${VENERA_BLOCK_APPROVAL_PUBKEY}" approval_key_length)
if(NOT approval_key_length EQUAL 64 OR NOT VENERA_BLOCK_APPROVAL_PUBKEY MATCHES "^[0-9a-fA-F]+$" OR VENERA_BLOCK_APPROVAL_PUBKEY MATCHES "^0+$")
  message(FATAL_ERROR "Set -DVENERA_BLOCK_APPROVAL_PUBKEY to your real public signing key (64 hex digits)")
endif()
add_definitions(-DVENERA_BLOCK_APPROVAL_HEIGHT=${VENERA_BLOCK_APPROVAL_HEIGHT})
add_compile_definitions("VENERA_BLOCK_APPROVAL_PUBKEY=\"${VENERA_BLOCK_APPROVAL_PUBKEY}\"")

 
 
 
set(VENERA_EMISSION_CAP_HEIGHT "575928" CACHE STRING "Agreed future subsidy-cap activation height")
if(NOT VENERA_EMISSION_CAP_HEIGHT MATCHES "^[1-9][0-9]*$" OR VENERA_EMISSION_CAP_HEIGHT LESS 572960)
  message(FATAL_ERROR "Set -DVENERA_EMISSION_CAP_HEIGHT to an agreed future block height (>=572960); keep the original approval height")
endif()
string(LENGTH "${VENERA_EMISSION_CAP_HEIGHT}" emission_cap_height_length)
if(emission_cap_height_length GREATER 20 OR (emission_cap_height_length EQUAL 20 AND VENERA_EMISSION_CAP_HEIGHT STRGREATER "18446744073709551615"))
  message(FATAL_ERROR "VENERA_EMISSION_CAP_HEIGHT must fit uint64")
endif()
add_definitions(-DVENERA_EMISSION_CAP_HEIGHT=${VENERA_EMISSION_CAP_HEIGHT}ULL)
message(STATUS "Venera subsidy cap: 6944444444 atomic units/block, active at ${VENERA_EMISSION_CAP_HEIGHT}")


list(INSERT CMAKE_MODULE_PATH 0
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)
include(CheckLinkerFlag)
include(CheckLibraryExists)
include(CheckFunctionExists)
include(FindPythonInterp)

if (IOS)
    INCLUDE(CmakeLists_IOS.txt)
endif()

cmake_minimum_required(VERSION 3.5)
message(STATUS "CMake version ${CMAKE_VERSION}")

project(venera)

option (USE_CCACHE "Use ccache if a usable instance is found" ON)
if (USE_CCACHE)
	include(FindCcache)  
else()
	message(STATUS "ccache deselected")
endif()
option (USE_COMPILATION_TIME_PROFILER "Use compilation time profiler (for CLang >= 9 only)" OFF)
if (USE_COMPILATION_TIME_PROFILER)
	if (NOT "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
		message(FATAL_ERROR "The flag USE_COMPILATION_TIME_PROFILER is meant to be set only for CLang compiler!")
	endif()
	add_compile_options("-ftime-trace")
endif()

if (${CMAKE_VERSION} VERSION_GREATER "3.0.0" AND CMAKE_MAKE_PROGRAM MATCHES "ninja")
  set(MONERO_PARALLEL_COMPILE_JOBS "" CACHE STRING "The maximum number of concurrent compilation jobs.")
  if (MONERO_PARALLEL_COMPILE_JOBS)
    set_property(GLOBAL APPEND PROPERTY JOB_POOLS compile_job_pool=${MONERO_PARALLEL_COMPILE_JOBS})
    set(CMAKE_JOB_POOL_COMPILE compile_job_pool)
  endif ()

  set(MONERO_PARALLEL_LINK_JOBS "" CACHE STRING "The maximum number of concurrent link jobs.")
  if (MONERO_PARALLEL_LINK_JOBS)
    set_property(GLOBAL APPEND PROPERTY JOB_POOLS link_job_pool=${MONERO_PARALLEL_LINK_JOBS})
    set(CMAKE_JOB_POOL_LINK link_job_pool)
  endif ()
endif ()
  
option (USE_CLANG_TIDY_C   "Lint the code with clang-tidy - variant C"   OFF)
option (USE_CLANG_TIDY_CXX "Lint the code with clang-tidy - variant C++" OFF)
if (USE_CLANG_TIDY_C AND USE_CLANG_TIDY_CXX)
	message(FATAL_ERROR "Enabling both USE_CLANG_TIDY_C and USE_CLANG_TIDY_CXX simultaneously crashes clang-tidy.")
endif()
if (USE_CLANG_TIDY_C OR USE_CLANG_TIDY_CXX)
	include(SetClangTidy)
endif()
if (USE_CLANG_TIDY_C)
	monero_clang_tidy("C")
elseif (USE_CLANG_TIDY_CXX)
	monero_clang_tidy("CXX")
endif()

enable_language(C ASM)

 
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

function (die msg)
  if (NOT WIN32)
    string(ASCII 27 Esc)
    set(ColourReset "${Esc}[m")
    set(BoldRed     "${Esc}[1;31m")
  else ()
    set(ColourReset "")
    set(BoldRed     "")
  endif ()

  message(FATAL_ERROR "${BoldRed}${msg}${ColourReset}")
endfunction ()

function (add_c_flag_if_supported flag var)
   
   
  set(TMP "-Werror ${flag}")
  string(REGEX REPLACE "[- ]" "_" supported ${TMP}_c)
  check_c_compiler_flag(${TMP} ${supported})
  if(${${supported}})
    set(${var} "${${var}} ${flag}" PARENT_SCOPE)
  endif()
endfunction()

function (add_cxx_flag_if_supported flag var)
  set(TMP "-Werror ${flag}")
  string(REGEX REPLACE "[- ]" "_" supported ${TMP}_cxx)
  check_cxx_compiler_flag(${TMP} ${supported})
  if(${${supported}})
    set(${var} "${${var}} ${flag}" PARENT_SCOPE)
  endif()
endfunction()

function (add_linker_flag_if_supported flag var)
  string(REPLACE "-" "_" supported ${flag}_ld)
  string(REPLACE "," "_" supported ${flag}_ld)
  check_linker_flag(${flag} ${supported})
  if(${${supported}})
    set(${var} "${${var}} ${flag}" PARENT_SCOPE)
  endif()
endfunction()

function (add_definition_if_function_found function var)
  string(REPLACE "-" "_" supported ${function}_function)
  check_function_exists(${function} ${supported})
  if(${${supported}})
    add_definitions("-D${var}")
  endif()
endfunction()

function (add_definition_if_library_exists library function header var)
  string(REPLACE "-" "_" supported ${function}_library)
  check_library_exists(${library} ${function} ${header} ${supported})
  if(${${supported}})
    add_definitions("-D${var}")
  endif()
endfunction()

option(RELINK_TARGETS "Relink targets, when just a dependant .so changed, but not its header?" OFF)
function (monero_set_target_no_relink target)
  if (RELINK_TARGETS MATCHES OFF)
     
    set_target_properties("${target}" PROPERTIES LINK_DEPENDS_NO_SHARED true)
  endif()
endfunction()

option(STRIP_TARGETS "Strip symbols from targets?" OFF)
function (monero_set_target_strip target)
  if (STRIP_TARGETS)
    set_target_properties("${target}" PROPERTIES LINK_FLAGS_RELEASE -s)
    set_target_properties("${target}" PROPERTIES LINK_FLAGS_DEBUG -s)
     
  endif()
endfunction()

function (monero_add_minimal_executable name)
  source_group("${name}"
    FILES
      ${ARGN})

  add_executable("${name}"
    ${ARGN})
    monero_set_target_no_relink("${name}")
    monero_set_target_strip    ("${name}")
endfunction()

 
 
 
 
 
macro (monero_find_all_headers headers_found module_root_dir)
  file(GLOB ${headers_found}
           "${module_root_dir}/*.h*"     
           "${module_root_dir}/**/*.h*"  
           "${module_root_dir}/*.inl"    
           "${module_root_dir}/**/*.inl"
)
endmacro()

 
 
 
function(forbid_undefined_symbols)
  unset(TMP)
   
  add_linker_flag_if_supported(-Wl,--no-undefined TMP)
   
  add_linker_flag_if_supported(-Wl,-undefined,error TMP)
  string(APPEND CMAKE_SHARED_LINKER_FLAGS ${TMP})
  string(APPEND CMAKE_MODULE_LINKER_FLAGS ${TMP})
  set(CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} PARENT_SCOPE)
  set(CMAKE_MODULE_LINKER_FLAGS ${CMAKE_MODULE_LINKER_FLAGS} PARENT_SCOPE)
  set(TEST_PROJECT "${CMAKE_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/CMakeTmp/test_project")
  foreach(EXPECT IN ITEMS TRUE FALSE)
    file(REMOVE_RECURSE "${TEST_PROJECT}")
    file(MAKE_DIRECTORY "${TEST_PROJECT}")
    file(WRITE "${TEST_PROJECT}/CMakeLists.txt"
      [=[
cmake_minimum_required(VERSION 3.5)
project(test)
option(EXPECT_SUCCESS "" ON)
file(WRITE "${CMAKE_SOURCE_DIR}/incorrect_source.cpp" "void undefined_symbol(); void symbol() { undefined_symbol(); }")
if (EXPECT_SUCCESS)
  file(APPEND "${CMAKE_SOURCE_DIR}/incorrect_source.cpp" " void undefined_symbol() {}; ")
endif()
add_library(l0 SHARED incorrect_source.cpp)
add_library(l1 MODULE incorrect_source.cpp)
add_library(l2 STATIC incorrect_source.cpp)
add_library(l3 OBJECT incorrect_source.cpp)
]=]
    )
    try_compile(SUCCESS "${TEST_PROJECT}/build" "${TEST_PROJECT}" test
      CMAKE_FLAGS
      "-DCMAKE_SHARED_LINKER_FLAGS=${CMAKE_SHARED_LINKER_FLAGS}"
      "-DCMAKE_MODULE_LINKER_FLAGS=${CMAKE_MODULE_LINKER_FLAGS}"
      "-DEXPECT_SUCCESS=${EXPECT}"
    )
    if (NOT ${SUCCESS} STREQUAL ${EXPECT})
      message(FATAL_ERROR "Undefined symbols test failure: expect(${EXPECT}), success(${SUCCESS})")
    endif()
    file(REMOVE_RECURSE "${TEST_PROJECT}")
  endforeach()
endfunction()
if (NOT (CMAKE_SYSTEM_NAME MATCHES "kOpenBSD.*|OpenBSD.*") AND NOT OSSFUZZ)
  forbid_undefined_symbols()
endif()

if (MINGW)
  function(export_all_symbols)
    unset(TMP)
    add_linker_flag_if_supported(-Wl,--export-all-symbols TMP)
    string(APPEND CMAKE_SHARED_LINKER_FLAGS ${TMP})
    string(APPEND CMAKE_MODULE_LINKER_FLAGS ${TMP})
    set(CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} PARENT_SCOPE)
    set(CMAKE_MODULE_LINKER_FLAGS ${CMAKE_MODULE_LINKER_FLAGS} PARENT_SCOPE)
  endfunction()
  export_all_symbols()
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
  message(STATUS "Setting default build type: ${CMAKE_BUILD_TYPE}")
endif()
string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_LOWER)

 
 
 
 
 
 
 
 
if (NOT ARCH_ID)
if (NOT ARCH OR ARCH STREQUAL "" OR ARCH STREQUAL "native" OR ARCH STREQUAL "default")
  if(CMAKE_SYSTEM_PROCESSOR STREQUAL "")
    set(CMAKE_SYSTEM_PROCESSOR ${CMAKE_HOST_SYSTEM_PROCESSOR})
  endif()
  set(ARCH_ID "${CMAKE_SYSTEM_PROCESSOR}")
else()
  set(ARCH_ID "${ARCH}")
endif()
endif()
string(TOLOWER "${ARCH_ID}" ARM_ID)
string(SUBSTRING "${ARM_ID}" 0 3 ARM_TEST)
if (ARM_TEST STREQUAL "arm")
  set(ARM 1)
  string(SUBSTRING "${ARM_ID}" 0 5 ARM_TEST)
  if (ARM_TEST STREQUAL "armv6")
    set(ARM6 1)
  endif()
  if (ARM_TEST STREQUAL "armv7")
    set(ARM7 1)
  endif()
endif()

if (ARM_ID STREQUAL "aarch64" OR ARM_ID STREQUAL "arm64" OR ARM_ID STREQUAL "armv8-a")
  set(ARM 1)
  set(ARM8 1)
  set(ARCH "armv8-a")
endif()

if(ARCH_ID STREQUAL "ppc64le")
  set(PPC64LE 1)
  set(PPC64   0)
  set(PPC     0)
endif()

if(ARCH_ID STREQUAL "powerpc64" OR ARCH_ID STREQUAL "ppc64")
  set(PPC64LE 0)
  set(PPC64   1)
  set(PPC     0)
endif()

if(ARCH_ID STREQUAL "powerpc" OR ARCH_ID STREQUAL "ppc")
  set(PPC64LE 0)
  set(PPC64   0)
  set(PPC     1)
endif()

if(ARCH_ID STREQUAL "s390x")
  set(S390X 1)
endif()

if(ARCH_ID STREQUAL "riscv64")
set(RISCV   1)
set(RISCV64 1)
endif()

if(ARCH_ID STREQUAL "riscv32")
set(RISCV   1)
set(RISCV32 1)
endif()

if(WIN32 OR ARM OR PPC64LE OR PPC64 OR PPC)
  set(OPT_FLAGS_RELEASE "-O2")
else()
  set(OPT_FLAGS_RELEASE "-Ofast")
endif()

 
if(BUILD_TAG)
  message(STATUS "Building build tag ${BUILD_TAG}")
  add_definitions("-DBUILD_TAG=${BUILD_TAG}")
else()
  message(STATUS "Building without build tag")
endif()

 
set(PER_BLOCK_CHECKPOINT 0)

if(PER_BLOCK_CHECKPOINT)
  add_definitions("-DPER_BLOCK_CHECKPOINT")
  set(Blocks "blocks")
else()
  set(Blocks "")
endif()

list(INSERT CMAKE_MODULE_PATH 0
  "${CMAKE_SOURCE_DIR}/cmake")

if (NOT DEFINED ENV{DEVELOPER_LOCAL_TOOLS})
  message(STATUS "Could not find DEVELOPER_LOCAL_TOOLS in env (not required)")
  set(BOOST_IGNORE_SYSTEM_PATHS_DEFAULT OFF)
elseif ("$ENV{DEVELOPER_LOCAL_TOOLS}" EQUAL 1)
  message(STATUS "Found: env DEVELOPER_LOCAL_TOOLS = 1")
  set(BOOST_IGNORE_SYSTEM_PATHS_DEFAULT ON)
else()
  message(STATUS "Found: env DEVELOPER_LOCAL_TOOLS = 0")
  set(BOOST_IGNORE_SYSTEM_PATHS_DEFAULT OFF)
endif()

message(STATUS "BOOST_IGNORE_SYSTEM_PATHS defaults to ${BOOST_IGNORE_SYSTEM_PATHS_DEFAULT}")
option(BOOST_IGNORE_SYSTEM_PATHS "Ignore boost system paths for local boost installation" ${BOOST_IGNORE_SYSTEM_PATHS_DEFAULT})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
enable_testing()

set(BUILD_DOCUMENTATION OFF)
set(BUILD_TESTS OFF)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(DEFAULT_BUILD_DEBUG_UTILITIES ON)
else()
  set(DEFAULT_BUILD_DEBUG_UTILITIES OFF)
endif()
set(BUILD_DEBUG_UTILITIES OFF)

if(OSSFUZZ)
  message(STATUS "Using OSS-Fuzz fuzzing system")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DOSSFUZZ")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DOSSFUZZ")
endif()

 
if(CMAKE_SIZEOF_VOID_P EQUAL "8")
  set(DEFAULT_BUILD_64 ON)
else()
  set(DEFAULT_BUILD_64 OFF)
endif()
option(BUILD_64 "Build for 64-bit? 'OFF' builds for 32-bit." ${DEFAULT_BUILD_64})

if(BUILD_64)
  set(ARCH_WIDTH "64")
else()
  set(ARCH_WIDTH "32")
endif()
message(STATUS "Building for a ${ARCH_WIDTH}-bit system")

 
 
if(CMAKE_SYSTEM_NAME MATCHES "kFreeBSD.*|FreeBSD")
  set(FREEBSD TRUE)
endif()

 
if(CMAKE_SYSTEM_NAME MATCHES "DragonFly.*")
  set(DRAGONFLY TRUE)
endif()

 
if(CMAKE_SYSTEM_NAME MATCHES "kOpenBSD.*|OpenBSD.*")
  set(OPENBSD TRUE)
endif()

 
 
 
 
 
 

include_directories(external/rapidjson/include external/easylogging++ src contrib/epee/include external external/supercop/include)

if(APPLE)
  cmake_policy(SET CMP0042 NEW)
endif()

if(MSVC OR MINGW)
  set(DEFAULT_STATIC true)
else()
  set(DEFAULT_STATIC false)
endif()
option(STATIC "Link libraries statically" ${DEFAULT_STATIC})

 
set(BUILD_SHARED_LIBS_DEFAULT OFF)
if (NOT STATIC AND CMAKE_BUILD_TYPE_LOWER STREQUAL "debug")
	set(BUILD_SHARED_LIBS_DEFAULT ON)
endif()
option(BUILD_SHARED_LIBS "Build internal libraries as shared" ${BUILD_SHARED_LIBS_DEFAULT})

if (BUILD_SHARED_LIBS)
  message(STATUS "Building internal libraries with position independent code")
  add_definitions("-DBUILD_SHARED_LIBS")
else()
  message(STATUS "Building internal libraries as static")
endif()
set(PIC_FLAG "-fPIC")

if(MINGW)
  get_filename_component(msys2_compiler_bin "${CMAKE_C_COMPILER}" DIRECTORY)
  get_filename_component(msys2_toolchain_root "${msys2_compiler_bin}" DIRECTORY)
  message(STATUS "MSYS toolchain: ${msys2_toolchain_root}")
  list(APPEND CMAKE_INCLUDE_PATH "${msys2_toolchain_root}/include")
   
   
   
   
  set(DEFLIB "${msys2_toolchain_root}/lib")
  list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_DIRECTORIES ${DEFLIB})
  list(REMOVE_ITEM CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES ${DEFLIB})
endif()

if(STATIC)
  if(MSVC)
    set(CMAKE_FIND_LIBRARY_SUFFIXES .lib .dll.a .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
  else()
    set(CMAKE_FIND_LIBRARY_SUFFIXES .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
  endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DZMQ_STATIC")
endif()

option(SANITIZE "Use ASAN memory sanitizer" OFF)
if(SANITIZE)
  if (MSVC)
    message(FATAL_ERROR "Cannot sanitize with MSVC")
  else()
    message(STATUS "Using ASAN")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
  endif()
endif()

 
 
 
set(DATABASE lmdb)
message(STATUS "Using LMDB as default DB type")
set(BLOCKCHAIN_DB DB_LMDB)
add_definitions("-DDEFAULT_DB_TYPE=\"lmdb\"")
add_definitions("-DBLOCKCHAIN_DB=${BLOCKCHAIN_DB}")

 
 
 
if (APPLE)
  set(DEFAULT_STACK_TRACE OFF)
  set(LIBUNWIND_LIBRARIES "")
elseif (DEPENDS AND NOT LINUX)
  set(DEFAULT_STACK_TRACE OFF)
  set(LIBUNWIND_LIBRARIES "")
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU" AND NOT MINGW)
  set(DEFAULT_STACK_TRACE ON)
  set(STACK_TRACE_LIB "easylogging++")  
  set(LIBUNWIND_LIBRARIES "")
elseif (ARM)
  set(DEFAULT_STACK_TRACE OFF)
  set(LIBUNWIND_LIBRARIES "")
else()
  find_package(Libunwind)
  if(LIBUNWIND_FOUND)
    set(DEFAULT_STACK_TRACE ON)
    set(STACK_TRACE_LIB "libunwind")  
  else()
    set(DEFAULT_STACK_TRACE OFF)
    set(LIBUNWIND_LIBRARIES "")
  endif()
endif()

option(STACK_TRACE "Install a hook that dumps stack on exception" ${DEFAULT_STACK_TRACE})

if(STACK_TRACE)
  message(STATUS "Stack trace on exception enabled (using ${STACK_TRACE_LIB})")
else()
  message(STATUS "Stack trace on exception disabled")
endif()

if (UNIX AND NOT APPLE)
   
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads)
  add_c_flag_if_supported(-pthread CMAKE_C_FLAGS)
  add_cxx_flag_if_supported(-pthread CMAKE_CXX_FLAGS)
endif()

 
if (CMAKE_SYSTEM_NAME MATCHES "(SunOS|Solaris)")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthreads")
endif ()

if (APPLE AND NOT IOS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=default")
  if (NOT OPENSSL_ROOT_DIR)
      EXECUTE_PROCESS(COMMAND brew --prefix openssl
        OUTPUT_VARIABLE OPENSSL_ROOT_DIR
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    message(STATUS "Using OpenSSL found at ${OPENSSL_ROOT_DIR}")
  endif()
endif()

find_package(OpenSSL 1.1 REQUIRED)
message(STATUS "Using OpenSSL include dir at ${OPENSSL_INCLUDE_DIR}")
include_directories(${OPENSSL_INCLUDE_DIR})
if(STATIC AND NOT IOS)
  if(UNIX)
    set(OPENSSL_LIBRARIES "${OPENSSL_LIBRARIES};${CMAKE_DL_LIBS};${CMAKE_THREAD_LIBS_INIT}")
  endif()
endif()

if (WIN32)
  list(APPEND OPENSSL_LIBRARIES ws2_32 crypt32)
endif()

 
 
set(HIDAPI_FOUND OFF)
set(HIDAPI_LIBRARIES "")
set(DEVICE_TREZOR_READY OFF)
set(USE_DEVICE_TREZOR OFF CACHE BOOL "No hardware wallet integration in this build" FORCE)
set(USE_DEVICE_TREZOR_LIBUSB OFF CACHE BOOL "No USB hardware wallet integration" FORCE)

add_definition_if_library_exists(c memset_s "string.h" HAVE_MEMSET_S)
add_definition_if_library_exists(c explicit_bzero "strings.h" HAVE_EXPLICIT_BZERO)
add_definition_if_function_found(strptime HAVE_STRPTIME)

add_definitions(-DAUTO_INITIALIZE_EASYLOGGINGPP)

set(MONERO_GENERATED_HEADERS_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated_include")
include_directories(${MONERO_GENERATED_HEADERS_DIR})

 
function(set_default_arch)
  if (OPENBSD)
    set(ARCH default)
  else()
    set(ARCH native)
  endif()

  set(ARCH ${ARCH} CACHE STRING "CPU to build for: -march value or 'default' to not pass -march at all")
endfunction()

if (NOT (MSVC OR ARCH))
    set_default_arch()
endif()

option(COVERAGE "Enable profiling for test coverage report" OFF)
if(COVERAGE)
    message(STATUS "Building with profiling for test coverage report")
endif()
macro (monero_enable_coverage)
  if(COVERAGE)
    foreach(COV_FLAG -fprofile-arcs -ftest-coverage --coverage)
      set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${COV_FLAG}")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COV_FLAG}")
    endforeach()
  endif()
endmacro()

function (monero_add_library name)
    monero_add_library_with_deps(NAME "${name}" SOURCES ${ARGN})
endfunction()

function (monero_add_library_with_deps)
  cmake_parse_arguments(MONERO_ADD_LIBRARY "" "NAME" "DEPENDS;SOURCES" ${ARGN})
  source_group("${MONERO_ADD_LIBRARY_NAME}" FILES ${MONERO_ADD_LIBRARY_SOURCES})

   
   
   
   
  set(objlib obj_${MONERO_ADD_LIBRARY_NAME})
  add_library(${objlib} OBJECT ${MONERO_ADD_LIBRARY_SOURCES})
  add_library("${MONERO_ADD_LIBRARY_NAME}" $<TARGET_OBJECTS:${objlib}>)
  monero_set_target_no_relink("${MONERO_ADD_LIBRARY_NAME}")
  monero_set_target_strip    ("${MONERO_ADD_LIBRARY_NAME}")
  if (MONERO_ADD_LIBRARY_DEPENDS)
    add_dependencies(${objlib} ${MONERO_ADD_LIBRARY_DEPENDS})
  endif()
  set_property(TARGET "${MONERO_ADD_LIBRARY_NAME}" PROPERTY FOLDER "libs")
  target_compile_definitions(${objlib}
    PRIVATE $<TARGET_PROPERTY:${MONERO_ADD_LIBRARY_NAME},INTERFACE_COMPILE_DEFINITIONS>)
endfunction ()

 
 
 
include(ExternalProject)
ExternalProject_Add(generate_translations_header
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/translations"
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/translations"
  STAMP_DIR ${LRELEASE_PATH}
  CMAKE_ARGS -DLRELEASE_PATH=${LRELEASE_PATH}
  INSTALL_COMMAND ${CMAKE_COMMAND} -E echo "")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/translations")
add_subdirectory(external)

 
include_directories(${UNBOUND_INCLUDE_DIR})

 
include_directories(${EASYLOGGING_INCLUDE})
link_directories(${EASYLOGGING_LIBRARY_DIRS})

 
include_directories(${LMDB_INCLUDE})

 
include_directories(${LIBUNWIND_INCLUDE})
link_directories(${LIBUNWIND_LIBRARY_DIRS})

 
if (HIDAPI_FOUND) 
  message(STATUS "Using HIDAPI include dir at ${HIDAPI_INCLUDE_DIR}")
  add_definitions(-DHAVE_HIDAPI)
  include_directories(${HIDAPI_INCLUDE_DIR})
  link_directories(${LIBHIDAPI_LIBRARY_DIRS})
else()
  message(STATUS "Could not find HIDAPI")
endif()


if(MSVC)
  add_definitions("/bigobj /MP /W3 /GS- /D_CRT_SECURE_NO_WARNINGS /wd4996 /wd4345 /D_WIN32_WINNT=0x0600 /DWIN32_LEAN_AND_MEAN /DGTEST_HAS_TR1_TUPLE=0 /FIinline_c.h /D__SSE4_1__")
   
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /STACK:10485760")
  if(STATIC)
    foreach(VAR CMAKE_C_FLAGS_DEBUG CMAKE_CXX_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_CXX_FLAGS_RELEASE)
      string(REPLACE "/MD" "/MT" ${VAR} "${${VAR}}")
    endforeach()
  endif()
  include_directories(SYSTEM src/platform/msc)
else()
  include(TestCXXAcceptsFlag)
  message(STATUS "Building on ${CMAKE_SYSTEM_PROCESSOR} for ${ARCH}")
  if(ARCH STREQUAL "default")
    set(ARCH_FLAG "")
  elseif(PPC64LE)
    set(ARCH_FLAG "-mcpu=power8")
  elseif(PPC64)
    set(ARCH_FLAG "-mcpu=970")
  elseif(PPC)
    set(ARCH_FLAG "-mcpu=7400")
  elseif(IOS AND ARCH STREQUAL "arm64")
    message(STATUS "IOS: Changing arch from arm64 to armv8")
    set(ARCH_FLAG "-march=armv8")
  else()
    set(ARCH_FLAG "-march=${ARCH}")
    if(ARCH STREQUAL "native")
      check_c_compiler_flag(-march=native CC_SUPPORTS_MARCH_NATIVE)
      if (NOT CC_SUPPORTS_MARCH_NATIVE)
        check_c_compiler_flag(-mtune=native CC_SUPPORTS_MTUNE_NATIVE)
        if (CC_SUPPORTS_MTUNE_NATIVE)
          set(ARCH_FLAG "-mtune=${ARCH}")
        else()
          set(ARCH_FLAG "")
        endif()
      endif()
    endif()
  endif()

  option(NO_AES "Explicitly disable AES support" ${NO_AES})

  if(NO_AES)
    message(STATUS "AES support explicitly disabled")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNO_AES")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNO_AES")
  elseif(NOT ARM AND NOT PPC64LE AND NOT PPC64 AND NOT PPC AND NOT S390X AND NOT RISCV)
    message(STATUS "AES support enabled")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -maes")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -maes")
  elseif(PPC64LE OR PPC64 OR PPC)
    message(STATUS "AES support not available on POWER")
  elseif(S390X)
    message(STATUS "AES support not available on s390x")
  elseif(RISCV)
    message(STATUS "AES support not available on RISC-V")
  elseif(ARM6)
    message(STATUS "AES support not available on ARMv6")
  elseif(ARM7)
    message(STATUS "AES support not available on ARMv7")
  elseif(ARM8)
    CHECK_CXX_ACCEPTS_FLAG("-march=${ARCH}+crypto" ARCH_PLUS_CRYPTO)
    if(ARCH_PLUS_CRYPTO)
      message(STATUS "Crypto extensions enabled for ARMv8")
      set(ARCH_FLAG "-march=${ARCH}+crypto")
    else()
      message(STATUS "Crypto extensions unavailable on your ARMv8 device")
    endif()
  else()
    message(STATUS "AES support disabled")
  endif()

  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ARCH_FLAG}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ARCH_FLAG}")

  set(WARNINGS "-Wall -Wextra -Wpointer-arith -Wundef -Wvla -Wwrite-strings -Wno-error=extra -Wno-error=deprecated-declarations -Wno-unused-parameter -Wno-error=unused-variable -Wno-error=undef -Wno-error=uninitialized")
  if(CMAKE_C_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if(ARM)
      set(WARNINGS "${WARNINGS} -Wno-error=inline-asm")
    endif()
  else()
    set(WARNINGS "${WARNINGS} -Wlogical-op -Wno-error=maybe-uninitialized -Wno-error=cpp")
  endif()
  if(MINGW)
    set(WARNINGS "${WARNINGS} -Wno-error=unused-value -Wno-error=unused-but-set-variable")
    set(MINGW_FLAG "${MINGW_FLAG} -DWIN32_LEAN_AND_MEAN")
    set(Boost_THREADAPI win32)
    include_directories(SYSTEM src/platform/mingw)
     
    set(USE_LTO_DEFAULT false)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--stack,10485760")
    if(NOT BUILD_64)
      add_definitions(-DWINVER=0x0600 -D_WIN32_WINNT=0x0600)
    endif()
  endif()
  set(C_WARNINGS "-Waggregate-return -Wnested-externs -Wold-style-definition -Wstrict-prototypes")
  set(CXX_WARNINGS "-Wno-reorder -Wno-missing-field-initializers")
  try_compile(STATIC_ASSERT_RES "${CMAKE_CURRENT_BINARY_DIR}/static-assert" "${CMAKE_CURRENT_SOURCE_DIR}/cmake/test-static-assert.c" CMAKE_FLAGS -DCMAKE_C_STANDARD=11)
  if(STATIC_ASSERT_RES)
    set(STATIC_ASSERT_FLAG "")
  else()
    set(STATIC_ASSERT_FLAG "-Dstatic_assert=_Static_assert")
  endif()

  try_compile(STATIC_ASSERT_CPP_RES "${CMAKE_CURRENT_BINARY_DIR}/static-assert" "${CMAKE_CURRENT_SOURCE_DIR}/cmake/test-static-assert.cpp" CMAKE_FLAGS -DCMAKE_CXX_STANDARD=11)
  if(STATIC_ASSERT_CPP_RES)
    set(STATIC_ASSERT_CPP_FLAG "")
  else()
    set(STATIC_ASSERT_CPP_FLAG "-Dstatic_assert=_Static_assert")
  endif()

  monero_enable_coverage()
   
   
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-strict-aliasing")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-strict-aliasing")

   
  if(CMAKE_BUILD_TYPE STREQUAL "Release" AND NOT OPENBSD)
    set(C_SECURITY_FLAGS "${C_SECURITY_FLAGS} -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=1")
    set(CXX_SECURITY_FLAGS "${CXX_SECURITY_FLAGS} -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=1")
  endif()

   
  add_c_flag_if_supported(-Wformat C_SECURITY_FLAGS)
  add_cxx_flag_if_supported(-Wformat CXX_SECURITY_FLAGS)
  add_c_flag_if_supported(-Wformat-security C_SECURITY_FLAGS)
  add_cxx_flag_if_supported(-Wformat-security CXX_SECURITY_FLAGS)

   
  if (NOT OPENBSD AND NOT (WIN32 AND (CMAKE_C_COMPILER_ID STREQUAL "GNU" AND CMAKE_C_COMPILER_VERSION VERSION_LESS 9.1)))
    add_c_flag_if_supported(-fstack-protector C_SECURITY_FLAGS)
    add_cxx_flag_if_supported(-fstack-protector CXX_SECURITY_FLAGS)
    add_c_flag_if_supported(-fstack-protector-strong C_SECURITY_FLAGS)
    add_cxx_flag_if_supported(-fstack-protector-strong CXX_SECURITY_FLAGS)
  endif()

   
  if (NOT OPENBSD AND NOT (WIN32 AND (CMAKE_C_COMPILER_ID STREQUAL "GNU" AND CMAKE_C_COMPILER_VERSION VERSION_LESS 9.1)))
    add_c_flag_if_supported(-fcf-protection=full C_SECURITY_FLAGS)
    add_cxx_flag_if_supported(-fcf-protection=full CXX_SECURITY_FLAGS)
  endif()
  if (NOT WIN32 AND NOT OPENBSD)
    add_c_flag_if_supported(-fstack-clash-protection C_SECURITY_FLAGS)
    add_cxx_flag_if_supported(-fstack-clash-protection CXX_SECURITY_FLAGS)
  endif()

   
  if (NOT (CMAKE_C_COMPILER_ID STREQUAL "GNU" AND NOT CMAKE_C_COMPILER_VERSION VERSION_LESS 9.1))
    add_c_flag_if_supported(-mmitigate-rop C_SECURITY_FLAGS)
    add_cxx_flag_if_supported(-mmitigate-rop CXX_SECURITY_FLAGS)
  endif()

   
  if (NOT SANITIZE AND NOT OSSFUZZ AND NOT (WIN32 AND (CMAKE_C_COMPILER_ID STREQUAL "GNU" AND (CMAKE_C_COMPILER_VERSION VERSION_LESS 9.1 OR NOT STATIC))))
     
     
     
    if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
       
      add_linker_flag_if_supported("-Wl,-pie" LD_SECURITY_FLAGS)
    else()
      add_linker_flag_if_supported("-pie" LD_SECURITY_FLAGS)
    endif()
  endif()
  add_linker_flag_if_supported(-Wl,-z,relro LD_SECURITY_FLAGS)
  add_linker_flag_if_supported(-Wl,-z,now LD_SECURITY_FLAGS)
  add_linker_flag_if_supported(-Wl,-z,noexecstack noexecstack_SUPPORTED)
  if (noexecstack_SUPPORTED)
    set(LD_SECURITY_FLAGS "${LD_SECURITY_FLAGS} -Wl,-z,noexecstack")
  endif()
  add_linker_flag_if_supported(-Wl,-z,noexecheap noexecheap_SUPPORTED)
  if (noexecheap_SUPPORTED)
    set(LD_SECURITY_FLAGS "${LD_SECURITY_FLAGS} -Wl,-z,noexecheap")
  endif()

  if(BACKCOMPAT)
      add_linker_flag_if_supported(-Wl,--wrap=__divmoddi4 LD_BACKCOMPAT_FLAGS)
      add_linker_flag_if_supported(-Wl,--wrap=glob LD_BACKCOMPAT_FLAGS)
      message(STATUS "Using Lib C back compat flags: ${LD_BACKCOMPAT_FLAGS}")
  endif()

   
  if (WIN32)
    add_linker_flag_if_supported(-Wl,--dynamicbase LD_SECURITY_FLAGS)
    add_linker_flag_if_supported(-Wl,--nxcompat LD_SECURITY_FLAGS)
    add_linker_flag_if_supported(-Wl,--high-entropy-va LD_SECURITY_FLAGS)
  endif()

   
   
  add_c_flag_if_supported(  -Werror=switch      C_SECURITY_FLAGS)
  add_cxx_flag_if_supported(-Werror=switch      CXX_SECURITY_FLAGS)
  add_c_flag_if_supported(  -Werror=return-type C_SECURITY_FLAGS)
  add_cxx_flag_if_supported(-Werror=return-type CXX_SECURITY_FLAGS)

  message(STATUS "Using C security hardening flags: ${C_SECURITY_FLAGS}")
  message(STATUS "Using C++ security hardening flags: ${CXX_SECURITY_FLAGS}")
  message(STATUS "Using linker security hardening flags: ${LD_SECURITY_FLAGS}")

  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_GNU_SOURCE ${MINGW_FLAG} ${STATIC_ASSERT_FLAG} ${WARNINGS} ${C_WARNINGS} ${PIC_FLAG} ${C_SECURITY_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GNU_SOURCE ${MINGW_FLAG} ${STATIC_ASSERT_CPP_FLAG} ${WARNINGS} ${CXX_WARNINGS} ${PIC_FLAG} ${CXX_SECURITY_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LD_SECURITY_FLAGS} ${LD_BACKCOMPAT_FLAGS}")

   
   
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-strict-aliasing")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-strict-aliasing")

  if(ARM)
    message(STATUS "Setting FPU Flags for ARM Processors")

     
     

     
     
     
     

     
     
     
     
     

    if(NOT ARM8)
      CHECK_CXX_ACCEPTS_FLAG(-mfpu=vfp3-d16 CXX_ACCEPTS_VFP3_D16)
      CHECK_CXX_ACCEPTS_FLAG(-mfpu=vfp4 CXX_ACCEPTS_VFP4)
      CHECK_CXX_ACCEPTS_FLAG(-mfloat-abi=hard CXX_ACCEPTS_MFLOAT_HARD)
      CHECK_CXX_ACCEPTS_FLAG(-mfloat-abi=softfp CXX_ACCEPTS_MFLOAT_SOFTFP)
    endif()

    if(ARM8)
      CHECK_CXX_ACCEPTS_FLAG(-mfix-cortex-a53-835769 CXX_ACCEPTS_MFIX_CORTEX_A53_835769)
      CHECK_CXX_ACCEPTS_FLAG(-mfix-cortex-a53-843419 CXX_ACCEPTS_MFIX_CORTEX_A53_843419)
    endif()

    if(ARM6)
      message(STATUS "Selecting VFP for ARMv6")
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfpu=vfp")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpu=vfp")
      if(DEPENDS)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -marm")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -marm")
      endif()
    endif(ARM6)

    if(ARM7)
      if(CXX_ACCEPTS_VFP3_D16 AND NOT CXX_ACCEPTS_VFP4)
        message(STATUS "Selecting VFP3 for ARMv7")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfpu=vfp3-d16")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpu=vfp3-d16")
      endif()

      if(CXX_ACCEPTS_VFP4)
        message(STATUS "Selecting VFP4 for ARMv7")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfpu=vfp4")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpu=vfp4")
      endif()

      if(CXX_ACCEPTS_MFLOAT_HARD)
        message(STATUS "Setting Hardware ABI for Floating Point")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfloat-abi=hard")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfloat-abi=hard")
      endif()

      if(CXX_ACCEPTS_MFLOAT_SOFTFP AND NOT CXX_ACCEPTS_MFLOAT_HARD)
        message(STATUS "Setting Software ABI for Floating Point")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfloat-abi=softfp")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfloat-abi=softfp")
      endif()
    endif(ARM7)

    if(ARM8)
      if(CXX_ACCEPTS_MFIX_CORTEX_A53_835769)
        message(STATUS "Enabling Cortex-A53 workaround 835769")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfix-cortex-a53-835769")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfix-cortex-a53-835769")
      endif()

      if(CXX_ACCEPTS_MFIX_CORTEX_A53_843419)
        message(STATUS "Enabling Cortex-A53 workaround 843419")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfix-cortex-a53-843419")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfix-cortex-a53-843419")
      endif()
    endif(ARM8)

  endif(ARM)

   
  if(NOT SANITIZE AND ANDROID AND NOT BUILD_GUI_DEPS STREQUAL "ON" OR IOS)
     
    message(STATUS "Enabling PIE executable")
    set(PIC_FLAG "")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIE")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIE")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_CXX_FLAGS} -fPIE -pie")
  endif()

  if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=default -DGTEST_HAS_TR1_TUPLE=0")
  endif()

  set(DEBUG_FLAGS "-g3")

   
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftemplate-depth=900")

  if(NOT DEFINED USE_LTO_DEFAULT)
    set(USE_LTO_DEFAULT false)
  endif()
  set(USE_LTO ${USE_LTO_DEFAULT} CACHE BOOL "Use Link-Time Optimization (Release mode only)")

  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
     
    set(USE_LTO false)
  endif()


  if(USE_LTO)
    set(RELEASE_FLAGS "${RELEASE_FLAGS} -flto")
    if(STATIC)
      set(RELEASE_FLAGS "${RELEASE_FLAGS} -ffat-lto-objects")
    endif()
     
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND NOT (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9.0) AND NOT OPENBSD AND NOT DRAGONFLY)
       
       
      if (DEFINED CHOST)
        set(CMAKE_AR "${CHOST}-gcc-ar")
        set(CMAKE_RANLIB "${CHOST}-gcc-ranlib")
      else()
        set(CMAKE_AR "gcc-ar")
        set(CMAKE_RANLIB "gcc-ranlib")
      endif()
    endif()
  endif()

  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${DEBUG_FLAGS}")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${DEBUG_FLAGS}")
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${RELEASE_FLAGS}")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${RELEASE_FLAGS}")

  if(STATIC)
     
     
    if (MINGW)
       
       
      set(STATIC_FLAGS "-static")
    elseif (NOT (APPLE OR FREEBSD OR OPENBSD OR DRAGONFLY))
       
      set(STATIC_FLAGS "-static-libgcc -static-libstdc++")
    endif()
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${STATIC_FLAGS} ")
  endif()
endif()

if (${BOOST_IGNORE_SYSTEM_PATHS} STREQUAL "ON")
  set(Boost_NO_SYSTEM_PATHS TRUE)
endif()

set(OLD_LIB_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES})
set(Boost_NO_BOOST_CMAKE ON)
if(STATIC)
  if(MINGW)
    set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
  endif()

  set(Boost_USE_STATIC_LIBS ON)
  if(NOT DEFINED Boost_USE_STATIC_RUNTIME)
    set(Boost_USE_STATIC_RUNTIME ON)
  endif()
endif()
find_package(Boost 1.66 QUIET REQUIRED COMPONENTS system filesystem thread date_time chrono regex serialization program_options locale)
add_definitions(-DBOOST_ASIO_ENABLE_SEQUENTIAL_STRAND_ALLOCATION)

set(CMAKE_FIND_LIBRARY_SUFFIXES ${OLD_LIB_SUFFIXES})
if(NOT Boost_FOUND)
  die("Could not find Boost libraries, please make sure you have installed Boost or libboost-all-dev (>=1.66) or the equivalent")
elseif(Boost_FOUND)
  message(STATUS "Found Boost Version: ${Boost_VERSION}")
  if (Boost_VERSION VERSION_LESS 10 AND Boost_VERSION VERSION_LESS 1.62.0 AND NOT (OPENSSL_VERSION VERSION_LESS 1.1))
    set(BOOST_BEFORE_1_62 true)
  endif()
  if (NOT Boost_VERSION VERSION_LESS 10 AND Boost_VERSION VERSION_LESS 106200 AND NOT (OPENSSL_VERSION VERSION_LESS 1.1))
    set(BOOST_BEFORE_1_62 true)
  endif()
  if (BOOST_BEFORE_1_62)
      message(FATAL_ERROR "Boost ${Boost_VERSION} (older than 1.62) is too old to link with OpenSSL ${OPENSSL_VERSION} (1.1 or newer) found at ${OPENSSL_INCLUDE_DIR} and ${OPENSSL_LIBRARIES}. "
                          "Update Boost or install OpenSSL 1.0 and set path to it when running cmake: "
                          "cmake -DOPENSSL_ROOT_DIR='/usr/include/openssl-1.0'")
  endif()
endif()

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
if(MINGW)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wa,-mbig-obj")
  set(EXTRA_LIBRARIES mswsock;ws2_32;iphlpapi;crypt32;bcrypt)
  if(DEPENDS)
    set(ICU_LIBRARIES icuio icui18n icuuc icudata icutu iconv)
  else()
    set(ICU_LIBRARIES icuio icuin icuuc icudt icutu iconv)
  endif()
elseif(APPLE OR OPENBSD OR ANDROID)
  set(EXTRA_LIBRARIES "")
elseif(FREEBSD)
  set(EXTRA_LIBRARIES execinfo)
elseif(DRAGONFLY)
  find_library(COMPAT compat)
  set(EXTRA_LIBRARIES execinfo ${COMPAT})
elseif(CMAKE_SYSTEM_NAME MATCHES "(SunOS|Solaris)")
  set(EXTRA_LIBRARIES socket nsl resolv)
elseif(NOT MSVC AND NOT DEPENDS)
  find_library(RT rt)
  set(EXTRA_LIBRARIES ${RT})
endif()

list(APPEND EXTRA_LIBRARIES ${CMAKE_DL_LIBS})

if (HIDAPI_FOUND OR LibUSB_COMPILE_TEST_PASSED)
  if (APPLE)
    if(DEPENDS)
      list(APPEND EXTRA_LIBRARIES "-framework Foundation -framework AppKit -framework IOKit")
    else()
      find_library(COREFOUNDATION CoreFoundation)
      find_library(APPKIT AppKit)
      find_library(IOKIT IOKit)
      list(APPEND EXTRA_LIBRARIES ${APPKIT})
      list(APPEND EXTRA_LIBRARIES ${IOKIT})
      list(APPEND EXTRA_LIBRARIES ${COREFOUNDATION})
    endif()
  endif()
  if (WIN32)
    list(APPEND EXTRA_LIBRARIES setupapi)
  endif()
endif()

option(USE_READLINE "Build with GNU readline support." ON)
if(USE_READLINE AND NOT DEPENDS)
  find_package(Readline)
  if(READLINE_FOUND AND GNU_READLINE_FOUND)
    add_definitions(-DHAVE_READLINE)
    include_directories(${Readline_INCLUDE_DIR})
    message(STATUS "Found readline library at: ${Readline_ROOT_DIR}")
    set(EPEE_READLINE epee_readline)
  else()
    message(STATUS "Could not find GNU readline library so building without readline support")
  endif()
elseif(USE_READLINE AND DEPENDS AND NOT MINGW)
  find_path(Readline_INCLUDE_PATH readline/readline.h)
  find_library(Readline_LIBRARY readline)
  find_library(Terminfo_LIBRARY tinfo)
  set(Readline_LIBRARY "${Readline_LIBRARY};${Terminfo_LIBRARY}")
  set(GNU_READLINE_LIBRARY ${Readline_LIBRARY})
  add_definitions(-DHAVE_READLINE)
  set(EPEE_READLINE epee_readline)
endif()

if(ANDROID)
  set(ATOMIC libatomic.a)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=user-defined-warnings")
endif()
if(CMAKE_C_COMPILER_ID MATCHES "Clang" AND ARCH_WIDTH EQUAL "32" AND NOT IOS AND NOT FREEBSD)
  find_library(ATOMIC atomic)
  if (ATOMIC_FOUND)
    list(APPEND EXTRA_LIBRARIES ${ATOMIC})
  endif()
endif()

find_path(ZMQ_INCLUDE_PATH zmq.h)
find_library(ZMQ_LIB zmq)
find_library(PGM_LIBRARY pgm)
find_library(NORM_LIBRARY norm)
find_library(GSSAPI_LIBRARY gssapi_krb5)
find_library(PROTOLIB_LIBRARY protolib)
find_library(SODIUM_LIBRARY sodium)
find_library(BSD_LIBRARY bsd)
find_library(MD_LIBRARY md)
find_library(PROTOKIT_LIBRARY protokit)

if(NOT ZMQ_INCLUDE_PATH)
  message(FATAL_ERROR "Could not find required header zmq.h")
endif()
if(NOT ZMQ_LIB)
  message(FATAL_ERROR "Could not find required libzmq")
endif()
if(PGM_LIBRARY)
  set(ZMQ_LIB "${ZMQ_LIB};${PGM_LIBRARY}")
endif()
if(NORM_LIBRARY)
  set(ZMQ_LIB "${ZMQ_LIB};${NORM_LIBRARY}")
endif()
if(GSSAPI_LIBRARY)
  set(ZMQ_LIB "${ZMQ_LIB};${GSSAPI_LIBRARY}")
endif()
if(PROTOLIB_LIBRARY)
  set(ZMQ_LIB "${ZMQ_LIB};${PROTOLIB_LIBRARY}")
endif()
if(SODIUM_LIBRARY)
  set(ZMQ_LIB "${ZMQ_LIB};${SODIUM_LIBRARY}")
endif()
if(BSD_LIBRARY)
  set(ZMQ_LIB "${ZMQ_LIB};${BSD_LIBRARY}")
endif()
if(MD_LIBRARY)
  set(ZMQ_LIB "${ZMQ_LIB};${MD_LIBRARY}")
endif()
if(PROTOKIT_LIBRARY)
  set(ZMQ_LIB "${ZMQ_LIB};${PROTOKIT_LIBRARY}")
endif()

include(external/supercop/functions.cmake)  
add_subdirectory(contrib)
add_subdirectory(src)

 
