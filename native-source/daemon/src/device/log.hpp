                                       
                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               
  

#pragma once

#include <cstddef>
#include <string>

#include "ringct/rctOps.h"
#include "crypto/crypto.h"
#include "cryptonote_basic/account.h"

#include "device.hpp"

namespace hw {

     
 
 
 
 
 
 
 
 
 
 
 

    void buffer_to_str(char *to_buff,  size_t to_len, const char *buff, size_t len) ;
    void log_hexbuffer(const std::string &msg,  const char* buff, size_t len);
    void log_message(const std::string &msg, const std::string &info );

    #ifdef WITH_DEVICE_LEDGER    
    namespace ledger {

        #ifdef DEBUG_HWDEVICE
        #define TRACK printf("file %s:%d\n",__FILE__, __LINE__)
         
         

        void decrypt(char* buf, size_t len) ;
        crypto::key_derivation decrypt(const crypto::key_derivation &derivation) ;
        cryptonote::account_keys decrypt(const cryptonote::account_keys& keys) ;
        crypto::secret_key decrypt(const crypto::secret_key &sec) ;
        rct::key  decrypt(const rct::key &sec);
        crypto::ec_scalar decrypt(const crypto::ec_scalar &res);
        rct::keyV decrypt(const rct::keyV &keys);

        void check32(const std::string &msg, const std::string &info, const char *h, const char *d, bool crypted=false);
        void check8(const std::string &msg, const std::string &info, const char *h, const char *d,  bool crypted=false);
        void check1(const std::string &msg, const std::string &info, const char *h, const char *d,  bool crypted=false);

        void set_check_verbose(bool verbose);
        #endif
    }
    #endif
}
