                                       
                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#include <boost/algorithm/string.hpp>
#include "misc_log_ex.h"
#include "util.h"
#include "dns_utils.h"
#include "updates.h"

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "updates"

namespace tools
{
  bool check_updates(const std::string &, const std::string &, std::string &, std::string &)
  {
    return false;  
  }
  std::string get_update_url(const std::string &, const std::string &, const std::string &,
                             const std::string &, bool)
  {
    return {};
  }
}
