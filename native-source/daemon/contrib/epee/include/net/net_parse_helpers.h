                                                                    
                       
   
                                                                     
                                                                              
                                                                   
                                                                
                                                                      
                                                                      
                                                                       
                                                         
                                                                       
                                                                        
   
                                                                                  
                                                                                
                                                                         
                                                                       
                                                                             
                                                                               
                                                                              
                                                                             
                                                                                
                                                               
   

#pragma once 

#include "http_base.h"

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "net"

namespace epee
{
namespace net_utils
{
    bool parse_uri(const std::string uri, http::uri_content& content);
    bool parse_url_ipv6(const std::string url_str, http::url_content& content);
    bool parse_url(const std::string url_str, http::url_content& content);
}
}
