                                       
                                              

  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#pragma once

#include <cstdint>

namespace net
{
    enum class error : int;
    class tor_address;
    class i2p_address;

    namespace socks
    {
        class client;
        template<typename> class connect_handler;
        enum class error : int;
        enum class version : std::uint8_t;
    }
}
