                                       
                                              
  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               
  
                                                                                      

#pragma once
#include <map>
#include "misc_log_ex.h"
#include "crypto/hash.h"
#include "cryptonote_config.h"
#include "cryptonote_basic/difficulty.h"

#define ADD_CHECKPOINT(h, hash)  CHECK_AND_ASSERT(add_checkpoint(h,  hash), false);
#define ADD_CHECKPOINT2(h, hash, difficulty)  CHECK_AND_ASSERT(add_checkpoint(h,  hash, difficulty), false);
#define JSON_HASH_FILE_NAME "checkpoints.json"


namespace cryptonote
{
   
 
 
 
 
 
 
  class checkpoints
  {
  public:

     
 
 
    checkpoints();

     
 
 
 
 
 
 
 
 
 
 
    bool add_checkpoint(uint64_t height, const std::string& hash_str, const std::string& difficulty_str = "");

     
 
 
 
 
 
 
 
 
 
 
    bool is_in_checkpoint_zone(uint64_t height) const;

     
 
 
 
 
 
 
 
 
 
 
 
 
 
 
    bool check_block(uint64_t height, const crypto::hash& h, bool& is_a_checkpoint) const;

     
 
 
    bool check_block(uint64_t height, const crypto::hash& h) const;

     
 
 
 
 
 
 
 
 
 
 
 
 
 
    bool is_alternative_block_allowed(uint64_t blockchain_height, uint64_t block_height) const;

     
 
 
 
 
    uint64_t get_max_height() const;

     
 
 
 
 
    const std::map<uint64_t, crypto::hash>& get_points() const;

     
 
 
 
 
    const std::map<uint64_t, difficulty_type>& get_difficulty_points() const;

     
 
 
 
 
 
 
 
 
 
    bool check_for_conflicts(const checkpoints& other) const;

     
 
 
 
 
 
    bool init_default_checkpoints(network_type nettype);

     
 
 
 
 
 
 
 
 
 
 
 
    bool load_new_checkpoints(const std::string &json_hashfile_fullpath, network_type nettype=MAINNET, bool dns=true);

     
 
 
 
 
 
 
    bool load_checkpoints_from_json(const std::string &json_hashfile_fullpath);

     
 
 
 
 
 
 
    bool load_checkpoints_from_dns(network_type nettype = MAINNET);

  private:
    std::map<uint64_t, crypto::hash> m_points;  
    std::map<uint64_t, difficulty_type> m_difficulty_points;  
  };

}
