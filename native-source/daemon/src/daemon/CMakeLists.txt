                                      
                                             
 
                      
 
                                                                                      
                                                           
 
                                                                                        
                                             
 
                                                                                        
                                                                                 
                                              
 
                                                                                      
                                                                                    
                              
 
                                                                                     
                                                                                         
                                                                                        
                                                                                      
                                                                              
                                                                                         
                                                                                   
                                                                                         
                                                                              

set(daemon_sources
  command_parser_executor.cpp
  command_server.cpp
  daemon.cpp
  executor.cpp
  main.cpp
  rpc_command_executor.cpp
)

set(daemon_headers)

monero_find_all_headers(daemon_private_headers "${CMAKE_CURRENT_SOURCE_DIR}")

monero_private_headers(daemon
  ${daemon_private_headers})
monero_add_executable(daemon
  ${daemon_sources}
  ${daemon_headers}
  ${daemon_private_headers})
target_link_libraries(daemon
  PRIVATE
    rpc
    blockchain_db
    cryptonote_core
    cncrypto
    common
    p2p
    cryptonote_protocol
    daemonizer
    serialization
    daemon_rpc_server
    ${EPEE_READLINE}
    version
    ${Boost_CHRONO_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_REGEX_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${ZMQ_LIB}
    ${GNU_READLINE_LIBRARY}
    ${EXTRA_LIBRARIES}
    ${Blocks})
set_property(TARGET daemon
  PROPERTY
    OUTPUT_NAME "venerad")
install(TARGETS daemon DESTINATION bin)
