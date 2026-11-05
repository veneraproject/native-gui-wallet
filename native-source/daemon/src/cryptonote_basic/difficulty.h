                                       
                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               
   
                                                                                      

#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <boost/multiprecision/cpp_int.hpp>
#include "crypto/hash.h"

namespace cryptonote
{
    typedef boost::multiprecision::uint128_t difficulty_type;

     
 
 
 
 
 
 
 
 
 
 
 
    bool check_hash_64(const crypto::hash &hash, uint64_t difficulty);
    uint64_t next_difficulty_64(std::vector<std::uint64_t> timestamps, std::vector<uint64_t> cumulative_difficulties, size_t target_seconds);

    bool check_hash_128(const crypto::hash &hash, difficulty_type difficulty);
    bool check_hash(const crypto::hash &hash, difficulty_type difficulty);
    difficulty_type next_difficulty(std::vector<std::uint64_t> timestamps, std::vector<difficulty_type> cumulative_difficulties, size_t target_seconds);
    difficulty_type next_difficulty_v2(std::vector<std::uint64_t> timestamps, std::vector<difficulty_type> cumulative_difficulties, size_t target_seconds);

    std::string hex(difficulty_type v);
}
