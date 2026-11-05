                                       
                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#pragma once

#include <stdint.h>
#include <string>
#include "crypto/crypto.h"

namespace cryptonote
{
  std::string make_rpc_payment_signature(const crypto::secret_key &skey);
  bool verify_rpc_payment_signature(const std::string &message, crypto::public_key &pkey, uint64_t &ts);
}
