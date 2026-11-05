                                       
                                              
  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#include "multisig_account.h"

#include "crypto/crypto.h"
#include "cryptonote_config.h"
#include "include_base_utils.h"
#include "multisig.h"
#include "multisig_kex_msg.h"
#include "ringct/rctOps.h"

#include <boost/math/special_functions/binomial.hpp>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>


#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "multisig"

namespace multisig
{
   
   
 
 
 
 
 
 
 
   
  static void check_multisig_config(const std::uint32_t round,
    const std::uint32_t threshold,
    const std::uint32_t num_signers)
  {
    CHECK_AND_ASSERT_THROW_MES(num_signers > 1, "Must be at least one other multisig signer.");
    CHECK_AND_ASSERT_THROW_MES(num_signers <= config::MULTISIG_MAX_SIGNERS,
      "Too many multisig signers specified (limit = 16 to prevent dangerous combinatorial explosion during key exchange).");
    CHECK_AND_ASSERT_THROW_MES(num_signers >= threshold,
      "Multisig threshold may not be larger than number of signers.");
    CHECK_AND_ASSERT_THROW_MES(threshold > 0, "Multisig threshold must be > 0.");
    CHECK_AND_ASSERT_THROW_MES(round > 0, "Multisig kex round must be > 0.");
    CHECK_AND_ASSERT_THROW_MES(round <= multisig_setup_rounds_required(num_signers, threshold),
      "Trying to process multisig kex for an invalid round.");
  }
   
   
 
 
 
 
 
 
 
 
 
 
 
 
 
   
  static crypto::secret_key calculate_multisig_keypair_from_derivation(const crypto::public_key_memsafe &derivation,
    crypto::public_key &derived_pubkey_out)
  {
    crypto::secret_key blinded_skey = get_multisig_blinded_secret_key(rct::rct2sk(rct::pk2rct(derivation)));
    CHECK_AND_ASSERT_THROW_MES(crypto::secret_key_to_public_key(blinded_skey, derived_pubkey_out), "Failed to derive public key");

    return blinded_skey;
  }
   
   
 
 
 
 
 
 
 
   
  static void make_multisig_common_privkey(std::vector<crypto::secret_key> participant_base_common_privkeys,
    crypto::secret_key &common_privkey_out)
  {
     
     
    std::sort(participant_base_common_privkeys.begin(), participant_base_common_privkeys.end(),
        [](const crypto::secret_key &key1, const crypto::secret_key &key2) -> bool
        {
          return memcmp(&key1, &key2, sizeof(crypto::secret_key)) < 0;
        }
      );

     
    crypto::hash_to_scalar(participant_base_common_privkeys.data(),
      participant_base_common_privkeys.size()*sizeof(crypto::secret_key),
      common_privkey_out);

    CHECK_AND_ASSERT_THROW_MES(common_privkey_out != crypto::null_skey, "Unexpected null secret key (danger!).");
  }
   
   
 
 
 
 
 
 
 
 
 
 
 
 
 
 
   
  static rct::key compute_multisig_aggregation_coefficient(const std::vector<crypto::public_key> &sorted_keys,
    const crypto::public_key &aggregation_key)
  {
    CHECK_AND_ASSERT_THROW_MES(std::is_sorted(sorted_keys.begin(), sorted_keys.end()),
      "Keys for aggregation coefficient aren't sorted.");

     
    CHECK_AND_ASSERT_THROW_MES(std::find(sorted_keys.begin(), sorted_keys.end(), aggregation_key) != sorted_keys.end(),
      "Aggregation key expected to be in input keyset.");

     
    rct::key salt = rct::zero();
    static_assert(sizeof(rct::key) >= sizeof(config::HASH_KEY_MULTISIG_KEY_AGGREGATION), "Hash domain separator is too big.");
    memcpy(salt.bytes, config::HASH_KEY_MULTISIG_KEY_AGGREGATION, sizeof(config::HASH_KEY_MULTISIG_KEY_AGGREGATION));

     
    rct::keyV data;
    data.reserve(sorted_keys.size() + 2);
    data.push_back(rct::pk2rct(aggregation_key));
    for (const auto &key : sorted_keys)
      data.push_back(rct::pk2rct(key));
    data.push_back(salt);

     
    return rct::hash_to_scalar(data);
  }
   
   
 
 
 
 
 
 
 
 
 
 
   
  static crypto::public_key generate_multisig_aggregate_key(std::vector<crypto::public_key> final_keys,
    std::vector<crypto::secret_key> &privkeys_inout)
  {
     
    final_keys.reserve(final_keys.size() + privkeys_inout.size());

     
     
     
    std::unordered_map<crypto::public_key, std::size_t> own_keys_mapping;

    for (std::size_t multisig_keys_index{0}; multisig_keys_index < privkeys_inout.size(); ++multisig_keys_index)
    {
      crypto::public_key pubkey;
      CHECK_AND_ASSERT_THROW_MES(crypto::secret_key_to_public_key(privkeys_inout[multisig_keys_index], pubkey),
        "Failed to derive public key");

      own_keys_mapping[pubkey] = multisig_keys_index;

      final_keys.push_back(pubkey);
    }

     
     
    std::sort(final_keys.begin(), final_keys.end());
    CHECK_AND_ASSERT_THROW_MES(std::adjacent_find(final_keys.begin(), final_keys.end()) == final_keys.end(),
        "Unexpected duplicate found in input list.");

     
    rct::key aggregate_key = rct::identity();

    for (const crypto::public_key &key : final_keys)
    {
       
      rct::key coeff = compute_multisig_aggregation_coefficient(final_keys, key);

       
       
      auto found_key = own_keys_mapping.find(key);
      if (found_key != own_keys_mapping.end())
      {
         
        sc_mul((unsigned char*)&(privkeys_inout[found_key->second]),
          coeff.bytes,
          (const unsigned char*)&(privkeys_inout[found_key->second]));

        CHECK_AND_ASSERT_THROW_MES(privkeys_inout[found_key->second] != crypto::null_skey,
          "Multisig privkey with aggregation coefficient unexpectedly null.");
      }

       
       
      rct::key converted_pubkey = rct::scalarmultKey(rct::pk2rct(key), coeff);

       
      rct::addKeys(aggregate_key, aggregate_key, converted_pubkey);
    }

    return rct::rct2pk(aggregate_key);
  }
   
   
 
 
 
 
 
 
 
 
 
 
   
  static void multisig_kex_make_round_keys(const crypto::secret_key &base_privkey,
    multisig_keyset_map_memsafe_t pubkey_origins_map,
    multisig_keyset_map_memsafe_t &derivation_origins_map_out)
  {
     
    derivation_origins_map_out.clear();

    for (auto &pubkey_and_origins : pubkey_origins_map)
    {
       
       
       
       
      rct::key derivation_rct;
      auto a_wiper = epee::misc_utils::create_scope_leave_handler([&]{
        memwipe(&derivation_rct, sizeof(rct::key));
      });

      rct::scalarmultKey(derivation_rct, rct::pk2rct(pubkey_and_origins.first), rct::sk2rct(base_privkey));
      rct::scalarmultKey(derivation_rct, derivation_rct, rct::EIGHT);

       
       
      derivation_origins_map_out[rct::rct2pk(derivation_rct)] = std::move(pubkey_and_origins.second);
    }
  }
   
   
 
 
 
 
 
 
   
  static void check_messages_round(const std::vector<multisig_kex_msg> &expanded_msgs,
    const std::uint32_t expected_round)
  {
    CHECK_AND_ASSERT_THROW_MES(expanded_msgs.size() > 0, "At least one input message expected.");
    const std::uint32_t round{expanded_msgs[0].get_round()};
    CHECK_AND_ASSERT_THROW_MES(round == expected_round, "Messages don't have the expected kex round number.");

    for (const auto &expanded_msg : expanded_msgs)
      CHECK_AND_ASSERT_THROW_MES(expanded_msg.get_round() == round, "All messages must have the same kex round number.");
  }
   
   
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
   
  static std::uint32_t multisig_kex_msgs_sanitize_pubkeys(const std::vector<multisig_kex_msg> &expanded_msgs,
    const std::vector<crypto::public_key> &exclude_pubkeys,
    multisig_keyset_map_memsafe_t &sanitized_pubkeys_out)
  {
     
    CHECK_AND_ASSERT_THROW_MES(expanded_msgs.size() > 0, "At least one input message expected.");
    const std::uint32_t round{expanded_msgs[0].get_round()};
    check_messages_round(expanded_msgs, round);

    sanitized_pubkeys_out.clear();

     
     
    for (const auto &expanded_msg : expanded_msgs)
    {
       
      if (round == 1)
      {
         
        sanitized_pubkeys_out[expanded_msg.get_signing_pubkey()].insert(expanded_msg.get_signing_pubkey());
      }
       
      else
      {
         
        for (const auto &pubkey : expanded_msg.get_msg_pubkeys())
        {
           
          if (std::find(exclude_pubkeys.begin(), exclude_pubkeys.end(), pubkey) != exclude_pubkeys.end())
            continue;

           
          sanitized_pubkeys_out[pubkey].insert(expanded_msg.get_signing_pubkey());
        }
      }
    }

    return round;
  }
   
   
 
 
 
 
 
 
   
  static void remove_key_from_mapped_sets(const crypto::public_key &key_to_remove,
    multisig_keyset_map_memsafe_t &keyset_inout)
  {
     
    for (auto keyset_it = keyset_inout.begin(); keyset_it != keyset_inout.end();)
    {
       
      keyset_it->second.erase(key_to_remove);

       
      if (keyset_it->second.size() == 0)
        keyset_it = keyset_inout.erase(keyset_it);
      else
        ++keyset_it;
    }
  }
   
   
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
   
  static multisig_keyset_map_memsafe_t evaluate_multisig_kex_round_msgs(
    const crypto::public_key &base_pubkey,
    const std::uint32_t expected_round,
    const std::vector<crypto::public_key> &signers,
    const std::vector<multisig_kex_msg> &expanded_msgs,
    const std::vector<crypto::public_key> &exclude_pubkeys,
    const bool incomplete_signer_set)
  {
     
    for (auto it = exclude_pubkeys.begin(); it != exclude_pubkeys.end(); ++it)
    {
      CHECK_AND_ASSERT_THROW_MES(std::find(exclude_pubkeys.begin(), it, *it) == it,
        "Found duplicate pubkeys for exclusion unexpectedly.");
    }

     
    multisig_keyset_map_memsafe_t pubkey_origins_map;   
    const std::uint32_t round = multisig_kex_msgs_sanitize_pubkeys(expanded_msgs, exclude_pubkeys, pubkey_origins_map);
    CHECK_AND_ASSERT_THROW_MES(round == expected_round,
      "Kex messages were for round [" << round << "], but expected round is [" << expected_round << "]");

     
     
    remove_key_from_mapped_sets(base_pubkey, pubkey_origins_map);

     
    std::unordered_map<crypto::public_key, std::unordered_set<crypto::public_key>> origin_pubkeys_map;   

     
    const std::size_t num_recommendations_per_pubkey_required{
        incomplete_signer_set
        ? 1
        : round
      };

    for (const auto &pubkey_and_origins : pubkey_origins_map)
    {
       
       
       
      CHECK_AND_ASSERT_THROW_MES(pubkey_and_origins.second.size() >= num_recommendations_per_pubkey_required,
        "A pubkey recommended by multisig kex messages had an unexpected number of recommendations.");

       
      for (const auto &origin : pubkey_and_origins.second)
        origin_pubkeys_map[origin].insert(pubkey_and_origins.first);
    }

     
     
     
    const std::size_t num_signers_required{
        incomplete_signer_set
        ? signers.size() - 1 - (round - 1)
        : signers.size() - 1
      };

    CHECK_AND_ASSERT_THROW_MES(origin_pubkeys_map.size() >= num_signers_required,
      "Number of unique other signers recommending pubkeys does not equal number of required other signers "
      "(kex round: " << round << ", num signers found: " << origin_pubkeys_map.size() << ", num signers required: " <<
      num_signers_required << ").");

     

     
    auto n_choose_k_f =
      [](const std::uint32_t n, const std::uint32_t k) -> std::uint32_t
      {
        static_assert(std::numeric_limits<std::int32_t>::digits <= std::numeric_limits<double>::digits,
          "n_choose_k requires no rounding issues when converting between int32 <-> double.");

        if (n < k)
          return 0;

        double fp_result = boost::math::binomial_coefficient<double>(n, k);

        if (fp_result < 0)
          return 0;

        if (fp_result > std::numeric_limits<std::int32_t>::max())   
          return 0;

        return static_cast<std::uint32_t>(std::round(fp_result));
      };

     
       
       
       
       
       
       
       
       
    const std::uint32_t expected_recommendations_others = n_choose_k_f(signers.size() - 2, round - 1);

     
    const std::uint32_t expected_recommendations_self = n_choose_k_f(signers.size() - 1, round - 1);

     
     
    CHECK_AND_ASSERT_THROW_MES(expected_recommendations_self > 0 && expected_recommendations_others > 0,
      "Bad num signers or round num (possibly numerical limits exceeded).");

     
    CHECK_AND_ASSERT_THROW_MES(exclude_pubkeys.size() == expected_recommendations_self,
      "Local account did not recommend expected number of multisig keys.");

     
    for (const auto &origin_and_pubkeys : origin_pubkeys_map)
    {
      CHECK_AND_ASSERT_THROW_MES(origin_and_pubkeys.second.size() == expected_recommendations_others,
        "A multisig signer recommended an unexpected number of pubkeys.");

       
      CHECK_AND_ASSERT_THROW_MES(std::find(signers.begin(), signers.end(), origin_and_pubkeys.first) != signers.end(),
        "Multisig kex message with unexpected signer encountered.");
    }

     
    return pubkey_origins_map;
  }
   
   
 
 
 
 
 
 
 
 
 
 
 
 
 
   
  static multisig_keyset_map_memsafe_t evaluate_multisig_post_kex_round_msgs(
    const crypto::public_key &base_pubkey,
    const std::uint32_t expected_round,
    const std::vector<crypto::public_key> &signers,
    const std::vector<multisig_kex_msg> &expanded_msgs,
    const bool incomplete_signer_set)
  {
     
    const std::vector<crypto::public_key> dummy;
    multisig_keyset_map_memsafe_t pubkey_origins_map;   
    const std::uint32_t round = multisig_kex_msgs_sanitize_pubkeys(expanded_msgs, dummy, pubkey_origins_map);
    CHECK_AND_ASSERT_THROW_MES(round == expected_round,
      "Kex messages were for round [" << round << "], but expected round is [" << expected_round << "]");

     
     
     

     

     
    CHECK_AND_ASSERT_THROW_MES(pubkey_origins_map.size() == 2,
      "Multisig post-kex round messages from other signers did not all contain two pubkeys.");

     
    CHECK_AND_ASSERT_THROW_MES(pubkey_origins_map.begin()->second == (++(pubkey_origins_map.begin()))->second,
      "Multisig post-kex round messages from other signers did not all recommend the same pubkey pair.");

     
    auto origins = pubkey_origins_map.begin()->second;
    origins.insert(base_pubkey);   

    const std::size_t num_signers_required{
        incomplete_signer_set
        ? 1
        : signers.size()
      };

    CHECK_AND_ASSERT_THROW_MES(origins.size() >= num_signers_required,
      "Multisig post-kex round message origins don't line up with multisig signer set "
      "(num signers found: " << origins.size() << ", num signers required: " << num_signers_required << ").");

    for (const crypto::public_key &origin : origins)
    {
       
       
      CHECK_AND_ASSERT_THROW_MES(std::find(signers.begin(), signers.end(), origin) != signers.end(),
        "An unknown origin recommended a multisig post-kex verification messsage.");
    }

    return pubkey_origins_map;
  }
   
   
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
   
  static void multisig_kex_process_round_msgs(const crypto::secret_key &base_privkey,
    const crypto::public_key &base_pubkey,
    const std::uint32_t current_round,
    const std::uint32_t threshold,
    const std::vector<crypto::public_key> &signers,
    const std::vector<multisig_kex_msg> &expanded_msgs,
    const std::vector<crypto::public_key> &exclude_pubkeys,
    const bool incomplete_signer_set,
    multisig_keyset_map_memsafe_t &keys_to_origins_map_out)
  {
    check_multisig_config(current_round, threshold, signers.size());
    const std::uint32_t kex_rounds_required{multisig_kex_rounds_required(signers.size(), threshold)};

     
    multisig_keyset_map_memsafe_t evaluated_pubkeys;

    if (threshold == 1 && current_round == kex_rounds_required)
    {
       
       
    }
    else if (current_round <= kex_rounds_required)
    {
       
      evaluated_pubkeys = evaluate_multisig_kex_round_msgs(base_pubkey,
        current_round,
        signers,
        expanded_msgs,
        exclude_pubkeys,
        incomplete_signer_set);
    }
    else  
    {
       
      evaluated_pubkeys = evaluate_multisig_post_kex_round_msgs(base_pubkey,
        current_round,
        signers,
        expanded_msgs,
        incomplete_signer_set);
    }

     
    if (current_round < kex_rounds_required)
    {
       
      multisig_kex_make_round_keys(base_privkey, std::move(evaluated_pubkeys), keys_to_origins_map_out);
    }
    else if (current_round >= kex_rounds_required)
    {
       
       
      keys_to_origins_map_out = std::move(evaluated_pubkeys);
    }
  }
   
   
   
  void multisig_account::initialize_kex_update(const std::vector<multisig_kex_msg> &expanded_msgs,
    const std::uint32_t kex_rounds_required,
    std::vector<crypto::public_key> &exclude_pubkeys_out)
  {
    if (m_kex_rounds_complete == 0)
    {
       

       
       
       
      std::vector<crypto::secret_key> participant_base_common_privkeys;
      participant_base_common_privkeys.reserve(expanded_msgs.size() + 1);

       
      participant_base_common_privkeys.emplace_back(m_base_common_privkey);

       
      for (const auto &expanded_msg : expanded_msgs)
      {
        if (expanded_msg.get_signing_pubkey() != m_base_pubkey)
        {
          participant_base_common_privkeys.emplace_back(expanded_msg.get_msg_privkey());
        }
      }

       
      make_multisig_common_privkey(std::move(participant_base_common_privkeys), m_common_privkey);

       
      CHECK_AND_ASSERT_THROW_MES(crypto::secret_key_to_public_key(m_common_privkey, m_common_pubkey),
        "Failed to derive public key");

       
      if (kex_rounds_required == 1)
      {
        m_multisig_privkeys.clear();
        m_multisig_privkeys.emplace_back(m_base_privkey);
      }

       
       
      exclude_pubkeys_out.emplace_back(m_base_pubkey);
    }
    else
    {
       

       
      for (const auto &shared_key_with_origins : m_kex_keys_to_origins_map)
      {
        exclude_pubkeys_out.emplace_back(shared_key_with_origins.first);
      }
    }
  }
   
   
   
  void multisig_account::finalize_kex_update(const std::uint32_t kex_rounds_required,
    multisig_keyset_map_memsafe_t result_keys_to_origins_map)
  {
    std::vector<crypto::public_key> next_msg_keys;

     
    if (m_kex_rounds_complete == kex_rounds_required)
    {
       
      CHECK_AND_ASSERT_THROW_MES(result_keys_to_origins_map.count(m_multisig_pubkey) > 0,
        "Multisig post-kex round: expected multisig pubkey wasn't found in input messages.");
      CHECK_AND_ASSERT_THROW_MES(result_keys_to_origins_map.count(m_common_pubkey) > 0,
        "Multisig post-kex round: expected common pubkey wasn't found in input messages.");

       
       
      next_msg_keys.reserve(2);
      next_msg_keys.push_back(m_multisig_pubkey);
      next_msg_keys.push_back(m_common_pubkey);
    }
    else if (m_kex_rounds_complete + 1 == kex_rounds_required)
    {
       

       
      std::vector<crypto::public_key> result_keys;
      result_keys.reserve(result_keys_to_origins_map.size());

      for (const auto &result_key_and_origins : result_keys_to_origins_map)
      {
        result_keys.emplace_back(result_key_and_origins.first);
      }

       
      m_multisig_pubkey = generate_multisig_aggregate_key(std::move(result_keys), m_multisig_privkeys);

       
       
      m_kex_keys_to_origins_map.clear();

       
       
      next_msg_keys.reserve(2);
      next_msg_keys.push_back(m_multisig_pubkey);
      next_msg_keys.push_back(m_common_pubkey);
    }
    else if (m_kex_rounds_complete + 2 == kex_rounds_required)
    {
       
       

       

       
       
       
       
      m_multisig_privkeys.clear();
      m_multisig_privkeys.reserve(result_keys_to_origins_map.size());

      m_kex_keys_to_origins_map.clear();
      next_msg_keys.reserve(result_keys_to_origins_map.size());

      for (const auto &derivation_and_origins : result_keys_to_origins_map)
      {
         
         
        crypto::public_key_memsafe derived_pubkey;
        m_multisig_privkeys.push_back(
          calculate_multisig_keypair_from_derivation(derivation_and_origins.first, derived_pubkey));

         
        m_kex_keys_to_origins_map[derived_pubkey] = std::move(derivation_and_origins.second);

         
         
         
        next_msg_keys.push_back(derived_pubkey);
      }
    }
    else   
    {
       

       
       
      next_msg_keys.reserve(result_keys_to_origins_map.size());

      for (const auto &derivation_and_origins : result_keys_to_origins_map)
        next_msg_keys.push_back(derivation_and_origins.first);

       
      m_kex_keys_to_origins_map = std::move(result_keys_to_origins_map);
    }

     
    ++m_kex_rounds_complete;

     
    m_next_round_kex_message = multisig_kex_msg{
      (m_kex_rounds_complete > kex_rounds_required ? kex_rounds_required : m_kex_rounds_complete) + 1,
      m_base_privkey,
      std::move(next_msg_keys)}.get_msg();
  }
   
   
   
  void multisig_account::kex_update_impl(const std::vector<multisig_kex_msg> &expanded_msgs,
    const bool incomplete_signer_set)
  {
     
    check_messages_round(expanded_msgs, m_kex_rounds_complete + 1);

     
    const std::uint32_t kex_rounds_required{multisig_kex_rounds_required(m_signers.size(), m_threshold)};

    CHECK_AND_ASSERT_THROW_MES(kex_rounds_required > 0, "Multisig kex rounds required unexpectedly 0.");
    CHECK_AND_ASSERT_THROW_MES(m_kex_rounds_complete < kex_rounds_required + 1,
      "Multisig kex has already completed all required rounds (including post-kex verification).");

     
    std::vector<crypto::public_key> exclude_pubkeys;
    initialize_kex_update(expanded_msgs, kex_rounds_required, exclude_pubkeys);

     
    multisig_keyset_map_memsafe_t result_keys_to_origins_map;
    multisig_kex_process_round_msgs(
      m_base_privkey,
      m_base_pubkey,
      m_kex_rounds_complete + 1,
      m_threshold,
      m_signers,
      expanded_msgs,
      exclude_pubkeys,
      incomplete_signer_set,
      result_keys_to_origins_map);

     
    finalize_kex_update(kex_rounds_required, std::move(result_keys_to_origins_map));
  }
   
}  
