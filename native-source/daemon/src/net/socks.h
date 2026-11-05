                                       
                                              
  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#pragma once

#include <cstdint>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/strand.hpp>
#include <boost/system/error_code.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/utility/string_ref.hpp>
#include <memory>
#include <utility>

#include "net/fwd.h"
#include "span.h"

namespace epee
{
namespace net_utils
{
    class ipv4_network_address;
}
}

namespace net
{
namespace socks
{
     
    enum class version : std::uint8_t
    {
        v4 = 0,
        v4a,
        v4a_tor   
    };

     
    enum class error : int
    {
         
         
        rejected = 92,
        identd_connection,
        identd_user,
         
        bad_read = 257,
        bad_write,
        unexpected_version
    };

     
 
 
 

     
    const boost::system::error_category& error_category() noexcept;

     
    inline boost::system::error_code make_error_code(error value) noexcept
    {
        return boost::system::error_code{int(value), socks::error_category()};
    }

     
    class client
    {
        boost::asio::ip::tcp::socket proxy_;
        boost::asio::io_service::strand strand_;
        std::uint16_t buffer_size_;
        std::uint8_t buffer_[1024];
        socks::version ver_;

         
 
 
 
 
 
 

 
 
 
        virtual void done(boost::system::error_code error, std::shared_ptr<client> self) = 0;

    public:
        using stream_type = boost::asio::ip::tcp;

         
        struct write;
        struct read;
        struct completed;

         
 
 
 
 
        explicit client(stream_type::socket&& proxy, socks::version ver);

        client(const client&) = delete;
        virtual ~client();
        client& operator=(const client&) = delete;

         
        stream_type::socket take_socket()
        {
            return stream_type::socket{std::move(proxy_)};
        }

         
        socks::version socks_version() const noexcept { return ver_; }

         
        epee::span<const std::uint8_t> buffer() const noexcept
        {
            return {buffer_, buffer_size_};
        }

         
        void clear_command() noexcept { buffer_size_ = 0; }

         
        bool set_connect_command(const epee::net_utils::ipv4_network_address& address);

         
        bool set_connect_command(boost::string_ref domain, std::uint16_t port);

         
        bool set_connect_command(const net::tor_address& address);

         
        bool set_connect_command(const net::i2p_address& address);

         
        bool set_resolve_command(boost::string_ref domain);

         
 
 
 

 
 
 
 

 
 
 
 
        static bool connect_and_send(std::shared_ptr<client> self, const stream_type::endpoint& proxy_address);

         
 
 
 

 
 
 
 

 
 
 
        static bool send(std::shared_ptr<client> self);

         
 
        struct async_close
        {
            std::shared_ptr<client> self_;
            void operator()(boost::system::error_code error = boost::system::error_code{});
        };

         
        struct close_on_exit
        {
            std::shared_ptr<client> self;
            ~close_on_exit() { async_close{std::move(self)}(); }
        };
    };

    template<typename Handler>
    class connect_client : public client
    {
        Handler handler_;

        virtual void done(boost::system::error_code error, std::shared_ptr<client>) override
        {
            handler_(error, take_socket());
        }

    public:
        explicit connect_client(stream_type::socket&& proxy, socks::version ver, Handler&& handler)
          : client(std::move(proxy), ver), handler_(std::move(handler))
        {}

        virtual ~connect_client() override {}
    };

    template<typename Handler>
    inline std::shared_ptr<client>
    make_connect_client(client::stream_type::socket&& proxy, socks::version ver, Handler handler)
    {
        return std::make_shared<connect_client<Handler>>(std::move(proxy), ver, std::move(handler));
    }
}  
}  

namespace boost
{
namespace system
{
    template<>
    struct is_error_code_enum<net::socks::error>
      : true_type
    {};
}  
}  
