                                       
                                              

  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#include <string>
#include <vector>

template<typename> class expect;

namespace net
{
namespace dnssec
{
  struct service_response
  {
    std::vector<std::string> ip;    
    std::vector<std::string> tlsa;  
  };

   
  expect<service_response> resolve_hostname(const std::string& addr, const std::string& tlsa_port = {});
}  
}  
