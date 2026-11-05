                                       
                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#pragma once

#include "syncobj.h"
#include "hardforks/hardforks.h"
#include "cryptonote_basic/cryptonote_basic.h"

namespace cryptonote
{
  class BlockchainDB;

  class HardFork
  {
  public:
    typedef enum {
      LikelyForked,
      UpdateNeeded,
      Ready,
    } State;

    static const uint64_t DEFAULT_ORIGINAL_VERSION_TILL_HEIGHT = 0;  
    static const time_t DEFAULT_FORKED_TIME = 31557600;  
    static const time_t DEFAULT_UPDATE_TIME = 31557600 / 2;
    static const uint64_t DEFAULT_WINDOW_SIZE = 10080;  
    static const uint8_t DEFAULT_THRESHOLD_PERCENT = 80;

     
 
 
 
 
 
 
 
 
    HardFork(cryptonote::BlockchainDB &db, uint8_t original_version = 1, uint64_t original_version_till_height = DEFAULT_ORIGINAL_VERSION_TILL_HEIGHT, time_t forked_time = DEFAULT_FORKED_TIME, time_t update_time = DEFAULT_UPDATE_TIME, uint64_t window_size = DEFAULT_WINDOW_SIZE, uint8_t default_threshold_percent = DEFAULT_THRESHOLD_PERCENT);

     
 
 
 
 
 
 
 
 
 
    bool add_fork(uint8_t version, uint64_t height, uint8_t threshold, time_t time);

     
 
 
 
 
 
 
 
 
    bool add_fork(uint8_t version, uint64_t height, time_t time);

     
 
 
 
 
    void init();

     
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
    bool check(const cryptonote::block &block) const;

     
 
 
 
 
 
 
 
 
 
 
 
    bool check_for_height(const cryptonote::block &block, uint64_t height) const;

     
 
 
 
 
 
 
    bool add(const cryptonote::block &block, uint64_t height);

     
 
 
 
 
 
 
 
 
 
 
    bool reorganize_from_block_height(uint64_t height);
    bool reorganize_from_chain_height(uint64_t height);

     
 
 
 
 
 
 
 
    void on_block_popped(uint64_t new_chain_height);

     
 
 
 
 
 
 
 
    State get_state(time_t t) const;
    State get_state() const;

     
 
 
 
 
    uint8_t get(uint64_t height) const;

     
 
 
 
 
    uint8_t get_ideal_version() const;

     
 
 
 
 
    uint8_t get_ideal_version(uint64_t height) const;

     
 
 
 
 
    uint8_t get_next_version() const;

     
 
 
 
 
 
    uint8_t get_current_version() const;

     
 
 
    uint64_t get_earliest_ideal_height_for_version(uint8_t version) const;

     
 
 
 
 
 
 
 
 
 
 
 
    bool get_voting_info(uint8_t version, uint32_t &window, uint32_t &votes, uint32_t &threshold, uint64_t &earliest_height, uint8_t &voting) const;

     
 
 
    uint64_t get_window_size() const { return window_size; }

     
 
 
    const std::vector<hardfork_t>& get_hardforks() const { return heights; }

  private:

    uint8_t get_block_version(uint64_t height) const;
    bool do_check(uint8_t block_version, uint8_t voting_version) const;
    bool do_check_for_height(uint8_t block_version, uint8_t voting_version, uint64_t height) const;
    int get_voted_fork_index(uint64_t height) const;
    uint8_t get_effective_version(uint8_t voting_version) const;
    bool add(uint8_t block_version, uint8_t voting_version, uint64_t height);

    bool rescan_from_block_height(uint64_t height);
    bool rescan_from_chain_height(uint64_t height);

  private:

    BlockchainDB &db;

    time_t forked_time;
    time_t update_time;
    uint64_t window_size;
    uint8_t default_threshold_percent;

    uint8_t original_version;
    uint64_t original_version_till_height;

    std::vector<hardfork_t> heights;

    std::deque<uint8_t> versions;  
    unsigned int last_versions[256];  
    uint32_t current_fork_index;

    mutable epee::critical_section lock;
  };

}   

