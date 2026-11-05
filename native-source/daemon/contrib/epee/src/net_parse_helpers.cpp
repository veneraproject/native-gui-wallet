                                                                    
                       
   
                                                                     
                                                                              
                                                                   
                                                                
                                                                      
                                                                      
                                                                       
                                                         
                                                                       
                                                                        
   
                                                                                  
                                                                                
                                                                         
                                                                       
                                                                             
                                                                               
                                                                              
                                                                             
                                                                                
                                                               
   

#include "net/net_parse_helpers.h"
#include "net/http_base.h"
#include "misc_log_ex.h"
#include "reg_exp_definer.h"
#include <boost/lexical_cast.hpp>

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "net"

namespace epee
{
namespace net_utils
{

  static bool parse_uri_query(const std::string& query, std::list<std::pair<std::string, std::string> >& params)
  { 
    enum state
    {
      st_param_name, 
      st_param_val
    };
    state st = st_param_name;
    std::string::const_iterator start_it = query.begin();
    std::pair<std::string, std::string> e;
    for(std::string::const_iterator it = query.begin(); it != query.end(); ++it)
    {
      switch(st)
      {
      case st_param_name:
        if(*it == '=')
        {
          e.first.assign(start_it, it);
          start_it = it;++start_it;
          st = st_param_val;
        }
        break;
      case st_param_val:
        if(*it == '&')
        {
          e.second.assign(start_it, it);
          start_it = it;++start_it;
          params.push_back(e);
          e.first.clear();e.second.clear();
          st = st_param_name;
        }
        break;
      default:
        LOG_ERROR("Unknown state " << (int)st);
        return false;
      }
    }
    if(st == st_param_name)
    {
      if(start_it != query.end())
      {
        e.first.assign(start_it, query.end());
        params.push_back(e);
      }
    }else
    {
      if(start_it != query.end())
        e.second.assign(start_it, query.end());

      if(e.first.size())
        params.push_back(e);
    }
    return true;
  }
  
  bool parse_uri(const std::string uri, http::uri_content& content)
  {

     
    content.m_query_params.clear();
    STATIC_REGEXP_EXPR_1(rexp_match_uri, "^([^?#]*)(\\?([^#]*))?(#(.*))?", boost::regex::icase | boost::regex::normal);

    boost::smatch result;	
    if(!(boost::regex_search(uri, result, rexp_match_uri, boost::match_default) && result[0].matched))
    {
      LOG_PRINT_L1("[PARSE URI] regex not matched for uri: " << uri);
      content.m_path = uri;
      return true;
    }
    if(result[1].matched)
    {
      content.m_path = result[1];
    }
    if(result[3].matched)
    {
      content.m_query = result[3];
    }
    if(result[5].matched)
    {
      content.m_fragment = result[5];
    }
    if(content.m_query.size())
    {
      parse_uri_query(content.m_query, content.m_query_params);
    }
    return true;
  }

  bool parse_url_ipv6(const std::string url_str, http::url_content& content)
  {
    STATIC_REGEXP_EXPR_1(rexp_match_uri, "^(([^:]*?)://)?(\\[(.*)\\](:(\\d+))?)(.*)?", boost::regex::icase | boost::regex::normal);
     

    content.port = 0;
    boost::smatch result;
    if(!(boost::regex_search(url_str, result, rexp_match_uri, boost::match_default) && result[0].matched))
    {
      LOG_PRINT_L1("[PARSE URI] regex not matched for uri: " << rexp_match_uri);
       
      return false;
    }
    if(result[2].matched)
    {
      content.schema = result[2];
    }
    if(result[4].matched)
    {
      content.host = result[4];
    }
    else   
    {
      return false;
    }
    if(result[6].matched)
    {
      content.port = boost::lexical_cast<uint64_t>(result[6]);
    }
    if(result[7].matched)
    {
      content.uri = result[7];
      return parse_uri(result[7], content.m_uri_content);
    }

    return true;
  }

  bool parse_url(const std::string url_str, http::url_content& content)
  {

    if (parse_url_ipv6(url_str, content)) return true;

     
     
    STATIC_REGEXP_EXPR_1(rexp_match_uri, "^(([^:]*?)://)?(([^/:]*)(:(\\d+))?)(.*)?", boost::regex::icase | boost::regex::normal);
     
    content.port = 0;
    boost::smatch result;	
    if(!(boost::regex_search(url_str, result, rexp_match_uri, boost::match_default) && result[0].matched))
    {
      LOG_PRINT_L1("[PARSE URI] regex not matched for uri: " << rexp_match_uri);
       
      return true;
    }
    if(result[2].matched)
    {
      content.schema = result[2];
    }
    if(result[4].matched)
    {
      content.host = result[4];
    }
    if(result[6].matched)
    {
      content.port = boost::lexical_cast<uint64_t>(result[6]);
    }
    if(result[7].matched)
    {
      content.uri = result[7];
      return parse_uri(result[7], content.m_uri_content);
    }
    
    return true;
  }

}
}
