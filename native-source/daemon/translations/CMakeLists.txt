                                      
                                             
 
                      
 
                                                                                      
                                                           
 
                                                                                        
                                             
 
                                                                                        
                                                                                 
                                              
 
                                                                                      
                                                                                    
                              
 
                                                                                     
                                                                                         
                                                                                        
                                                                                      
                                                                              
                                                                                         
                                                                                   
                                                                                         
                                                                              

cmake_minimum_required(VERSION 3.5)

project(translations)

 
IF(CMAKE_CROSSCOMPILING)
  message(WARNING "CrossCompiling")
  SET(IMPORT_EXECUTABLES "${CMAKE_CURRENT_BINARY_DIR}/ImportExecutables.cmake" CACHE FILEPATH "Point it to the export file from a native build")
  INCLUDE(${IMPORT_EXECUTABLES})
ENDIF(CMAKE_CROSSCOMPILING)

 
IF(NOT CMAKE_CROSSCOMPILING)
  add_executable(generate_translations_header generate_translations_header.c)
ENDIF(NOT CMAKE_CROSSCOMPILING)

if(LRELEASE_PATH STREQUAL "")
  find_program(LRELEASE lrelease)
else()
  set(LRELEASE ${LRELEASE_PATH}/lrelease)
endif()

if(LRELEASE STREQUAL "LRELEASE-NOTFOUND")
  set(ts_files "")
  message(WARNING "lrelease program not found, translation files not built")
else()
  execute_process(COMMAND ${LRELEASE} -version
    RESULT_VARIABLE lrelease_ret)
  if(NOT lrelease_ret EQUAL "0")
    set(ts_files "")
    message(WARNING "lrelease program not working, translation files not built")
  else()
    file(GLOB ts_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" *.ts)
    foreach(ts_file ${ts_files})
      string(REPLACE ".ts" ".qm" qm_file "${ts_file}")
      add_custom_command(TARGET generate_translations_header
        PRE_BUILD
        COMMAND ${LRELEASE} "${CMAKE_CURRENT_SOURCE_DIR}/${ts_file}" -qm "${qm_file}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_BIN_DIR}")
    endforeach()
  endif()
endif()

string(REPLACE ".ts" ".qm" qm_files "${ts_files}")

add_custom_command(TARGET generate_translations_header
  POST_BUILD
  COMMAND $<TARGET_FILE:generate_translations_header> ${qm_files}
  WORKING_DIRECTORY "${CMAKE_CURRENT_BIN_DIR}"
  COMMENT "Generating embedded translations header")

 
IF(NOT CMAKE_CROSSCOMPILING)
  EXPORT(TARGETS generate_translations_header FILE ${CMAKE_CURRENT_BINARY_DIR}/ImportExecutables.cmake )
ENDIF(NOT CMAKE_CROSSCOMPILING)
