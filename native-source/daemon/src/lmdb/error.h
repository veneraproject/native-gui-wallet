                                       
                                              
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               
#pragma once

#include <system_error>
#include <type_traits>

 
#define MONERO_LMDB_CHECK(...)         \
    do                                 \
    {                                  \
        const int err = __VA_ARGS__ ;  \
        if (err)                       \
            return {lmdb::error(err)}; \
    } while (0)

namespace lmdb
{
     
    enum class error : int
    {
         
         
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
    struct is_error_code_enum<::lmdb::error>
      : true_type
    {};
}
