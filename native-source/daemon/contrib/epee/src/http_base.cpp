                                                                    
                       
   
                                                                     
                                                                              
                                                                   
                                                                
                                                                      
                                                                      
                                                                       
                                                         
                                                                       
                                                                        
   
                                                                                  
                                                                                
                                                                         
                                                                       
                                                                             
                                                                               
                                                                              
                                                                             
                                                                                
                                                               
   

#include "net/http_base.h"
#include "memwipe.h"
#include "string_tools.h"

#include <boost/regex.hpp>
#include <string>
#include <utility>

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "net.http"

namespace epee
{
namespace net_utils
{
namespace http
{
    std::string get_value_from_fields_list(const std::string& param_name, const net_utils::http::fields_list& fields)
    {
        fields_list::const_iterator it = fields.begin();
        for(; it != fields.end(); ++it)
            if(!string_tools::compare_no_case(param_name, it->first))
                break;

        if(it==fields.end())
            return std::string();

        return it->second;
    }

    std::string get_value_from_uri_line(const std::string& param_name, const std::string& uri)
    {
        std::string buff = "([\\?|&])";
        buff += param_name + "=([^&]*)";
        boost::regex match_param(buff.c_str(), boost::regex::icase | boost::regex::normal);
        boost::smatch	result;
        if(boost::regex_search(uri, result, match_param, boost::match_default) && result[0].matched) 
        {
            return result[2];
        }
        return std::string();
    }
}
}
}
