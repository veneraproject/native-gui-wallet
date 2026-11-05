                                       
                                              

  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <boost/utility/string_ref.hpp>
#include <cstdint>

#include "common/expect.h"
#include "net/net_utils_base.h"

namespace net
{
     
 
 
 
 
 
 
 
 
 
    void get_network_address_host_and_port(const std::string& address, std::string& host, std::string& port);

     
 
 
 

 
 
 
 

 
 
    expect<epee::net_utils::network_address>
        get_network_address(boost::string_ref address, std::uint16_t default_port);

     
 
 
 

 
 

 
 
    expect<epee::net_utils::ipv4_network_subnet>
        get_ipv4_subnet_address(boost::string_ref address, bool allow_implicit_32 = false);

    expect<boost::asio::ip::tcp::endpoint> get_tcp_endpoint(const boost::string_ref address);
}

