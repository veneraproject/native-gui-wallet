                                       
                                              
  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#pragma once

 
#include "crypto/crypto.h"
#include "cryptonote_basic/account.h"
#include "cryptonote_basic/subaddress_index.h"
#include "cryptonote_config.h"
#include "wallet2.h"

 

 
#include <functional>
#include <boost/optional.hpp>
#include <unordered_map>
#include <vector>

namespace tools
{
namespace wallet
{
 
 
 
 
 
 
 
void check_consistent_ins_outs(const std::vector<wallet2::tx_construction_data> &txes);
void check_consistent_ins_outs(const std::vector<wallet2::pending_tx> &txes);
void sanity_check_tx_construction_data(const wallet2::tx_construction_data &tx);
 
void sanity_check_unsigned_tx_set(const std::vector<wallet2::tx_construction_data> &txes,
    const cryptonote::account_keys &account_keys,
    const std::unordered_map<crypto::public_key, cryptonote::subaddress_index> &subaddresses);
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
void sanity_check_pending_tx(const wallet2::pending_tx &ptx,
    const cryptonote::network_type nettype,
    const cryptonote::account_keys &account_keys,
    const std::unordered_map<crypto::public_key, cryptonote::subaddress_index> &subaddresses,
    const std::vector<wallet2::transfer_details> &transfers,
    const bool redacted,
    const boost::optional<std::function<const crypto::key_image(const size_t)>> &transfer_ki_resolver,
    const bool allow_read_only);
 
void sanity_check_pending_tx_set(const std::vector<wallet2::pending_tx> &ptxs,
    const cryptonote::network_type nettype,
    const cryptonote::account_keys &account_keys,
    const std::unordered_map<crypto::public_key, cryptonote::subaddress_index> &subaddresses,
    const std::vector<wallet2::transfer_details> &transfers,
    const bool redacted,
    const boost::optional<std::function<const crypto::key_image(const size_t)>> &transfer_ki_resolver,
    const bool allow_read_only);
}  
}  
