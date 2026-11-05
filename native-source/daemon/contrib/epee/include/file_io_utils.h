                                                                    
                       
   
                                                                     
                                                                              
                                                                   
                                                                
                                                                      
                                                                      
                                                                       
                                                         
                                                                       
                                                                        
   
                                                                                  
                                                                                
                                                                         
                                                                       
                                                                             
                                                                               
                                                                              
                                                                             
                                                                                
                                                               
   

#ifndef _FILE_IO_UTILS_H_
#define _FILE_IO_UTILS_H_

#include <string>
#include <ctime>
#include <cstdint>

namespace epee
{
namespace file_io_utils
{
    bool is_file_exist(const std::string& path);
    bool save_string_to_file(const std::string& path_to_file, const std::string& str);
    bool load_file_to_string(const std::string& path_to_file, std::string& target_str, size_t max_size = 1000000000);
    bool get_file_size(const std::string& path_to_file, uint64_t &size);
}
}

#endif  
