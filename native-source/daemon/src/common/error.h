                                       
                                              

                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               
#pragma once

#include <system_error>
#include <type_traits>

enum class common_error : int
{
     
    kInvalidArgument = 1,  
    kInvalidErrorCode      
};

std::error_category const& common_category() noexcept;

inline std::error_code make_error_code(::common_error value) noexcept
{
    return std::error_code{int(value), common_category()};
}

namespace std
{
    template<>
    struct is_error_code_enum<::common_error>
      : true_type
    {};
}
