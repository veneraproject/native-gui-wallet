                                       
                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#pragma once

#include "crypto/crypto.h"
#include "serialization/containers.h"
#include "serialization/crypto.h"
#include "serialization/serialization.h"

#include <cstdint>
#include <vector>


namespace multisig
{
   
  struct multisig_kex_msg_serializable_round1
  {
     
    crypto::secret_key msg_privkey;
     
    crypto::public_key signing_pubkey;
     
    crypto::signature signature;

    BEGIN_SERIALIZE()
      FIELD(msg_privkey)
      FIELD(signing_pubkey)
      FIELD(signature)
    END_SERIALIZE()
  };

   
  struct multisig_kex_msg_serializable_general
  {
     
    std::uint32_t kex_round;
     
    std::vector<crypto::public_key> msg_pubkeys;
     
    crypto::public_key signing_pubkey;
     
    crypto::signature signature;

    BEGIN_SERIALIZE()
      VARINT_FIELD(kex_round)
      FIELD(msg_pubkeys)
      FIELD(signing_pubkey)
      FIELD(signature)
    END_SERIALIZE()
  };
}  
