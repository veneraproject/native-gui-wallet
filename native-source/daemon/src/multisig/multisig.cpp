                                       
                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#include "crypto/crypto.h"
#include "cryptonote_basic/account.h"
#include "cryptonote_basic/cryptonote_format_utils.h"
#include "cryptonote_config.h"
#include "include_base_utils.h"
#include "multisig.h"
#include "ringct/rctOps.h"

#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "multisig"

namespace multisig
{
   
  crypto::secret_key get_multisig_blinded_secret_key(const crypto::secret_key &key)
  {
    CHECK_AND_ASSERT_THROW_MES(key != crypto::null_skey, "Unexpected null secret key (danger!).");

    rct::key multisig_salt;
    static_assert(sizeof(rct::key) == sizeof(config::HASH_KEY_MULTISIG), "Hash domain separator is an unexpected size");
    memcpy(multisig_salt.bytes, config::HASH_KEY_MULTISIG, sizeof(rct::key));

     
    rct::keyV data;
    data.reserve(2);
    data.push_back(rct::sk2rct(key));
    data.push_back(multisig_salt);
    crypto::secret_key result = rct::rct2sk(rct::hash_to_scalar(data));
    memwipe(&data[0], sizeof(rct::key));
    return result;
  }
   
  bool generate_multisig_key_image(const cryptonote::account_keys &keys,
    std::size_t multisig_key_index,
    const crypto::public_key& out_key,
    crypto::key_image& ki)
  {
    if (multisig_key_index >= keys.m_multisig_keys.size())
      return false;
    crypto::generate_key_image(out_key, keys.m_multisig_keys[multisig_key_index], ki);
    return true;
  }
   
  void generate_multisig_LR(const crypto::public_key pkey,
    const crypto::secret_key &k,
    crypto::public_key &L,
    crypto::public_key &R)
  {
    rct::scalarmultBase((rct::key&)L, rct::sk2rct(k));
    crypto::generate_key_image(pkey, k, (crypto::key_image&)R);
  }
   
  bool generate_multisig_composite_key_image(const cryptonote::account_keys &keys,
    const std::unordered_map<crypto::public_key, cryptonote::subaddress_index> &subaddresses,
    const crypto::public_key &out_key,
    const crypto::public_key &tx_public_key,
    const std::vector<crypto::public_key> &additional_tx_public_keys,
    std::size_t real_output_index,
    const std::vector<crypto::key_image> &pkis,
    crypto::key_image &ki)
  {
     
     
     
     
    cryptonote::keypair in_ephemeral;
    if (!cryptonote::generate_key_image_helper(keys, subaddresses, out_key, tx_public_key, additional_tx_public_keys, real_output_index, in_ephemeral, ki, keys.get_device()))
      return false;
    std::unordered_set<crypto::key_image> used;

     
    for (std::size_t m = 0; m < keys.m_multisig_keys.size(); ++m)
    {
      crypto::key_image pki;
       
       
       
      bool r = generate_multisig_key_image(keys, m, out_key, pki);
      if (!r)
        return false;

       
      used.insert(pki);
    }

     
     
    for (const auto &pki: pkis)
    {
      if (used.find(pki) == used.end())
      {
         
        used.insert(pki);

         
        rct::addKeys((rct::key&)ki, rct::ki2rct(ki), rct::ki2rct(pki));
      }
    }

     
     
     

    return true;
  }
   
}  
