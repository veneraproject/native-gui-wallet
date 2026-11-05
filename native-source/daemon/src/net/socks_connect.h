                                       
                                              
  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/thread/future.hpp>
#include <string>

namespace net
{
namespace socks
{
     
    struct connector
    {
        boost::asio::ip::tcp::endpoint proxy_address;

         
 
 

 
 
        boost::unique_future<boost::asio::ip::tcp::socket>
            operator()(const std::string& remote_host, const std::string& remote_port, boost::asio::steady_timer& timeout) const;
    };
}  
}  
