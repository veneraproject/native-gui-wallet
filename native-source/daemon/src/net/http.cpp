                                       
                                              

  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#include "http.h"

#include "parse.h"
#include "socks_connect.h"

namespace net
{
namespace http
{

bool client::set_proxy(const std::string &address)
{
  if (address.empty())
  {
    set_connector(epee::net_utils::direct_connect{});
  }
  else
  {
    const auto endpoint = get_tcp_endpoint(address);
    if (!endpoint)
    {
      auto always_fail = net::socks::connector{boost::asio::ip::tcp::endpoint()};
      set_connector(always_fail);
    }
    else
    {
      set_connector(net::socks::connector{*endpoint});
    }
  }

  disconnect();

  return true;
}

std::unique_ptr<epee::net_utils::http::abstract_http_client> client_factory::create()
{
  return std::unique_ptr<epee::net_utils::http::abstract_http_client>(new client());
}

}  
}  
