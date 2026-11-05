                                       
                                              

  
                       
   
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#pragma once

#include <array>
#include <boost/thread/mutex.hpp>
#include <boost/utility/string_ref.hpp>
#include <cstdint>
#include <deque>
#include <memory>
#include <vector>

#include "cryptonote_basic/fwd.h"
#include "net/zmq.h"
#include "span.h"
#include "cryptonote_basic/difficulty.h"

namespace cryptonote { namespace listener
{
 

 
 
 
 
 
 
class zmq_pub
{
   
 
 

    net::zmq::socket relay_;
    std::deque<std::vector<txpool_event>> txes_;
    std::array<std::size_t, 2> chain_subs_;
    std::array<std::size_t, 1> miner_subs_;
    std::array<std::size_t, 2> txpool_subs_;
    boost::mutex sync_;  

  public:
     
    static constexpr const char* relay_endpoint() noexcept { return "inproc://pub_relay"; }

    explicit zmq_pub(void* context);

    zmq_pub(const zmq_pub&) = delete;
    zmq_pub(zmq_pub&&) = delete;

    ~zmq_pub();

    zmq_pub& operator=(const zmq_pub&) = delete;
    zmq_pub& operator=(zmq_pub&&) = delete;

     
    bool sub_request(const boost::string_ref message);

     
 
    bool relay_to_pub(void* relay, void* pub);

     
 
 
    std::size_t send_chain_main(std::uint64_t height, epee::span<const cryptonote::block> blocks);

     
 
 
    std::size_t send_miner_data(uint8_t major_version, uint64_t height, const crypto::hash& prev_id, const crypto::hash& seed_hash, difficulty_type diff, uint64_t median_weight, uint64_t already_generated_coins, const std::vector<tx_block_template_backlog_entry>& tx_backlog);

     
 
 
    std::size_t send_txpool_add(std::vector<cryptonote::txpool_event> txes);

     
    struct chain_main
    {
      std::weak_ptr<zmq_pub> self_;
      void operator()(std::uint64_t height, epee::span<const cryptonote::block> blocks) const;
    };

     
    struct miner_data
    {
      std::weak_ptr<zmq_pub> self_;
      void operator()(uint8_t major_version, uint64_t height, const crypto::hash& prev_id, const crypto::hash& seed_hash, difficulty_type diff, uint64_t median_weight, uint64_t already_generated_coins, const std::vector<tx_block_template_backlog_entry>& tx_backlog) const;
    };

     
    struct txpool_add
    {
      std::weak_ptr<zmq_pub> self_;
      void operator()(std::vector<cryptonote::txpool_event> txes) const;
    };
  };
}}
