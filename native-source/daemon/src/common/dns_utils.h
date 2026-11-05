                                       
                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               
#pragma once

#include <vector>
#include <string>
#include <functional>
#include <boost/optional/optional_fwd.hpp>
#include <boost/utility/string_ref_fwd.hpp>

namespace tools
{

 
constexpr const int DNS_CLASS_IN  = 1;
constexpr const int DNS_TYPE_A    = 1;
constexpr const int DNS_TYPE_TXT  = 16;
constexpr const int DNS_TYPE_AAAA = 8;
constexpr const int DNS_TYPE_TLSA = 52;

struct DNSResolverData;

 
 
 
 
 
 
 
class DNSResolver
{
private:

   
 
 
 
 
  DNSResolver();

public:

   
 
 
  ~DNSResolver();

   
 
 
 
 
 
 
 
 
 
 
 
  std::vector<std::string> get_ipv4(const std::string& url, bool& dnssec_available, bool& dnssec_valid);

   
 
 
 
 
 
 
 
 
 
   std::vector<std::string> get_ipv6(const std::string& url, bool& dnssec_available, bool& dnssec_valid);

   
 
 
 
 
 
 
 
   
   std::vector<std::string> get_txt_record(const std::string& url, bool& dnssec_available, bool& dnssec_valid);

   
 
 
 
 
 
 
 
 
  std::vector<std::string> get_tlsa_tcp_record(boost::string_ref url, boost::string_ref port, bool& dnssec_available, bool& dnssec_valid);

   
 
 
 
 
 
 
 
 
 
  std::string get_dns_format_from_oa_address(const std::string& oa_addr);

   
 
 
 
 
  static DNSResolver& instance();

   
 
 
 
 
  static DNSResolver create();

private:

   
 
 
 
 
 
 
 
 
 
   
  std::vector<std::string> get_record(const std::string& url, int record_type, boost::optional<std::string> (*reader)(const char *,size_t), bool& dnssec_available, bool& dnssec_valid);

  DNSResolverData *m_data;
};  

namespace dns_utils
{

std::string address_from_txt_record(const std::string& s);
std::vector<std::string> addresses_from_url(const std::string& url, bool& dnssec_valid);

std::string get_account_address_as_str_from_url(const std::string& url, bool& dnssec_valid, std::function<std::string(const std::string&, const std::vector<std::string>&, bool)> confirm_dns);

bool load_txt_records_from_dns(std::vector<std::string> &records, const std::vector<std::string> &dns_urls);

std::vector<std::string> parse_dns_public(const char *s);

}   

}   
