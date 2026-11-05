                                      
                                             
  
                      
  
                                                                                      
                                                           
  
                                                                                        
                                             
  
                                                                                        
                                                                                 
                                              
  
                                                                                      
                                                                                    
                              
  
                                                                                     
                                                                                         
                                                                                        
                                                                                      
                                                                              
                                                                                         
                                                                                   
                                                                                         
                                                                              
  
                                                                                     

 

function (get_version_tag_from_git GIT)
    execute_process(COMMAND "${GIT}" rev-parse --short=9 HEAD
                    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
                    RESULT_VARIABLE RET
                    OUTPUT_VARIABLE COMMIT
                    OUTPUT_STRIP_TRAILING_WHITESPACE)

    if(RET)
         

        message(WARNING "Cannot determine current commit. Make sure that you are building either from a Git working tree or from a source archive.")
        set(VERSIONTAG "unknown")
        set(VERSION_IS_RELEASE "false")
    else()
        string(SUBSTRING ${COMMIT} 0 9 COMMIT)
        message(STATUS "You are currently on commit ${COMMIT}")

         
        execute_process(COMMAND "${GIT}" tag -l --points-at HEAD
                        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
                        RESULT_VARIABLE RET
                        OUTPUT_VARIABLE TAG
                        OUTPUT_STRIP_TRAILING_WHITESPACE)

         
        if(TAG)
            message(STATUS "You are building a tagged release")
            set(VERSIONTAG "release")
            set(VERSION_IS_RELEASE "true")
        else()
            message(STATUS "You are ahead of or behind a tagged release")
            set(VERSIONTAG "${COMMIT}")
            set(VERSION_IS_RELEASE "false")
        endif()	    
    endif()

    set(VERSIONTAG "${VERSIONTAG}" PARENT_SCOPE)
    set(VERSION_IS_RELEASE "${VERSION_IS_RELEASE}" PARENT_SCOPE)
endfunction()
