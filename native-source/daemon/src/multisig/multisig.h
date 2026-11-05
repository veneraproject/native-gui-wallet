                                       
                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#pragma once

#include "crypto/crypto.h"
#include "cryptonote_basic/cryptonote_format_utils.h"
#include "ringct/rctTypes.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace cryptonote { struct account_keys; }

namespace multisig
{
   
 
 
 
 
 
 
 
  crypto::secret_key get_multisig_blinded_secret_key(const crypto::secret_key &key);

  bool generate_multisig_key_image(const cryptonote::account_keys &keys,
    std::size_t multisig_key_index,
    const crypto::public_key& out_key,
    crypto::key_image& ki);
  void generate_multisig_LR(const crypto::public_key pkey,
    const crypto::secret_key &k,
    crypto::public_key &L,
    crypto::public_key &R);
  bool generate_multisig_composite_key_image(const cryptonote::account_keys &keys,
    const std::unordered_map<crypto::public_key, cryptonote::subaddress_index> &subaddresses,
    const crypto::public_key &out_key,
    const crypto::public_key &tx_public_key,
    const std::vector<crypto::public_key> &additional_tx_public_keys,
    std::size_t real_output_index,
    const std::vector<crypto::key_image> &pkis,
    crypto::key_image &ki);
}  
