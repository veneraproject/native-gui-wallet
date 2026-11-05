                                       
                                              

  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#pragma once

#include <system_error>
#include <type_traits>

namespace net
{
     
    enum class error : int
    {
         
        bogus_dnssec = 1,    
        dns_query_failure,   
        expected_tld,        
        invalid_host,        
        invalid_i2p_address,
        invalid_mask,        
        invalid_port,        
        invalid_tor_address, 
        unsupported_address, 

    };

     
    std::error_category const& error_category() noexcept;

     
    inline std::error_code make_error_code(error value) noexcept
    {
        return std::error_code{int(value), error_category()};
    }
}

namespace std
{
    template<>
    struct is_error_code_enum<::net::error>
      : true_type
    {};
}
