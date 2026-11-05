                                       
                                              
  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#pragma once

#include <cstdint>

namespace cryptonote
{
   
   
  enum transaction_type : std::uint8_t
  {
    UNSET        = 0,
    MINER        = 1,
    PROTOCOL     = 2,
    TRANSFER     = 3,
    CONVERT      = 4,
    BURN         = 5,
    STAKE        = 6,
    RETURN       = 7,
    AUDIT        = 8,
    CREATE_TOKEN = 9,
    ROLLUP       = 10
  };

   
  enum class relay_method : std::uint8_t
  {
    none = 0,  
    local,     
    forward,   
    stem,      
    fluff,     
    block      
  };
}
