                                       
                                              
  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               
#include <boost/optional/optional.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/variables_map.hpp>

#include "common/command_line.h"

namespace wallet_args
{
  command_line::arg_descriptor<std::string> arg_generate_from_json();
  command_line::arg_descriptor<std::string> arg_wallet_file();
  command_line::arg_descriptor<std::string> arg_rpc_client_secret_key();
  command_line::arg_descriptor<std::string> arg_password_file();

  const char* tr(const char* str);

   
 
 
 

 
 
 
 
  std::pair<boost::optional<boost::program_options::variables_map>, bool> main(
    int argc, char** argv,
    const char* const usage,
    const char* const notice,
    boost::program_options::options_description desc_params,
    const boost::program_options::positional_options_description& positional_options,
    const std::function<void(const std::string&, bool)> &print,
    const char *default_log_name, bool log_to_console = false);
}
