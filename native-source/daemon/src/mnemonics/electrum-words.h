                                       
                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

 
 
 
 
 
 
 
 
 

#ifndef ELECTRUM_WORDS_H
#define ELECTRUM_WORDS_H

#include <string>
#include <cstdint>
#include "crypto/crypto.h"   

namespace epee {  class wipeable_string; }

 
 
 
 
 
namespace crypto
{
   
 
 
 
 
  namespace ElectrumWords
  {

    const int seed_length = 24;
    const std::string old_language_name = "EnglishOld";
     
 
 
 
 
 
 
 
 
    bool words_to_bytes(const epee::wipeable_string &words, epee::wipeable_string& dst, size_t len, bool duplicate,
      std::string &language_name);
     
 
 
 
 
 
 
    bool words_to_bytes(const epee::wipeable_string &words, crypto::secret_key& dst,
      std::string &language_name);

     
 
 
 
 
 
 
 
    bool bytes_to_words(const char *src, size_t len, epee::wipeable_string& words,
      const std::string &language_name);

     
 
 
 
 
 
 
    bool bytes_to_words(const crypto::secret_key& src, epee::wipeable_string& words,
      const std::string &language_name);

     
 
 
 
 
    void get_language_list(std::vector<std::string> &languages, bool english = false);

     
 
 
 
 
    bool get_is_old_style_seed(const epee::wipeable_string &seed);

     
 
 
 
 
    std::string get_english_name_for(const std::string &name);

    bool is_valid_language(const std::string &language);
  }
}

#endif
