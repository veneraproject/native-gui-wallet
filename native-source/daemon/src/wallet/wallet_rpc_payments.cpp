                                       
                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#include <boost/optional/optional.hpp>
#include <boost/utility/value_init.hpp>
#include "include_base_utils.h"
#include "cryptonote_config.h"
#include "wallet_rpc_helpers.h"
#include "wallet2.h"
#include "cryptonote_basic/cryptonote_format_utils.h"
#include "rpc/core_rpc_server_commands_defs.h"
#include "rpc/rpc_payment_signature.h"
#include "misc_language.h"
#include "cryptonote_basic/cryptonote_basic_impl.h"
#include "int-util.h"
#include "crypto/crypto.h"
#include "cryptonote_basic/blobdatatype.h"
#include "common/i18n.h"
#include "common/util.h"
#include "common/threadpool.h"

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "wallet.wallet2.rpc_payments"

#define RPC_PAYMENT_POLL_PERIOD 10  

namespace tools
{
 
std::string wallet2::get_client_signature() const
{
  return cryptonote::make_rpc_payment_signature(m_rpc_client_secret_key);
}
 
bool wallet2::get_rpc_payment_info(bool mining, bool &payment_required, uint64_t &credits, uint64_t &diff, uint64_t &credits_per_hash_found, cryptonote::blobdata &hashing_blob, uint64_t &height, uint64_t &seed_height, crypto::hash &seed_hash, crypto::hash &next_seed_hash, uint32_t &cookie)
{
  boost::optional<std::string> result = m_node_rpc_proxy.get_rpc_payment_info(mining, payment_required, credits, diff, credits_per_hash_found, hashing_blob, height, seed_height, seed_hash, next_seed_hash, cookie);
  credits = m_rpc_payment_state.credits;
  if (result && *result != CORE_RPC_STATUS_OK)
    return false;
  return true;
}
 
bool wallet2::daemon_requires_payment()
{
  bool payment_required = false;
  uint64_t credits, diff, credits_per_hash_found, height, seed_height;
  uint32_t cookie;
  cryptonote::blobdata blob;
  crypto::hash seed_hash, next_seed_hash;
  return get_rpc_payment_info(false, payment_required, credits, diff, credits_per_hash_found, blob, height, seed_height, seed_hash, next_seed_hash, cookie) && payment_required;
}
 
bool wallet2::make_rpc_payment(uint32_t nonce, uint32_t cookie, uint64_t &credits, uint64_t &balance)
{
  return false;  
  }
 
bool wallet2::search_for_rpc_payment(uint64_t credits_target, uint32_t n_threads, const std::function<bool(uint64_t, uint64_t)> &startfunc, const std::function<bool(unsigned)> &contfunc, const std::function<bool(uint64_t)> &foundfunc, const std::function<void(const std::string&)> &errorfunc)
{
  return false;  
  }
 
void wallet2::check_rpc_cost(const char *call, uint64_t post_call_credits, uint64_t pre_call_credits, double expected_cost)
{
  return tools::check_rpc_cost(m_rpc_payment_state, call, post_call_credits, pre_call_credits, expected_cost);
}
 
}
