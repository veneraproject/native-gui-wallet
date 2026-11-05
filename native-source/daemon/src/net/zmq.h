                                       
                                              
  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#pragma once

#include <memory>
#include <string>
#include <system_error>
#include <zmq.h>

#include "common/expect.h"
#include "span.h"

 
#define MONERO_ZMQ_CHECK(...)                      \
    do                                             \
    {                                              \
        if (( __VA_ARGS__ ) < 0)                   \
            return {::net::zmq::get_error_code()}; \
    } while (0)

 
#define MONERO_LOG_ZMQ_ERROR(...)                                                   \
    do                                                                          \
    {                                                                           \
        MERROR( __VA_ARGS__ << ": " << ::net::zmq::get_error_code().message()); \
    } while (0)

 
#define MONERO_ZMQ_THROW(msg)                         \
    MONERO_THROW( ::net::zmq::get_error_code(), msg )

namespace epee
{
    class byte_slice;
}

namespace net
{
namespace zmq
{
    constexpr std::size_t max_message_size = 10 * 1024 * 1024;  

     
    const std::error_category& error_category() noexcept;

     
    inline std::error_code make_error_code(int code) noexcept
    {
        return std::error_code{code, error_category()};
    }

     
    inline std::error_code get_error_code() noexcept
    {
        return make_error_code(zmq_errno());
    }

     
    class terminate
    {
        static void call(void* ptr) noexcept;
    public:
        void operator()(void* ptr) const noexcept
        {
            if (ptr)
                call(ptr);
        }
    };

     
    struct close
    {
        void operator()(void* ptr) const noexcept
        {
            if (ptr)
                zmq_close(ptr);
        }
    };

     
    using context = std::unique_ptr<void, terminate>;

     
    using socket = std::unique_ptr<void, close>;

   
 

 
 
 
    template<typename F, typename... T>
    expect<void> retry_op(F op, T&&... args) noexcept(noexcept(op(args...)))
    {
      for (;;)
      {
        if (0 <= op(args...))
          return success();

        const int error = zmq_errno();
        if (error != EINTR)
          return make_error_code(error);
      }
    }

     
 
 
 

 
 
 
 
 

 
 
 
    expect<std::string> receive(void* socket, int flags = 0);

     
 
 
 

 
 
 
 
 

 
 
 
 
    expect<void> send(epee::span<const std::uint8_t> payload, void* socket, int flags = 0) noexcept;

     
 
 
 

 
 
 
 
 

 
 
 

 
 
    expect<void> send(epee::byte_slice&& payload, void* socket, int flags = 0) noexcept;
}  
}  
