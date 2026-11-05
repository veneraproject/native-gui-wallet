                                       
                                         
  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#include "net/http_protocol_handler.h"

namespace epee { namespace net_utils { namespace http
{
	std::string get_rpc_connection_limit_key(const net_utils::network_address& address)
	{
		if (address.get_type_id() == net_utils::ipv6_network_address::get_type_id())
		{
			const boost::asio::ip::address_v6 ip = address.as<const net_utils::ipv6_network_address>().ip();
			if (net_utils::should_group_ipv6_by_prefix(ip))
				return net_utils::get_ipv6_subnet_address(ip, 64).to_string() + "/64";
		}

		return address.host_str();
	}
}}}
