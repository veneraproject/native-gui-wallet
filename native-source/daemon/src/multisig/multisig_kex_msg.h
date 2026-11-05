                                       
                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#pragma once

#include "crypto/crypto.h"

#include <cstdint>
#include <vector>


namespace multisig
{
   
   
   
   
   
   
   
   
   
   
   
  class multisig_kex_msg final
  {
   

   
  public:
     
    multisig_kex_msg() = default;

     
    multisig_kex_msg(const std::uint32_t round,
      const crypto::secret_key &signing_privkey,
      std::vector<crypto::public_key> msg_pubkeys,
      const crypto::secret_key &msg_privkey = crypto::null_skey);

     
    multisig_kex_msg(std::string msg);

     

   
    ~multisig_kex_msg() = default;

   

   
     
    const std::string& get_msg() const { return m_msg; }
     
    std::uint32_t get_round() const { return m_kex_round; }
     
    const std::vector<crypto::public_key>& get_msg_pubkeys() const { return m_msg_pubkeys; }
     
    const crypto::secret_key& get_msg_privkey() const { return m_msg_privkey; }
     
    const crypto::public_key& get_signing_pubkey() const { return m_signing_pubkey; }

  private:
     
    crypto::hash get_msg_to_sign() const;
     
    void construct_msg(const crypto::secret_key &signing_privkey);
     
    void parse_and_validate_msg();

   
  private:
     
    std::string m_msg;

     
    std::uint32_t m_kex_round;
     
    std::vector<crypto::public_key> m_msg_pubkeys;
     
    crypto::secret_key m_msg_privkey;
     
    crypto::public_key m_signing_pubkey;
  };
}  
