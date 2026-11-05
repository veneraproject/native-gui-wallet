                                       
                                              

  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#pragma once

#include <boost/utility/string_ref.hpp>
#include <cstdint>
#include <string>

#include "common/expect.h"
#include "net/enums.h"
#include "net/error.h"

namespace epee
{
namespace serialization
{
    class portable_storage;
    struct section;
}
}

namespace net
{
     
    class tor_address
    {
        std::uint16_t port_;
        char host_[63];  

         
        tor_address(boost::string_ref host, std::uint16_t port) noexcept;

    public:
         
        static constexpr std::size_t buffer_size() noexcept { return sizeof(host_); }

         
        static const char* unknown_str() noexcept;

         
        tor_address() noexcept;

         
        static tor_address unknown() noexcept { return tor_address{}; }

         
 
 
 
 
        static expect<tor_address> make(boost::string_ref address, std::uint16_t default_port = 0);

         
        bool _load(epee::serialization::portable_storage& src, epee::serialization::section* hparent);

         
        bool store(epee::serialization::portable_storage& dest, epee::serialization::section* hparent) const;

         

        tor_address(const tor_address& rhs) noexcept;
        ~tor_address() = default;
        tor_address& operator=(const tor_address& rhs) noexcept;

         
        bool is_unknown() const noexcept;

        bool equal(const tor_address& rhs) const noexcept;
        bool less(const tor_address& rhs) const noexcept;

         
        bool is_same_host(const tor_address& rhs) const noexcept;

         
        std::string str() const;

         
        const char* host_str() const noexcept { return host_; }

         
        std::uint16_t port() const noexcept { return port_; }

        static constexpr bool is_loopback() noexcept { return false; }
        static constexpr bool is_local() noexcept { return false; }

        static constexpr epee::net_utils::address_type get_type_id() noexcept
        {
            return epee::net_utils::address_type::tor;
        }

        static constexpr epee::net_utils::zone get_zone() noexcept
        {
            return epee::net_utils::zone::tor;
        }

         
        bool is_blockable() const noexcept { return !is_unknown(); }
    };

    inline bool operator==(const tor_address& lhs, const tor_address& rhs) noexcept
    {
        return lhs.equal(rhs);
    }

    inline bool operator!=(const tor_address& lhs, const tor_address& rhs) noexcept
    {
        return !lhs.equal(rhs);
    }

    inline bool operator<(const tor_address& lhs, const tor_address& rhs) noexcept
    {
        return lhs.less(rhs);
    }
}  
