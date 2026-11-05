                                       
                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#include "rpc_version_str.h"
#include "version.h"
#include <regex>

namespace cryptonote
{

namespace rpc
{

 
 
 
 
 

bool is_version_string_valid(const std::string& str)
{
    return std::regex_match(str, std::regex(
        "^\\d{1,2}(\\.\\d{1,2}){3}(-(release|[0-9a-f]{9}))?$",
        std::regex_constants::nosubs
    ));
}

}   

}   
