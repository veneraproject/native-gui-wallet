                                       
                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#pragma once

#include "crypto/crypto.h"
#include "multisig_kex_msg.h"

#include <cstdint>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>


namespace multisig
{
   
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
  using multisig_keyset_map_memsafe_t = 
    std::unordered_map<crypto::public_key_memsafe, std::unordered_set<crypto::public_key>>;

  class multisig_account final
  {
  public:
   
     
    multisig_account() = default;

     
 
 
 
 
 
 
    multisig_account(const crypto::secret_key &base_privkey,
      const crypto::secret_key &base_common_privkey);

     
    multisig_account(const std::uint32_t threshold,
      std::vector<crypto::public_key> signers,
      const crypto::secret_key &base_privkey,
      const crypto::secret_key &base_common_privkey,
      std::vector<crypto::secret_key> multisig_privkeys,
      const crypto::secret_key &common_privkey,
      const crypto::public_key &multisig_pubkey,
      const crypto::public_key &common_pubkey,
      const std::uint32_t kex_rounds_complete,
      multisig_keyset_map_memsafe_t kex_origins_map,
      std::string next_round_kex_message);

     

   
    ~multisig_account() = default;

   

   
     
    std::uint32_t get_threshold() const { return m_threshold; }
     
    const std::vector<crypto::public_key>& get_signers() const { return m_signers; }
     
    const crypto::secret_key& get_base_privkey() const { return m_base_privkey; }
     
    const crypto::public_key& get_base_pubkey() const { return m_base_pubkey; }
     
    const crypto::secret_key& get_base_common_privkey() const { return m_base_common_privkey; }
     
    const std::vector<crypto::secret_key>& get_multisig_privkeys() const { return m_multisig_privkeys; }
     
    const crypto::secret_key& get_common_privkey() const { return m_common_privkey; }
     
    const crypto::public_key& get_multisig_pubkey() const { return m_multisig_pubkey; }
     
    const crypto::public_key& get_common_pubkey() const { return m_common_pubkey; }
     
    std::uint32_t get_kex_rounds_complete() const { return m_kex_rounds_complete; }
     
    const multisig_keyset_map_memsafe_t& get_kex_keys_to_origins_map() const { return m_kex_keys_to_origins_map; }
     
    const std::string& get_next_kex_round_msg() const { return m_next_round_kex_message; }

   
     
    bool account_is_active() const;
     
    bool main_kex_rounds_done() const;
     
    bool multisig_is_ready() const;

   
  private:
     
    void set_multisig_config(const std::size_t threshold, std::vector<crypto::public_key> signers);

   
  public:
     
 
 
 
    void initialize_kex(const std::uint32_t threshold,
      std::vector<crypto::public_key> signers,
      const std::vector<multisig_kex_msg> &expanded_msgs_rnd1);
     
 
 
 
 
 
 
 
 
 
 
 
 
 
    void kex_update(const std::vector<multisig_kex_msg> &expanded_msgs,
      const bool force_update_use_with_caution = false);

  private:
     
    void kex_update_impl(const std::vector<multisig_kex_msg> &expanded_msgs, const bool incomplete_signer_set);
     
 
 
 
 
 
 
 
 
    void initialize_kex_update(const std::vector<multisig_kex_msg> &expanded_msgs,
      const std::uint32_t kex_rounds_required,
      std::vector<crypto::public_key> &exclude_pubkeys_out);
     
 
 
 
 
 
    void finalize_kex_update(const std::uint32_t kex_rounds_required,
      multisig_keyset_map_memsafe_t result_keys_to_origins_map);

   
  private:
     
     
    std::uint32_t m_threshold{0};
     
    std::vector<crypto::public_key> m_signers;

     
     
     
    crypto::secret_key m_base_privkey;
    crypto::public_key m_base_pubkey;
     
    crypto::secret_key m_base_common_privkey;

     
     
     
    std::vector<crypto::secret_key> m_multisig_privkeys;
     
    crypto::secret_key m_common_privkey;
     
    crypto::public_key m_multisig_pubkey;
     
    crypto::public_key m_common_pubkey;

     
     
    std::uint32_t m_kex_rounds_complete{0};
     
     
    multisig_keyset_map_memsafe_t m_kex_keys_to_origins_map;
     
    std::string m_next_round_kex_message;
  };

   
 
 
 
 
 
 
 
 
 
 
  std::uint32_t multisig_kex_rounds_required(const std::uint32_t num_signers, const std::uint32_t threshold);

   
 
 
 
 
 
 
  std::uint32_t multisig_setup_rounds_required(const std::uint32_t num_signers, const std::uint32_t threshold);
}  
